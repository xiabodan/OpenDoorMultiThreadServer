#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <wiringPi.h>  
#include "public.h"
//---开门函数----
int OpenDoor(void)  
{  
  wiringPiSetup() ;  
  pinMode (29, OUTPUT) ;  
  digitalWrite(29, HIGH) ; delay (100) ;  
  digitalWrite(29, LOW ) ; delay (100) ;   
  pinMode (29, INPUT) ; 

  return 0; 
}  


//---测试时间----
bool GetTime(void)
{
	//获取当前时间
	time_t now;   
	struct tm *timenow = (struct tm*)malloc(sizeof(struct tm));   
	time(&now);   
	timenow = localtime(&now);  
	printf("recent time is : %s \n", asctime(timenow));  	//recent time is : Fri Sep  4 22:45:22 2015
	printf("tm_isdst = %d \n",timenow->tm_isdst);			//表是否为日光节约时间
	printf("tm_yday = %d \n",timenow->tm_yday);				//本年第几日,在[0,365]之间，闰年有366日
	printf("tm_wday = %d \n",timenow->tm_wday);    			//星期几[0-6] 0: SUN  6:SAT
	
	printf("tm_year = %d-",timenow->tm_year + 1900);		//tm_year 要加上1900才是当前年
	printf("%d-",timenow->tm_mon + 1);						//在[0-11]之间
	printf("%d \n",timenow->tm_mday);						//在[1,31]之间
	printf("time = %d: ",timenow->tm_hour);					//在[0,23]之间
	printf("%d: ",timenow->tm_min);							//在[0,59]之间	
	printf("%d\n",timenow->tm_sec);							//在[0,61]之间，多出来的两秒是用来处理跳秒问题用的
	//free(timenow);
}

//计算两个时间之差函数
//return  = ctime - ptime
TimeS* TimeSub(TimeS* ctime, TimeS*  ptime)
{
	//计算小时分钟，秒忽略不计了
	TimeS *t = (TimeS*)malloc(sizeof(TimeS));    //用户自己释放
	if(NULL != t && NULL != ctime && NULL != ptime){
		t->h = ctime->h - ptime->h;
		if(ctime->m - ptime->m < 0){
			t->m = 60 + ctime->m - ptime->m;
			t->h -= 1;
		}else{
			t->m = ctime->m - ptime->m;
		}
		if(ctime->s - ptime->s < 0){
			t->s = 60 + ctime->s - ptime->s;
			t->m -= 1;
		}else{
			t->s = ctime->s - ptime->s;
		}
		if(t->h < 0) t->h = 0;
		if(t->m < 0) t->m = 0;
		if(t->s < 0) t->s = 0;
	}
	return t;
}

//计算两个时间之和函数
//return  = ctime + ptime
TimeS* TimeAdd(TimeS* ctime, TimeS*  ptime)
{
	//计算小时分钟，秒忽略不计了
	TimeS *t = (TimeS*)malloc(sizeof(TimeS));    //用户自己释放
	if(NULL != t && NULL != ctime && NULL != ptime){
		if((ctime->s + ptime->s) < 60){
			t->s = ctime->s + ptime->s;
			t->m = 0;
		}else{
			t->s = ctime->s + ptime->s - 60;
			t->m = 1;
		}
		if((ctime->m + ptime->m + t->m) < 60){
			t->m = ctime->m + ptime->m + t->m;
			t->h = 0;
		}else{
			t->m =  ctime->m + ptime->m + t->m - 60;
			t->h = 1;
		}
		t->h += ctime->h + ptime->h;
	}
	return t;
}

