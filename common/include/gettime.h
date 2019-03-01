#ifndef _GET_TIME_H_
#define _GET_TIME_H_

#include "def.h"

#define	ERROR_SUIFFIX  1//错误的后缀
#define	ERROR_BAD_COLOR_VALUE  2//错误的颜色值
#define	ERROR_NOT_EXIST_FILE  3	//无法找到文件
#define	ERROR_NOT_EXIST_Atals 4	//无法找到图集
#define	ERROR_PTR_REMOVE_FAIL 5	//ptr_remove删除对象失败
#define	ERROR_PTR_TYPE_FAIL 6	//类型匹配错误,未实现该类型的逻辑
#define ERROR_BAD_VALUE 7	//错误值,crash的数据
#define	ERROR_FLAGS 8	//错误的标示状态位
#define ERROR_GET_FRAME 9 //获取动作帧
#define ERROR_RENAME 10		//重命名
#define ERROR_SET_FRAME 11 //设置动作帧
#define ERROR_FUNCTION_WAIT_TO_DO 12 //待实现
/*
 *	获取当前时间 精确到豪秒
 *	clock函数测出来的时间为当前进程已经运行的时间，单位为滴答数(ticks)。
 */
DLLIMPORT long get_longTime();

/*生成当前的时间2015/1/2 0:20:1*/
DLLIMPORT void get_allTime(char* out,int bufferSize);

/*输出日志 ,写入本地或者上传到web服务器*/
DLLIMPORT void log_write(const char* _str);
/*
	获取当前运行的时间
*/
//DLLIMPORT float gettime_cur();

/************************************************************************/
/* 输出错误代码                                                 */
/************************************************************************/
DLLIMPORT void 
log_code(int c);

/*
*	输出指定color颜色的日志
*/
DLLIMPORT int 
log_color(int color,char* format, ...);

/************************************************************************
* 1开启/0关闭 输出颜色日志的开关                                  
************************************************************************/
DLLIMPORT void
log_state(int value);

//进程睡眠ms毫秒
DLLIMPORT void sleep(long ms);

#endif