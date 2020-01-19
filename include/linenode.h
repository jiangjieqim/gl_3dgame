#ifndef _LINE_NODE_H_
#define _LINE_NODE_H_
/*
vertcnt 顶点数量
int point 1:是点模式 0:是线模式
组件创建的时候需要在其他资源complete完成之后才是安全的
*/
void* linenode_create(const char* name,int vertcnt,void* cam,int point);
/*
	为线段增加一个点
*/
void linenode_push(void* p,float x,float y,float z);
/*
	linenode_push 压入顶点坐标结束的时候调用
*/
void linenode_end(void* p);

/*
	渲染
*/
void linenode_render(void* p);

/*
	销毁
*/
void linenode_dispose(void* p);

/*
	为linenode设置线段的颜色
*/
void linenode_set_color(void* p,float r,float g,float b);

/*
	修改顶点的索引上的顶点坐标数据,索引从0开始计算
*/
void linenode_mod(void* p,int index,float x,float y,float z);
#endif  