#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#include "tools.h"
#include "tween.h"
#include "tl_malloc.h"
#include "gettime.h"
//#include "ex.h"
static struct LStackNode* g_list;


typedef struct {
	/*当前引用*/
	double* p;
	double s;//开始值
	double t;//目标值
}TNode;
typedef struct{
	void* obj;
	/*
		节点长度
	*/
	int length;

	/*
		需要的时间(毫秒)
	*/
	int needTime;
	int useTime;
	/*
		开始的时间
	*/
	int startTime;
	void (*updateCallBack)();
	void (*endCallBack)();
	TNode* ptr;
}TweenNode;
/*
	终止tween
*/
void
tween_stop(void* ptr){
	if(!tween_is_play(ptr))	{
		return;
	}
	{
		TweenNode* _node = (TweenNode*)ptr;
		if(_node->endCallBack)	_node->endCallBack(_node->obj);
		tl_free(_node->ptr);
		LStack_delNode(g_list,(int)_node);
		tl_free(_node);
	}
	
}
/*
	渐变
*/
void* 
tween_to(void* obj,
		 int time,
		 void (*end)(void*),
		 void (*update)(void*),
		 int cnt,...){
	TweenNode* _node = (TweenNode*)tl_malloc(sizeof(TweenNode));
	_node->obj = obj;
	_node->length = cnt/2;
	_node->ptr = (TNode*)tl_malloc(sizeof(TNode) * cnt/2);
	_node->updateCallBack = update;
	_node->endCallBack = end;
	_node->startTime = get_longTime();
	_node->needTime = time;
	//###############################################
	{
		int i;
		double* pv;
		int n = 0;
		int k = 0;
		//double* dptr;
		va_list ap;
		va_start(ap, cnt);
		for(i = 0; i < cnt; i++){
			if(n % 2 == 1){
				double* dptr;
				TNode* _nptr = &_node->ptr[k];
				
				dptr = (double*)pv;
				_nptr->p = pv;
				_nptr->s = *dptr;
				_nptr->t = va_arg(ap, double);
				//printf("%d\t%d\n",_nptr,sizeof(TNode));
				k++;
			}else{
				//printf("%d\t%.3f\n",p,(float)p);
				//printf("%d\n",p);
				pv = va_arg(ap, double*);
			}
			n++;
		}
		va_end(ap);
	}

	if(g_list==0){
		g_list = LStack_create();
	}
	LStack_push(g_list,(int)_node);
	return _node;
}
//每一次回调的时间间隔
static long g_delay = 0;
static long g_last= 0;
static void
f_nodeRun(TweenNode* _node,TNode* _nptr){
	double s = _nptr->s;
	double e = _nptr->t;
	//int c = 20;//_node->needTime / ex_delay_time();
	double v = _node->needTime/g_delay;
	double f = (e-s)/v;
	if(v <= 0) return;
	
	/*double* ptr = (double*)_nptr->p;
	*ptr += v;*/
	//printf("%.3f\t%.3f\n",s,(double*)_nptr->p);
	//printf("delay_time = %ld\n",ex_delay_time());
	if(f > 0 && *_nptr->p+f > e||f<0 && *_nptr->p+f < e){
		*_nptr->p = e;
	}else{
		*_nptr->p+=f;
	}
//	printf("%ld v = %.3f p = %.3f %.3f \t%ld %.3f\n",_node->useTime,v,*_nptr->p,e,g_delay,f);
}
//处理一个节点
static void
f_tween_play(TweenNode* _node,long _longTime){

	_node->useTime = _longTime - _node->startTime;

	if( _node->useTime >= _node->needTime){
		tween_stop(_node);
	}else{
		int i;
		for(i = 0;i < _node->length;i++)	
			f_nodeRun(_node,(TNode*)&_node->ptr[i]);

		if( _node->updateCallBack)	_node->updateCallBack(_node->obj);
		//printf("%ld\n",_node->useTime);
	}
}

int 
tween_is_play(void* ptr){
	struct LStackNode* s = (struct LStackNode* )g_list;
	void* top,*p;
	if(!g_list)	return 0;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		if((int)ptr == data)	return 1;
	}
	return 0;
}
void
tween_run(long _longTime){
	struct LStackNode* s = (struct LStackNode* )g_list;
	void* top,*p;

	if(!g_list)	return;

	g_delay = _longTime - g_last;
	
	if(g_delay < 1 )	return;

	g_last = _longTime;
	
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		//callBack(data,parm);
		f_tween_play((TweenNode*)data,_longTime);
	}
}