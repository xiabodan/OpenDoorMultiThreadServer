#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//第三方库
#include <pthread.h>
//自己实现的数据库MySql接口
#include "mydb.h"
#include "public.h"
#include "log.h"
//macro define
#define PORT 3333
#define BACKLOG 5
//global variable
CMyDB MySqlDB; //实例一个SQL对象，操作数据库
pthread_mutex_t mutexsql;//互斥锁
typedef struct {
    int connectfd;
    struct sockaddr_in client;
}targs;

void *process(void *arg){
    targs client;
    client.connectfd=((targs *)arg)->connectfd;
    client.client=((targs *)arg)->client;
    char buf[1024];
	char* ipaddress;
    int n=0;
	//time get
	time_t now;   
	struct tm *timenow = (struct tm*)malloc(sizeof(struct tm));   
	time(&now);   
	timenow = localtime(&now); 
	//read socket datas
    while(0<(n=read(client.connectfd,buf,sizeof(buf)))){
        buf[n]='\0';
		ipaddress = inet_ntoa(client.client.sin_addr);
        printf("You got a message <%s> from %s.\n",buf,inet_ntoa(client.client.sin_addr));
		
		//open door
        if(buf[0] == '1' && strlen(buf) == 1){
			sprintf(buf,"\n%s : %s",ipaddress,"OpenDoor"); 	//年月日时间更新值
			Fprint_LogString(buf,LOG_OPENDOOR_FILE,"a+");
			OpenDoor();
		}
		
		//SQL updata
		char buffer[400] = {0};
		char day[20] ;
		sprintf(day,"%d-%d-%d",timenow->tm_year + 1900,timenow->tm_mon + 1,timenow->tm_mday); 	//年月日时间更新值

		char tm[7][50] =  {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};  
		char wday[100] ;
		sprintf(wday,"%s",tm[timenow->tm_wday ]); 												//星期更新值

		char btime[20];
		sprintf(btime,"%02d:%02d:%02d",timenow->tm_hour,timenow->tm_min,timenow->tm_sec); 			//当前时间更新值,上班
		char atime[20];
		sprintf(atime,"%02d:%02d:%02d",timenow->tm_hour,timenow->tm_min,timenow->tm_sec); 			//当前时间更新值,下班
		//加锁
		pthread_mutex_lock (&mutexsql);
		//上下班统计次数
		int atimef = 0;
		int row=0;
		int col=0;	
		sprintf(buffer,"select opentimenum from opentime where opentimeip='%s';",ipaddress);
		char***r = MySqlDB.executeSQL(buffer,&row,&col);
		if(NULL != r){
			if(atoi(r[0][0]) == 0) 
			{
				atimef = 0;
				//保存当天打卡次数
				sprintf(buffer,"update opentime  set opentimenum=%d where opentimeip='%s';",atoi(r[0][0])+1,ipaddress);
				MySqlDB.executeSQL(buffer,&row,&col);
			}
			else
			{
				atimef = 1;
				//保存当天打卡次数
				sprintf(buffer,"update opentime  set opentimenum=%d where opentimeip='%s';",atoi(r[0][0])+1,ipaddress);
				MySqlDB.executeSQL(buffer,&row,&col);
			}
			Free3point(r,row,col);
		}
		
		
		if(!atimef){	//第一次打卡
			sprintf(buffer,"update people p set p.peoplertimetoday='%s',p.peopleweektoday ='%s',p.peopleworkbegin ='%s' where peopleipaddress='%s';"\
																	,day																			\
																						,wday														\
																												,btime								\
																																		,ipaddress);									
			MySqlDB.executeSQL(buffer,&row,&col);
		}else{			//下班有可能多次打卡
			sprintf(buffer,"update people p set p.peoplertimetoday='%s',p.peopleweektoday ='%s',p.peopleworkafter ='%s' where peopleipaddress='%s';"\
																	,day																			\
																						,wday														\
																												,atime								\
																																		,ipaddress);									
			MySqlDB.executeSQL(buffer,&row,&col);
			
			sprintf(buffer,"select peopleworkbegin, peopleworkafter from people where peopleipaddress='%s';",ipaddress);	
			char** chtime;
			char***r = MySqlDB.executeSQL(buffer,&row,&col);
			if(NULL != r){
				chtime = GetTodayWorkTimeTotal(r,row,col);
			}
			Free3point(r,row,col);
			sprintf(buffer,"update people p set p.peopletodayworktime='%s' where peopleipaddress='%s';",chtime[0], ipaddress);	
			MySqlDB.executeSQL(buffer,&row,&col);												
		}
		pthread_mutex_unlock (&mutexsql); //解锁
        write(client.connectfd,buf,strlen(buf));
    }
    close(client.connectfd);
}

