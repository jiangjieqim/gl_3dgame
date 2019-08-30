#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vmath.h"

void* cam_create();

void cam_dispose(void* cam);

//获取camera的齐次坐标矩阵
void* cam_getPerctive(void* cam);
//获取模型视图矩阵
void* cam_getModel(void* cam);

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


//刷新cam的模型视图矩阵,在设置完x,y,z,rx,ry,rz需更新
void cam_refreshModdel(void *p);

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