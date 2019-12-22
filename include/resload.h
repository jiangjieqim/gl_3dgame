#ifndef _RES_LOAD_H_
#define _RES_LOAD_H_
//创建一个加载对象
void* resload_create(void(*endCallBack)(void*,void*),void* alals,void* param);
//压入一个资源节点
void resload_push(void* ptr,const char* tex);
//开始记加载资源
void resload_start(void* ptr);
#endif