#include <memory.h>
#include <stdarg.h>
#include <stdio.h>

#include "tools.h"
#include "tween.h"
#include "tl_malloc.h"
#include "gettime.h"

static struct LStackNode* g_list;


typedef struct {
	/*当前引用*/
	int p;
	double t;//目标值
	double cur;//当前值

}TNode;
typedef struct{
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
	TNode* ptr;
}TweenNode;
/*
	终止tween
*/
static void
f_tween_stop(void* ptr){
	TweenNode* _node = (TweenNode*)ptr;
	tl_free(_node->ptr);
	//tl_free(_node);
}
/*
	渐变
*/
void* 
tween_to(int time,int cnt,...){
	TweenNode* _node = (TweenNode*)tl_malloc(sizeof(TweenNode));
	_node->length = cnt/2;
	_node->ptr = (TNode*)tl_malloc(sizeof(TNode) * cnt/2);
	_node->startTime = get_longTime();
	_node->needTime = time;
	//###############################################
	{
		int i;
		int pv;
		int n = 0;
		int k = 0;
		va_list ap;
		va_start(ap, cnt);
		for(i = 0; i < cnt; i++){
			if(n % 2 == 1){
				TNode* _nptr = &_node->ptr[k];
				_nptr->p = pv;
				_nptr->t = va_arg(ap, double);
				//printf("%d\t%d\n",_nptr,sizeof(TNode));
				k++;
			}else{
				//printf("%d\t%.3f\n",p,(float)p);
				//printf("%d\n",p);
				pv = va_arg(ap, int);
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
static void
f_tween_play(TweenNode* _node,long _longTime){
	_node->useTime =_longTime - _node->startTime;
	printf("%ld\n",_node->useTime);
	if( _node->useTime >= _node->needTime){
		f_tween_stop(_node);
		LStack_delNode(g_list,(int)_node);
	}
}
void
tween_run(long _longTime){
	struct LStackNode* s = (struct LStackNode* )g_list;
	void* top,*p;
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