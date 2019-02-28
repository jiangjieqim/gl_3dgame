#ifndef _FT_FONT_
#define _FT_FONT_

//https://blog.csdn.net/wesleyluo/article/details/7432063
#include "def.h"
/*
 *	tl_malloc返回一个rgba的字节数据,取出之后记得tl_free掉数据
 */
DLLIMPORT void* ft_load(unsigned char* buffer,int fontw,int fonth,int *iWidth, int *iHeight,char* str);

#endif