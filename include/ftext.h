#ifndef _FTEXT_H_
#define _FTEXT_H_

/*
 *创建一个文本
 */
void*
ftext_create(char* txtName,int texw,int texh,int fw,int fh);
/************************************************************************/
/* 在指定坐标设置一个字符数据(一个汉字或者是一个英文)
/************************************************************************/
void 
ftext_set(void* p,char* s,int x,int y,int* pw,int* ph);

/*
 *	设置文本的屏幕坐标
 */
void 
ftext_setpos(void* p,int x,int y);

/*
 *	将str字符串转换成贴图数据展示出来
 */
void
ftext_parse(void* p,const char* str);
#endif