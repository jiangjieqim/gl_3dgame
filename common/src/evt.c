#define BUILDING_DLL

#include "tools.h"
#include "evt.h"
#include "tl_malloc.h"
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
	
	/************************************************************************/
	/* 用完事件直接删除                                                                     */
	/************************************************************************/
	int removed;
};


struct EvtParent{
	void* evtList;
};
//获取对象ptr引用中的evtList引用地址.
static void* 
f_get(void* ptr){
	if(ptr){
		struct EvtParent* p = (struct EvtParent*)ptr;
		return p->evtList;
	}
	return 0;
}
/************************************************************************/
/* 检测是否已经绑定了函数                                               */
/************************************************************************/
static int 
f_check_evt(void* evtList,int event,void (*evtCallBack)(int,void*)){

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
evt_on(void* ptr,int evtId,void (*evtCallBack)(int,void*)){
	void* evtList = f_get(ptr);
	
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

void
evt_once(void* ptr,int evtId,void (*evtCallBack)(int,void*)){
	void* evtList = f_get(ptr);

	struct EvtInfo* data;
	struct LStackNode* list;

	if(!f_check_evt(evtList,evtId,evtCallBack)){
		return;
	}
	data = (struct EvtInfo*)tl_malloc(sizeof(struct EvtInfo));
	data->removed = 1;
	data->evtId = evtId;
	data->ptr = evtCallBack;
	list =(struct LStackNode*)evtList;
	LStack_push(list,data);

}


//解绑事件
void
evt_off(void* ptr,int event,void (*evtCallBack)(int,void*)){
	void* evtList = f_get(ptr);
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
			break;
			//tl_free((void*)node);

			//printf("len= %d\n",LStack_length(s));
		}
	}

}

void
evt_dispatch(void* ptr,int evtID,void* sendData){
	void* evtList = f_get(ptr);
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
			if(node->removed)
			{
				//printf("%d\n",LStack_length(s));
				evt_off(ptr,node->evtId,node->ptr);
				//printf("%d\n",LStack_length(s));
				break;
			}
		}
	}

}

void 
evt_dispose(void* ptr){
	void* evtList = f_get(ptr);
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
		//tl_free(node);
	}
	LStack_delete(s);
}