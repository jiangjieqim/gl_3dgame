#ifndef _FTEXT_H_
#define _FTEXT_H_

/*
 *创建一个文本
 */
void*
ftext_create(char* txtName,int fw,int fh);
/************************************************************************/
/* 在指定坐标设置一个字符数据									        */
/************************************************************************/
void 
ftext_set(void* p,char* s,int x,int y,int* pw,int* ph);

/*
 *	设置文本的屏幕坐标
 */
void 
ftext_setpos(void* p,int x,int y);
#endif