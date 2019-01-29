#ifndef _EVT_H_
#define _EVT_H_

//绑定事件
void
evt_on(void* evtList,int id,void (*evtCallBack)(int,void*));

//解绑事件
void
evt_off(void* evtList,int id,void (*evtCallBack)(int,void*));

//派发事件
void
evt_dispatch(void* evtList,int evtID,void* sendData);

//销毁事件句柄
void 
evt_dispose(void* evtList);
#endif