#ifndef _FT_FONT_
#define _FT_FONT_

//https://blog.csdn.net/wesleyluo/article/details/7432063

/*
 *	tl_malloc返回一个rgba的字节数据,取出之后记得tl_free掉数据
 */
void* ft_load(int *iWidth, int *iHeight);

#endif