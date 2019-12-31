#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vmath.h"

void* cam_create();

void cam_dispose(void* cam);

//获取camera的齐次坐标矩阵
void* cam_getPerctive(void* cam);
//获取模型视图矩阵
void* cam_getModel(void* cam);
//获取cam对应的帧缓冲区的宽高
void cam_get_wh(void *p,float *w,float *h);

void cam_setX(void* cam,float x);
void cam_setY(void* cam,float y);
void cam_setZ(void* cam,float z);
void cam_setRX(void* cam,float rx);
void cam_setRY(void* cam,float ry);
void cam_setRZ(void* cam,float rz);
float cam_getX(void *p);
float cam_getY(void *p);
float cam_getZ(void *p);
float cam_getRX(void *p);
float cam_getRY(void* p);
float cam_getRZ(void* p);
void cam_offsetX(void* p,float v);
void cam_offsetY(void* p,float v);
void cam_offsetZ(void* p,float v);

//用于获取当前该cam矩阵空间渲染的区域,相对于主帧缓冲区基于左上角的x,y偏移
void cam_set_2dxy(void* p,int x,int y);
void cam_get_2dxy(void* p,int* px,int* py);

//刷新cam的模型视图矩阵,在设置完x,y,z,rx,ry,rz需更新
void cam_refreshModel(void *p);

//重置camera的坐标和旋转
void cam_reset(void* p);

//void cam_refreshPerspect(void* cam);
/************************************************************************/
/* 构造齐次矩阵                                                          
/************************************************************************/
void cam_setPerspect(void*p,float fov, float aspectRatio,
					  float zNear, float zFar);

/************************************************************************/
/* 设置模型视图的坐标                                                                     */
/************************************************************************/
void cam_setModePosition(void* p,float x,float y,float z,
						 float rx,float ry,float rz);

/************************************************************************/
/* 设置正交矩阵                                                                     */
/************************************************************************/
void cam_setOrtho(void* p,float sw,float sh,float zbuffer);

#endif