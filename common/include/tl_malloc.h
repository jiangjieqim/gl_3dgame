#ifndef _TL_MALLOC_H_
#define _TL_MALLOC_H_
/************************************************************************/
/* 内存管理                                                             */
/************************************************************************/

#include "def.h"

//内存池信息
struct MemInfo{
	//总字节长度
	int total;
	//未使用的节点个数
	int disable_cnt;
	//未使用的节点数据字节总大小
	int disable_bytes;
};

/*
	申请内存
*/
//DLLIMPORT void* memory_new(int size);
/*
	释放内存,只是释放引用,但是系统内存块并没有回收.
*/
//DLLIMPORT void memory_free(void* p);

/*
	直接系统回收,使用的free进行堆内存回收,真正意义上的回收,
	但是性能不如memory_free内存池回收来的好
*/
//DLLIMPORT void memory_retrieve(void* p);
/*
	获取信息
*/
//DLLIMPORT void memory_get_info(int* pDisable_bytes,int* pDisable_cnt,int* pg_total);
DLLIMPORT void memory_info(struct MemInfo* info);

/*
	Garbage Collection	垃圾回收
*/
DLLIMPORT void memory_gc();

/************************************************************************/
/* 总共向系统开辟里的内存大小(字节)                                                                     */
/************************************************************************/
//int memory_get_total();

/*开辟一块内存*/
DLLIMPORT void* tl_malloc(int size);

/*释放一块内存*/
DLLIMPORT void tl_free(void* s);

#endif