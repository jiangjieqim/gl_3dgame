#ifndef _EVT_H_
#define _EVT_H_

//事件结构体
struct EvtInfo{
	int evt;
	int ptr;
};

//绑定事件
void
evt_on(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*));

//解绑事件
void
evt_off(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*));
#endif