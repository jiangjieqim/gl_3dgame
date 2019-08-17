#include <stdio.h>

#include "tl_malloc.h"
#include "evt.h"
#include "ex.h"
#include "input.h"

typedef struct Input
{
	//0xcdcdcdcd
	/*事件引用*/
	//void* evtList;

	//是否在焦点中
	int force;
}Input;

//键盘事件
static void
f_key2(int evtId,void* data){
	struct E_KeyBoard* pkey = (struct E_KeyBoard*)data;

	
	printf("input key = %d\n",pkey->key);
	
	
	//switch(pkey->key){
	//}
}

void* input_create(){
	struct Input* ptr=(struct Input*)tl_malloc(sizeof(struct Input));
	//创建事件监听句柄
//	ptr->evtList = (void*)LStack_create();

	evt_on(ex_getInstance(),EVENT_ENGINE_KEYBOARD,f_key2);
	//printf("*\n");

	return ptr;
}