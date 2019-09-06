#ifndef _FBO_TEX_
#define _FBO_TEX_
//初始化
void* fbo_init(int texW,int texH);

//渲染回调
void fbo_render(void* ptr);

//绑定渲染回调
void fbo_bind(void* ptr,void (*callBack)());

//销毁fbo对象
void fbo_dispose(void* p);

//获取当前的cam
void* fbo_getCam(void* p);

//获取fbo创建的texture句柄
void* fbo_getTex(void* p);

//添加一个可渲染的节点
void fbo_pushNode(void* p,void* node);

#endif