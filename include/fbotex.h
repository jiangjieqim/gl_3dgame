#ifndef _FBO_TEX_
#define _FBO_TEX_
//初始化
void* fbo_init(int texW,int texH);

//渲染回调
void fbo_render(void* ptr);

//绑定渲染回调
//void fbo_bind(void* ptr,void (*callBack)());

//销毁fbo对象
void fbo_dispose(void* p);

//获取当前的3d cam
void* fbo_get3dCam(void* p);
//获取2d cam
void* fbo_get2dCam(void* p);

//获取fbo创建的texture句柄
void* fbo_getTex(void* p);

//当fbo所在的帧缓冲区发生该变的时候调用
void 
fbo_resize(void*p);


/*
 *	激活或者关闭当前的FBO,在FBO关闭状态下的时候,FBO绑定的渲染节点是不渲染的
 *	0 关闭 1 开启
 */
void
fbo_enable(void*p,int v);

#endif