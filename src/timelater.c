#include <memory.h>
#include <stdio.h>
#include <assert.h>
#include "tools.h"
#include "timelater.h"
#include "tl_malloc.h"
#include "evt.h"
#include "ex.h"

struct TimeLaterNode{
	int delay;
	int old;
	//(*callBack)(void*);
	int cur;//当前timer已经运行了的毫秒数
	void (*callBack)(void*);
	void* param;
};

void* timelater_new(int ms,void (*callBack)(void*),void* param){
	struct TimeLaterNode* node = (struct TimeLaterNode*)tl_malloc(sizeof(struct TimeLaterNode));
	node->delay = ms;
	node->cur = 0;
	node->callBack = callBack;
	node->param = param;
	node->old = 0;
	if(!ex_getIns()->timelaterList){
		ex_getIns()->timelaterList = LStack_create();
	}
	LStack_push((void*)ex_getIns()->timelaterList,(void*)node);
	return (void*)node;
}

void timelater_remove(void* timer){
	if(ex_getIns()->timelaterList){
		if(!LStack_delNode(ex_getIns()->timelaterList,(int)timer)){
			//移除失败
			 assert(0);
		}
	}
	tl_free(timer);
}

static void 
callBack(int data,int param){
	struct TimeLaterNode* node =(struct TimeLaterNode*)data;
	node->cur+=(int)g_delayTime;
	if(node->old+g_delayTime>node->delay){
		//printf("old=%d\n",node->old);
		node->old = 0;
		if (node->callBack!=0){
			node->callBack(node->param);
		}
		ex_lua_evt_dispatch_f(node,EVENT_TIMER,node->cur);
	}else{
		node->old+=g_delayTime;
	}
}

void 
timelater_run(){
	if(ex_getIns()->timelaterList)
		LStack_ergodic_t(ex_getIns()->timelaterList,callBack,0);
}