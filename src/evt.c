#include "tools.h"
#include "evt.h"
/************************************************************************/
/* 检测是否已经绑定了函数                                               */
/************************************************************************/
static int 
f_check_evt(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*)){

	struct LStackNode* s = (struct LStackNode* )evtList;
	struct EvtInfo* node;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;

		p=(void*)LStack_next(p);
		data = LStack_data(p);
		//if(!callBack(data,parm)){
		//	//callBack返回0的时候中断遍历
		//	return;
		//}

		node = (struct EvtInfo*)data;

		//if(data == (int)evtCallBack){
		if(node->evtId == event && node->ptr == evtCallBack){
			//已注册该函数
			return 0;
		}
	}

	return 1;
}
//绑定事件
void
evt_on(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*)){
	if(!f_check_evt(evtList,event,evtCallBack)){
		return;
	}

}

//解绑事件
void
evt_off(void* evtList,int event,void (*evtCallBack)(struct EvtInfo*)){

}