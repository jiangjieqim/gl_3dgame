#ifndef _BMP_H_
#define _BMP_H_
/*位图处理*/
/*
	解析位图 photoshop 文件格式:Windows 深度 24位
*/
unsigned char* tl_bmp24(const char* path,int* biWidth,int* biHeight);

#endif