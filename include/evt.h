#ifndef _EVT_H_
#define _EVT_H_



//事件结构体
struct EvtInfo{
	/*
	 *  事件 id
	 */
	int evtId;
	/*
	 *	回调函数引用
	 */
	void (*ptr)();
};

//绑定事件
void
evt_on(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*));

//解绑事件
void
evt_off(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*));
#endif