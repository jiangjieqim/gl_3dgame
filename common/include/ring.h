#ifndef _RING_H_
#define _RING_H_
#include "def.h"
//创建环
DLLIMPORT void* ring_create();
//压入数据
DLLIMPORT void ring_push(void* ptr,void* node);
//下一个数据
DLLIMPORT void* ring_next(void* ptr);
//前一个数据
DLLIMPORT void* ring_pre(void* ptr);
//销毁环
DLLIMPORT void ring_dispose(void* ptr);
//获取当前的环选择的数据
DLLIMPORT void* ring_cur(void* ptr);
#endif