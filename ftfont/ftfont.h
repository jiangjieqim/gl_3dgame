#ifndef _FT_FONT_
#define _FT_FONT_

//https://blog.csdn.net/wesleyluo/article/details/7432063
#include "def.h"

/*
 *	tl_malloc返回一个rgba的字节数据,取出之后记得tl_free掉数据,该接口调用完成之后会自动销毁资源句柄
 */
//DLLIMPORT void ft_load(unsigned char* buffer,int fontw,int fonth,int *iWidth, int *iHeight,char* str);

/************************************************************************/
/* 创建字体绘制设备句柄                                                                     */
/************************************************************************/
DLLIMPORT
void* ft_create(const char* fileName);
/************************************************************************/
/* 解析输出像素数据                                                                     */
/************************************************************************/
DLLIMPORT
void ft_parse(void* pt,unsigned char* outBuffer,int fontw,int fonth,int *iWidth, int *iHeight,int* iTop,char* str);

/*销毁字体绘制设备*/
DLLIMPORT
void ft_free(void* pt);
#endif