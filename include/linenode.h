#ifndef _LINE_NODE_H_
#define _LINE_NODE_H_
//vertcnt ¶¥µãÊýÁ¿
void* linenode_create(const char* name,int vertcnt,void* cam);
void linenode_push(void* p,float x,float y,float z);
void linenode_end(void* p);
void linenode_render(void* p);
void linenode_dispose(void* p);
#endif