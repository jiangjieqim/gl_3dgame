#ifndef _FBO_TEX_
#define _FBO_TEX_

void* fbo_init(int texW,int texH);

void fbo_render(void* ptr);

//绑定渲染回调
void fbo_bind(void* ptr,void (*callBack)());

//销毁fbo对象
void fbo_dispose(void* p);

void* fbo_get_spr(void* p);
void* fbo_get_cam(void* p);

#endif