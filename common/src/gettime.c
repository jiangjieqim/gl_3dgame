//#pragma warning(disable:4996) 
//#define BUILDING_DLL

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include <stdarg.h>

#define BUILDING_DLL

#include "tools.h"
#include "tl_malloc.h"
#include "gettime.h"
#include "common.h"


#define G_SIGN_BR "\n"			/*换行*/
//日志状态
static int g_log_stat;

//static char logpath[G_BUFFER_64_SIZE];//日志路径
static char* logpath = "c://3dgameLog.txt";
long get_longTime(){
	clock_t start=clock(); 
	return start;
}
//float gettime_cur(){ 
//	float currentTime = (float)clock()*0.001f;
//	return currentTime;
//}

//static void f_error(int code);
/*
	转化颜色,成功返回1
*/
static void
f_converColor(int color,WORD* w){
	WORD word;
	
	if(color == 0xff0000){
		word = FOREGROUND_RED ;
	}
	else if(color == 0x00ff00){
		word = FOREGROUND_GREEN ;
	}
	else if(color == 0xffff00){
		word = FOREGROUND_GREEN | FOREGROUND_RED;//yellow
	}
	else if(color == 0xff00ff){
		word = FOREGROUND_GREEN | FOREGROUND_BLUE;//紫红
	}
	else if(color == 0x0000ff){
		word = FOREGROUND_BLUE ;
	}
	else if(color == 0xffffff){
		word = FOREGROUND_RED |  FOREGROUND_GREEN | FOREGROUND_BLUE;
	}
	else if( color == 0x0){
		word = 0;
	}
	else{
		word = FOREGROUND_RED;
		//log_code(ERROR_BAD_COLOR_VALUE);
		//f_error(ERROR_BAD_COLOR_VALUE);
		printf("没有对应的配置颜色!\n");
		assert(0);
	}
	*w = word;
}
static HANDLE
GetHandle(){
	HANDLE hOut; 

	//  获取输出流的句柄
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	return hOut;
}
static void
f_push_color_str(int color,char* s){
	if(g_log_stat){
		HANDLE hOut =  GetHandle();
		WORD word;
		f_converColor(color,&word);
		SetConsoleTextAttribute(hOut, word | FOREGROUND_INTENSITY);
		printf("%s",s);
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN |FOREGROUND_BLUE ); 
	}
	else{
		printf("%s",s);
	}
}
void get_allTime(char* out,int bufferSize)
{
	time_t time_seconds = time(0);  
	struct tm now_time;  
	localtime_s(&now_time,&time_seconds); 
	memset(out,0,bufferSize);
	sprintf_s(out,bufferSize,"%d-%d-%d %d:%d:%d\n",now_time.tm_year + 1900, now_time.tm_mon + 1,  now_time.tm_mday, now_time.tm_hour, now_time.tm_min, now_time.tm_sec);  
}

void 
log_write(const char* _str){
	#define WRIET_LOG		//是否写入文件日志

#define SHOW_TIME		//是否显示时间
	int len = (int)strlen(_str)+G_BUFFER_64_SIZE;
	char* _res = NULL;
	char* _tstr = (char*)tl_malloc(len);
	time_t timep;

	char TIME_STR[G_BUFFER_64_SIZE];
	struct tm _tm;
	time (&timep);

	gmtime_s(&_tm,&timep);
	asctime_s(TIME_STR,G_BUFFER_64_SIZE,/*gmtime(&timep)*/&_tm);
	memset(_tstr,0,len);

#ifdef SHOW_TIME
	//时间
	_res = (char*)tl_malloc(len);
	memset(_res,0,len);

	
	sprintf_s(_tstr,G_BUFFER_256_SIZE,"[%s]%s",/*asctime(gmtime(&timep))*/TIME_STR,_str);
	tl_replace(_tstr,_res,G_SIGN_BR,"");
	memset(_tstr,0,len);
	memcpy(_tstr,_res,strlen(_res));
	tl_free(_res);
	_res=0;
	sprintf_s(_tstr,G_BUFFER_256_SIZE,"%s\n",_tstr);
#else
	sprintf_s(_tstr,G_BUFFER_256_SIZE,"%s\n",_str);
#endif

#ifdef WRIET_LOG
	tl_writeAppandFile(logpath,_tstr);
#endif

	printf("%s",_tstr);
	tl_free(_tstr);
	_tstr=0;
}



