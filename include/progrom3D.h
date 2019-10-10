#ifndef _Progrom3D_H_
#define _Progrom3D_H_

//构造着色器管理句柄
void*
progrom3d_create();

//获取一个着色器对象
void*
progrom3d_get(void* ptr,const char* shadername);

#endif