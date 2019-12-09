#ifndef _LINE_NODE_H_
#define _LINE_NODE_H_

//vertcnt 顶点数量
void* linenode_create(const char* name,int vertcnt,void* cam);

void linenode_push(void* p,float x,float y,float z);
void linenode_end(void* p);
void linenode_render(void* p);

//销毁
void linenode_dispose(void* p);

//为linenode设置线段的颜色
void linenode_set_color(void* p,float r,float g,float b);
#endif