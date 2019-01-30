#ifndef _EVT_H_
#define _EVT_H_

/*
	注意:观察者模式的缺点是在观察者众多的时候,如果事件派发在一个渲染循环里面,那么性能是低下的.如果在传递数据频率不高,为了降低耦合度用这种方式是首选.
*/

//绑定事件
void
evt_on(void* ptr,int id,void (*evtCallBack)(int,void*));

//解绑事件
void
evt_off(void* ptr,int id,void (*evtCallBack)(int,void*));

//派发事件
void
evt_dispatch(void* ptr,int evtID,void* sendData);

//销毁事件句柄
void 
evt_dispose(void* ptr);
#endif