//########################################################


static double 
sum(int lim, ...)
{	
	va_list ap; // declare variable used to save para
	double total = 0;
	int i;
	va_start(ap, lim);// initialize ap to para list
	for(i=0; i<lim; i++){
		total += va_arg(ap, double); // visit para list
	}
	va_end(ap); // clean
	return(total);
}


//static void f_converColor(int color,WORD* w);
//static void
//f_error(int code){
//
//	/*
//	HANDLE hOut =  GetHandle();
//	WORD word;
//	f_converColor(0xff0000,&word);
//	SetConsoleTextAttribute(hOut, word | FOREGROUND_INTENSITY);
//	printf("code:%d\n",code); 
//	SetConsoleTextAttribute(hOut, FOREGROUND_RED |   FOREGROUND_GREEN | FOREGROUND_BLUE ); 
//	*/
//	char s[G_BUFFER_8_SIZE];
//	sprintf_s(s,G_BUFFER_8_SIZE,"code:%d\n",code);
//	f_PushColorString(0xff0000,s);
//}



/************************************************************************
* 输出一段有色彩的日志             
* unsigned short color 颜色
/************************************************************************/
static void
f_log_put(const char* s,int color){

	/*
	HANDLE hOut =  GetHandle();
	WORD word;
	
	//test code
	//color = 0x00ffe0;//一个错误的测试颜色值
	
	f_converColor(color,&word);

	SetConsoleTextAttribute(hOut, word | FOREGROUND_INTENSITY);
	
	printf("%s",s); 
	
	SetConsoleTextAttribute(hOut, FOREGROUND_RED |   FOREGROUND_GREEN | FOREGROUND_BLUE ); 
	*/

	char _str[G_BUFFER_256_SIZE];
	sprintf_s(_str,G_BUFFER_256_SIZE,"%s",s);
	f_push_color_str(color,_str);
}

#define __SIZE 1024	//缓冲区大小
static int _log_status = 0;//默认的日志开启状态
void log_enable(int v){
	_log_status = v;
}
int 
log_color(int color,char* format, ...){

	char str_tmp[__SIZE];
	int i=0;
	va_list vArgList;                          
	va_start (vArgList, format);     
	if(!_log_status){
		return 0;
	}
	//i=_vsnprintf(str_tmp, __SIZE, format, vArgList); 
	i = _vsnprintf_s(str_tmp, __SIZE,__SIZE, format, vArgList);
	//#################################################
	{
/*
		HANDLE hOut =  GetHandle();
		WORD word;
		f_converColor(color,&word);
		SetConsoleTextAttribute(hOut, word | FOREGROUND_INTENSITY);
		//这里可以解析str_tmp 做相关业务处理
		printf("%s",str_tmp);
		SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN |FOREGROUND_BLUE ); 
		*/

		char s[__SIZE];
		sprintf_s(s,__SIZE,"%s",str_tmp);
		f_push_color_str(color,s);
	}
	va_end(vArgList); 
	return i;                                 
}

void
log_code(int code){
	char str[G_BUFFER_32_SIZE];
	memset(str,0,G_BUFFER_32_SIZE);
	sprintf_s(str,G_BUFFER_32_SIZE,"code:%d\n",code);
	f_log_put(str,0xff0000);
}

void
log_state(int value){
	g_log_stat = value;
}

void sleep(long ms){
	Sleep(ms);
}