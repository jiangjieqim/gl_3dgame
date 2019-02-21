#ifndef _ARR_
#define _ARR_

#include "def.h"

#define  ARRAY_HEAP 1	//堆内存需要tl_free释放
#define  ARRAY_STACK 0 //栈内存,自动释放

/*
	可边长数据
*/
typedef struct Array{
	int* ptr;
	int length;	//数组长度
	int cur;	//当前游标
	int mode;	//数组模式
}Array;
/*
	数组创建
*/
DLLIMPORT struct Array* array_create(int length,int mode);

/*
	释放数组
*/
DLLIMPORT void array_free(struct Array* ptr);
/*
	存储数据局
*/
DLLIMPORT void array_put(struct Array* ptr,void* d);
/*
	数组长度
*/
DLLIMPORT int array_len(struct Array* ptr);
/*
	获取数据找到数据返回1,没有数据就0
*/
DLLIMPORT int array_get(struct Array* ptr,int index,int* outData);
/*
	设置数据
*/
DLLIMPORT int array_set(struct Array* p,int index,int data);
/*
	打印数组数据
*/
//DLLIMPORT void array_log(struct Array* p);

/*
	根据key切割成多个字符文件,保存在数组中,字符串切割成字符串数组
*/
DLLIMPORT struct Array* array_split(const char* str,char key);

#endif