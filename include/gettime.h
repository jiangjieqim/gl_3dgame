#ifndef _GET_TIME_H_
#define _GET_TIME_H_

#define	ERROR_SUIFFIX  1//错误的后缀
#define	ERROR_BAD_COLOR_VALUE  2//错误的颜色值
#define	ERROR_NOT_EXIST_FILE  3	//无法找到文件
#define	ERROR_NOT_EXIST_Atals 4	//无法找到图集
#define	ERROR_PTR_REMOVE_FAIL 5	//ptr_remove删除对象失败
#define	ERROR_PTR_TYPE_FAIL 6	//类型匹配错误,未实现该类型的逻辑
#define ERROR_BAD_VALUE 7	//错误值
#define	ERROR_FLAGS 8	//错误的标示状态位
#define ERROR_GET_FRAME 9 //获取动作帧
#define ERROR_RENAME 10		//重命名
#define ERROR_SET_FRAME 11 //设置动作帧
/*
 *	获取当前时间 精确到豪秒
 *	clock函数测出来的时间为进程运行时间，单位为滴答数(ticks)。
 */
long get_longTime();

/*生成当前的时间2015/1/2 0:20:1*/
void get_allTime(char* out,int bufferSize);
/*输出日志 ,写入本地或者上传到web服务器*/
void log_write(const char* _str);
/*
	获取当前运行的时间
*/
float gettime_cur();

/************************************************************************
* 输出一段有色彩的日志             
* unsigned short color 颜色
/************************************************************************/
void
log_put(const char* s,int color);
/************************************************************************/
/* 输出错误代码                                                                     */
/************************************************************************/
void 
log_code(int c);

/*
*	输出指定color颜色的日志
*/
int 
log_color(int color,char* format, ...);
#endif