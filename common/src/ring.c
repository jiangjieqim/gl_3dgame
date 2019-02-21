#include <memory.h>
#include "tl_malloc.h"
#include "tools.h"
#include "ring.h"

typedef struct 
{
	void* list;
	int index;
}Ring;

//创建环
void* ring_create(){
	Ring* r = (Ring*)tl_malloc(sizeof(Ring));
	memset(r,0,sizeof(Ring));
	r->list = LStack_create();
	return (void*)r;
}
void* 
ring_cur(void* ptr){
	Ring * r = (Ring*)ptr;
	int out;
	LStatk_getNodeByIndex(r->list,r->index,&out);
	return (void*)out;
}

//压入数据
void ring_push(void* ptr,void* node){
	Ring * r = (Ring*)ptr;
	LStack_push(r->list,node);
}
//下一个数据
void* ring_next(void* ptr){
	Ring * r = (Ring*)ptr;
	int out;

	if(r->index+1<LStack_length(r->list)){
		r->index++;
	}
	else{
		r->index = 0;
	}
	LStatk_getNodeByIndex(r->list,r->index,&out);
	return (void*)out;
}
//前一个数据
void* ring_pre(void* ptr){
	Ring * r = (Ring*)ptr;
	int out;

	if(r->index-1 >= 0){
		r->index--;
	}
	else{
		r->index = LStack_length(r->list)-1;
	}
	LStatk_getNodeByIndex(r->list,r->index,&out);
	return (void*)out;

}

void ring_dispose(void* ptr){
	Ring * r = (Ring*)ptr;

	LStack_delete(r->list);
	r->list = 0;
}