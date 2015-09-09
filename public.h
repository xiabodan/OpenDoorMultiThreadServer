#ifndef PUBLIC_H
#define PUBLIC_H

//时间结构体
typedef struct TimeS{
	int h;
	int m;
	int s;
}TimeS;

extern int OpenDoor(void);
extern bool GetTime(void);
extern TimeS* TimeSub(TimeS* ctime, TimeS*  ptime);
extern TimeS* TimeAdd(TimeS* ctime, TimeS*  ptime);
extern char** GetWeekWorkTimeTotal(char*** r,int row ,int col);
extern char** GetTodayWorkTimeTotal(char*** r,int row ,int col);
extern char** GetWeekWorkTimeTotal1(char*** r,int row ,int col);

extern bool Free3point(char*** r,int row,int col);
extern bool Free2point(char**  r,int row,int col);
#endif