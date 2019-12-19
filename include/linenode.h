#ifndef _LINE_NODE_H_
#define _LINE_NODE_H_
/*
vertcnt 顶点数量
int point 1:是点模式 0:是线模式
组件创建的时候需要在其他资源complete完成之后才是安全的
*/
void* linenode_create(const char* name,int vertcnt,void* cam,int point);

void linenode_push(void* p,float x,float y,float z);
void linenode_end(void* p);
void linenode_render(void* p);

//销毁
void linenode_dispose(void* p);

//为linenode设置线段的颜色
void linenode_set_color(void* p,float r,float g,float b);
#endif