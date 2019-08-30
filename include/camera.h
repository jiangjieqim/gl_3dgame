#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vmath.h"

void* cam_create();

void cam_dispose(void* cam);

//获取camera的齐次坐标矩阵
void* cam_getPerctive(void* cam);
//获取模型视图矩阵
void* cam_getModel(void* cam);

//void cam_setX(void* cam,float x);
//void cam_setY(void* cam,float y);
//void cam_setZ(void* cam,float z);

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