#ifndef _EVT_H_
#define _EVT_H_
#include "def.h"

/*
	注意:观察者模式的缺点是在观察者众多的时候,如果事件派发在一个渲染循环里面,那么性能是低下的.如果在传递数据频率不高,为了降低耦合度用这种方式是首选.
*/

//绑定事件
// thisObj绑定的数据
DLLIMPORT void
evt_on(void* ptr,int id,void (*evtCallBack)(int,void*,void*),void* thisObj);

//监听一次释放掉事件
DLLIMPORT void
evt_once(void* ptr,int id,void (*evtCallBack)(int,void*,void*),void* thisObj);

//解绑事件
DLLIMPORT void
evt_off(void* ptr,int id,void (*evtCallBack)(int,void*,void*));

//派发事件
DLLIMPORT void
evt_dispatch(void* ptr,int evtID,void* sendData);

//销毁事件句柄
DLLIMPORT void 
evt_dispose(void* ptr);
#endif