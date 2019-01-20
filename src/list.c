#include "tools.h"
#include "tl_malloc.h"
#include "list.h"
//====================================================================================================
/*
*	可变长列表实现
*/
void list_create(struct List* _l){
	_l->length = 0;
	_l->list = 0;
}
void list_add(struct List* _l,float value){
	int i;
	float* newList = (float*)tl_malloc((_l->length+1)*sizeof(float));
	for(i = 0;i <_l->length;i++)
	{
		newList[i] = _l->list[i];
	}
	newList[i] = value;
	_l->length++;
	if(_l->list!=0)
	{
		tl_free(_l->list);
	}
	_l->list = newList;
}
void list_dispose(struct List* _l){
	_l->length = 0;
	tl_free(_l->list);
}
//====================================================================================================