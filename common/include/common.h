#ifndef _COMMON_H_
#define _COMMON_H_

#include "def.h"

//DLLIMPORT 
//int x_md2load(const char* path);

/*
 *	判断是否存在文件file,存在返回1 不存在返回0
 *	19:35 2017/7/8
 */
DLLIMPORT 
int tl_is_file_exist(const char* file);
/*
	将数据buffer 增量写入filePath中
	-------------------------------------------------
	wb 只写打开或新建一个二进制文件；只允许写数据。 
	wb+ 读写打开或建立一个二进制文件，允许读和写。 
	wt+ 读写打开或着建立一个文本文件；允许读写。 
	at+ 读写打开一个文本文件，允许读或在文本末追加数据。 
	ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。 
*/
DLLIMPORT 
void  tl_writeAppandFile(const char* filePath,char* buffer);
/*
	构造一个新名字
*/
DLLIMPORT
void tl_newName(char* buffer,int bufferSize);

//DLLIMPORT int dllGlobalVar;

/*
	相对于程序的路径
 */
DLLIMPORT char* g_local_path[256];

/*路径转化*/
DLLIMPORT int tl_convert_path(char* in,char* out,int outBufferSize);


#endif