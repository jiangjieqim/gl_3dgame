#include "tools.h"
#include "evt.h"
//事件结构体
struct EvtInfo{
	/*
	 *  事件 id
	 */
	int evtId;
	/*
	 *	回调函数引用
	 */
	void (*ptr)(int evtId,void* data);
};
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
evt_on(void* evtList,int evtId,void (*evtCallBack)(int,void*)){
	struct EvtInfo* data;
	struct LStackNode* list;
	if(!f_check_evt(evtList,evtId,evtCallBack)){
		return;
	}
	data = (struct EvtInfo*)tl_malloc(sizeof(struct EvtInfo));
	data->evtId = evtId;
	data->ptr = evtCallBack;
	list =(struct LStackNode*)evtList;
	LStack_push(list,data);
}

//解绑事件
void
evt_off(void* evtList,int event,void (*evtCallBack)(int,void*)){
	struct LStackNode* s = (struct LStackNode* )evtList;
	struct EvtInfo* node;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;

		p=(void*)LStack_next(p);
		data = LStack_data(p);

		node = (struct EvtInfo*)data;

		
		if(node->evtId == event && node->ptr == evtCallBack){
			node->evtId = 0;
			node->ptr = 0;
			LStack_delNode(s,data);
			tl_free(node);
		}
	}

}

void
evt_dispatch(void* evtList,int evtID,void* sendData){
	struct LStackNode* s = (struct LStackNode* )evtList;
	struct EvtInfo* node;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;

		p=(void*)LStack_next(p);
		data = LStack_data(p);
		
		node = (struct EvtInfo*)data;

		if(node->evtId == evtID){
			node->ptr(evtID,sendData);
		}
	}

}

void 
evt_dispose(void* evtList){
	struct LStackNode* s = (struct LStackNode* )evtList;
	struct EvtInfo* node;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;

		p=(void*)LStack_next(p);
		data = LStack_data(p);

		node = (struct EvtInfo*)data;

		node->evtId = 0;
		node->ptr = 0;
		LStack_delNode(s,data);
		tl_free(node);
	}
	LStack_delete(s);
}