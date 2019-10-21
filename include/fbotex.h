#ifndef _FBO_TEX_
#define _FBO_TEX_
//初始化
void* fbo_init(int texW,int texH);

//渲染回调
void fbo_render(void* ptr);

//销毁fbo对象
// deltex 1 销毁纹理对象 0 不销毁纹理对象
void fbo_dispose(void* p,int deltex);

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
//设置fbo只渲染一次,并且不销毁纹理对象
//void 
//fbo_set_once(void* ptr);
//void fbo_set_once(void* ptr,void (*onceCallBack)(void*,void*),void* parms);
#endif