//将字符串格式的时间转换为TimeS结构体
TimeS StrToTimeS(char* s)
{
	TimeS t;
	//实现.....
	int firstfenhao = 0;
	int secondfenhao = 0;
	for( int i=0;i<strlen(s);i++)
	{
		if((s[i] )== ':')
		{
			if(firstfenhao == 0) {
				firstfenhao  = i;
			}else{
				secondfenhao = i;
			}
		}
	}
	t.h = atoi(s);
	t.m = atoi(s+firstfenhao+1);
	t.s = atoi(s+secondfenhao+1);
	
	return t;
}
//将TimeS结构体转换为字符串格式的时间
char* TimeSToStr(TimeS ts)
{
	char* t = (char*)malloc(sizeof(char) * 10);
	//实现.....
	sprintf(t,"%02d:%02d:%02d",ts.h,ts.m,ts.s);
	
	return t;
}
//计算两个字符串格式的时间之和函数
//return = st1 + st2
//"12:33:43" = "9:10:23" + "3:23:20"
char* StrTimeAdd(char* st1, char* st2)
{
	if(NULL == st1 || NULL == st2)	return NULL;
	//printf("st1 = %s,strlen st1 = %d,sizeof st1 = %d, st2 = %s , strlen st2 = %d,sizeof st2 = %d\n",st1,strlen(st1),sizeof(st1),st2,strlen(st2),sizeof(st2));
	TimeS t1 = StrToTimeS(st1);
	//printf("t1.h = %d, t1.m = %d, t1.s = %d\n",t1.h,t1.m,t1.s);
	TimeS t2 = StrToTimeS(st2);
	//printf("t2.h = %d, t2.m = %d, t2.s = %d\n",t2.h,t2.m,t2.s);
	TimeS *t3 = TimeAdd(&t1,&t2);
	//printf("t3.h = %d, t3.m = %d, t3.s = %d\n",t3->h,t3->m,t3->s);
	char* t = TimeSToStr(*t3);
	//printf("string t  = %s\n",t);
	
	free(t3);
	return t;
}
//计算两个字符串格式的时间之和函数
//return = st1 - st2
//"9:10:23" =  "12:33:43 " - "3:23:20"
char* StrTimeSub(char* st1, char* st2)
{
	if(NULL == st1 || NULL == st2)	return NULL;
	//printf("st1 = %s,strlen st1 = %d,sizeof st1 = %d, st2 = %s , strlen st2 = %d,sizeof st2 = %d\n",st1,strlen(st1),sizeof(st1),st2,strlen(st2),sizeof(st2));
	TimeS t1 = StrToTimeS(st1);
	//printf("t1.h = %d, t1.m = %d, t1.s = %d\n",t1.h,t1.m,t1.s);
	TimeS t2 = StrToTimeS(st2);
	//printf("t2.h = %d, t2.m = %d, t2.s = %d\n",t2.h,t2.m,t2.s);
	TimeS *t3 = TimeSub(&t1,&t2);
	//printf("t3.h = %d, t3.m = %d, t3.s = %d\n",t3->h,t3->m,t3->s);
	char* t = TimeSToStr(*t3);
	//printf("string t  = %s\n",t);
	
	free(t3);
	return t;
}
//获取本天打卡时间总和，并返回总时间"xx:xx:xx"字符串
char** GetTodayWorkTimeTotal(char*** r,int row ,int col)
{
	if(col  != 2) return NULL;
	if(NULL == r) return NULL;
	
	char **t = (char**)malloc(sizeof(char*) * row);   //用户释放内存
	for(int i=0;i<row;i++){
		t[i] = StrTimeSub(r[i][1],r[i][0]);
	}
	return t;
}

//获取本周打卡时间总和，并返回总时间"xx:xx:xx"字符串,TimeTotal = peopletodayworktime + peopletimetotal
char** GetWeekWorkTimeTotal(char*** r,int row ,int col)
{
	if(col  != 2) return NULL;
	if(NULL == r) return NULL;
	
	char **t = (char**)malloc(sizeof(char*) * row);   //用户释放内存
	for(int i=0;i<row;i++){
		t[i] = StrTimeAdd(r[i][0],r[i][1]);
	}
	return t;
}

//获取本周打卡时间总和，并返回总时间"xx:xx:xx"字符串,TimeTotal = peopletimetotal + peopleworkafter - peopleworkbegin 
char** GetWeekWorkTimeTotal1(char*** r,int row ,int col)
{
	if(col  != 3) return NULL;
	if(NULL == r) return NULL;
	
	char **t = (char**)malloc(sizeof(char*) * row);   //用户释放内存
	char **t1 = (char**)malloc(sizeof(char*) * row);   //用户释放内存
	for(int i=0;i<row;i++){
		t1[i] = StrTimeAdd(r[i][0],r[i][1]);
		t[i] = StrTimeSub(t1[i]   ,r[i][2]);
	}
	for(int i=0;i<row;i++){
		free(t1[i]);
	}
	free(t1);
	return t;
}

//释放三级指针
bool Free3point(char*** r,int row,int col)
{
	//显示打印
	/*for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			printf("%s\t", r[i][j]);
		}
		printf("\n");
	}*/
	//释放内存
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			free(r[i][j]);
			r[i][j] = NULL;
		}
		free(r[i]);
		r[i] = NULL;
	}
	free(r);
	r = NULL;
	
	return true;
}
//释放二级指针
bool Free2point(char** r,int row,int col)
{
	//显示打印
	/*for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			printf("%s\t", r[i][j]);
		}
		printf("\n");
	}*/
	//释放内存
	for(int i=0;i<row;i++){
		free(r[i]);
		r[i] = NULL;
	}
	free(r);
	r = NULL;
	
	return true;
}
