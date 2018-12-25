#ifndef _PICK_H_
#define _PICK_H_
/*
	拾取回调
	callBack(char* name)	拾取的对象名
*/
void pick_select(void* p,int hits, int buffer[],CallBackFun callBack);
#include "pick.c"
#endif