//SQL更新线程
int preday = 0;
void* sqlupdate_start(void *message)
{
	time_t now;   
	struct tm *timenow = (struct tm*)malloc(sizeof(struct tm));  
	int row=0;
	int col=0;	
	char buffer[400] = {0};		//小心溢出！
	sleep(5);
	//读取配置文件
	sprintf(buffer,"select configday,configweek from config where idconfig=1;");
	char***r1 = MySqlDB.executeSQL(buffer,&row,&col);
	int oldday = atoi(r1[0][0]) ;	//每天更新一次, 包括更新本周总上班时间，清零当天打卡时间，更新日期，星期几，将上下班打卡时间标记为"无记录"
	int oldweek = atoi(r1[0][1]);	//每周更新一次,	包括清零本周总上班时间，清零当天打卡时间，更新日期，星期几，将上下班打卡时间标记为"无记录"
	Free3point(r1,row,col);
	//printf("oldday = %d,oldweek = %d \n",oldday,oldweek);
	while(1){
		sleep(5);
		time(&now);   
		timenow = localtime(&now);  
		int newday = timenow->tm_mday;	//几号
		int newweek= timenow->tm_wday; 	//星期几
		if((preday != newday) && (newday != oldday))
		{
			//SQL updata
			//printf("newday = %d, oldday = %d, update SQL excuse\n",newday,oldday);
			char day[20]    ;
			sprintf(day,"%d-%d-%d",timenow->tm_year + 1900,timenow->tm_mon + 1,timenow->tm_mday); 	//年月日时间更新值

			char tm[7][100] = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};  
			char wday[100]  ;
			sprintf(wday,"%s",tm[timenow->tm_wday]); 											//星期更新值
			char btime[20] = {"无记录"};		//上班时间，清零
			char atime[20] = {"无记录"};		//下班时间，清零 
			char wtime[20] = {"00:00:00"} ;		//本周上班中总时间 += 当日上班总时间  
			char dtime[20] = {"00:00:00"} ;		//当日上班总时间，清零
			char** htime;
			char***r = MySqlDB.executeSQL("select peopletimetotal,peopleworkafter, peopleworkbegin from people where idpeople>0;",&row,&col);
			if(NULL != r){
				htime = GetWeekWorkTimeTotal1(r,row,col);
				//for(int i=0;i<row;i++){
				//	printf("%s \n",htime[i]);
				//}	
			}
			Free3point(r,row,col);
			if(newweek == 1 ){
				sprintf(buffer,"update people p set p.peoplertimetoday='%s',p.peopleweektoday ='%s',p.peopleworkbegin ='%s',p.peopleworkafter ='%s',p.peopletimetotal ='%s',p.peopletodayworktime ='%s' where idpeople>0;"	\
																	,day																																					\
																							,wday																															\
																													,btime																									\
																																			,atime																			\
																																									,wtime													\					
																																																,dtime);						
				MySqlDB.executeSQL(buffer,&row,&col); //不是线程安全的，以后记得修改，加锁
			}
			else{
				for(int i=0;i<row;i++){
					printf("htime = %s \n",htime[i]);
					sprintf(buffer,"update people p set p.peoplertimetoday='%s',p.peopleweektoday ='%s',p.peopleworkbegin ='%s',p.peopleworkafter ='%s',p.peopletimetotal ='%s',p.peopletodayworktime ='%s' where idpeople=%d;"	\
																		,day																																					\
																								,wday																															\
																														,btime																									\
																																				,atime																			\
																																										,htime[i]				 								\					
																																																	,dtime						\
																																																						,i+1);						
					MySqlDB.executeSQL(buffer,&row,&col); //不是线程安全的，以后记得修改，加锁
				}
			}

			Free2point(htime,row,col);
			
			//清除当日开门次数opentimenum
			sprintf(buffer,"update opentime  set opentimenum=0 where idopentime>0;");
			MySqlDB.executeSQL(buffer,&row,&col);
			//将当天的配置文件写入数据库中，避免系统重启时，数据被清空
			sprintf(buffer,"update config set configday=%d, configweek=%d where idconfig=1;",newday,newweek);
			MySqlDB.executeSQL(buffer,&row,&col);
		}
		preday = newday;
	}
	return message;
}

int main(){
	//互斥锁初始化
	pthread_mutex_init(&mutexsql, NULL);
	//-----SQL更新线程--------
	pthread_t sqlupdatethread;
	pthread_create(&sqlupdatethread,NULL,sqlupdate_start,(void *)0);
	//-----打印当前时间-------
	GetTime();
	//-----mysql初始化--------
	int row=0;
	int col=0;	
    MySqlDB.initDB("localhost" , "root", "xiabo891219" , "r114" );//链接数据库
	char***r = MySqlDB.executeSQL("select * from people;",&row,&col);//打印所有信息
	Free3point(r,row,col);
	char***r1 = MySqlDB.executeSQL("select * from opentime;",&row,&col);//打印所有信息
	Free3point(r1,row,col);
	//MySqlDB.executeSQL("select peopleworkbegin from people where idpeople>0;",&row,&col);//打印所有信息
	//MySqlDB.executeSQL("update people p set p.peopletimetotal=0,p.peopletodayworktime=0 where idpeople>0;",&row,&col);
	printf("\n");
	//-----socket-------------
    int listenfd,connectfd;
    struct sockaddr_in server;
    struct sockaddr_in client;

    if(-1==(listenfd=socket(AF_INET,SOCK_STREAM,0))){
        perror("create socket error\n");
        exit(1);
    }

    int opt=SO_REUSEADDR;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=htonl(INADDR_ANY);

    if(-1==(bind(listenfd,(struct sockaddr*)&server,sizeof(struct sockaddr)))){
        perror("bind error\n");
        exit(1);
    }

    if(-1==(listen(listenfd,BACKLOG))){
        perror("listen error\n");
        exit(1);
    }

    socklen_t len=sizeof(client);
    targs arg;

    while(1){
        if(-1==(connectfd=accept(listenfd,(struct sockaddr *)&client,&len))){
            perror("accept error\n");
            exit(1);
        }
        arg.connectfd=connectfd;
        arg.client=client;

        pthread_t tid;
        if(pthread_create(&tid,NULL,process,(void *)&arg)){
            perror("create thread error\n");
            exit(1);
        }
    }
    close(listenfd);
    return 0;
}

