#include "log.h"

//系统日志打印

string GetCTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	string date = asctime(timeinfo);
	//cout<<date<<endl;
	return date ;
}
void Fprint_String(const char *buffer,const char* Outname,const char* Mode)  
{  
    //a+ : create append  w+: create rewrite  
    FILE *fp ;  
  
    fp=fopen(Outname,Mode);  
    fprintf(fp,"%s",buffer);  
    fclose(fp);  
}  
void Fprint_Num(int num,const char *Outname,const char *Mode)  
{  
    FILE *fp ;  
    fp = fopen(Outname,Mode);  
  
    fprintf(fp,"%d ",num);  
  
    fclose(fp);  
} 
void Fprint_LogString(const char *buffer,const char* Outname,const char* Mode)  
{  
	string wholeLog ;
	string currentTime = GetCTime();
	wholeLog.append("\n");
	wholeLog.append(currentTime);
	wholeLog.append(buffer);
	
	Fprint_String(wholeLog.c_str(),Outname,Mode);
} 

