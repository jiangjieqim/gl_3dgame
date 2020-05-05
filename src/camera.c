#include <string.h>
#include "tl_malloc.h"

#include "tlgl.h"
#include "camera.h"
#include "ex.h"
#include "vmath.h"

struct Camera{
	float x,y,z,rx,ry,rz;	//坐标,欧拉角
	Matrix44f perctive;		//齐次空间矩阵
	Matrix44f model;		//模型视图矩阵
	float sw,sh;			//当前cam所在帧缓冲区的宽高
	int _2dx,_2dy;			//2d cam相对于主帧缓冲区的位置,这两个变量不作为cam的计算属性,只是在Sprite做拾取的时候做偏移计算
};

void cam_set_2dxy(void* p,int x,int y){
	struct Camera* c = (struct Camera*)p;
	c->_2dx = x;
	c->_2dy = y;
}
void cam_get_2dxy(void* p,int* px,int* py){
	struct Camera* c = (struct Camera*)p;
	*px = c->_2dx;
	*py = c->_2dy;
}

void* cam_create(){
	void* p = tl_malloc(sizeof(struct Camera));
	struct Camera* c = (struct Camera*)p;
	memset(p,0,sizeof(struct Camera));
	
	//初始化模型矩阵
	mat4x4_identity(c->model);
	mat4x4_transpose(c->model);
	return p;
}

void cam_dispose(void* cam){
	tl_free(cam);
}

void cam_setPerspect(void*p,float fov, float aspectRatio,
					  float zNear, float zFar){
	struct Camera* c = (struct Camera*)p;
	mat4x4_perspective(c->perctive,fov,aspectRatio,zNear,zFar);
	mat4x4_transpose(c->perctive);	
}

void cam_setModePosition(void* p,float x,float y,float z,
						 float rx,float ry,float rz){
	struct Camera* c = (struct Camera*)p;
	tlgl_get_matrix(c->model,
		c->x,c->y,c->z,
		c->rx,c->ry,c->rz);
}

void cam_setOrtho(void* p,float sw,float sh,float zbuffer){
	struct Camera* c = (struct Camera*)p;
	c->sw = sw;
	c->sh = sh;
	mat4x4_identity(c->perctive);
	mat4x4_orthoPerspect(c->perctive,0,sw,0,sh,0,zbuffer);	
}

void cam_get_wh(void *p,float *w,float *h){
	struct Camera* c = (struct Camera*)p;
	*w = c->sw;
	*h = c->sh;
}

//获取camera的齐次坐标矩阵
void* cam_getPerctive(void* p){
	struct Camera* c = (struct Camera*)p;
	return c->perctive;
}
//获取模型视图矩阵
void* cam_getModel(void* p){
	struct Camera* c = (struct Camera*)p;
	return c->model;
}

void cam_setX(void* p,float x){
	struct Camera* c = (struct Camera*)p;
	c->x = x;
}

void cam_reset(void* p){
	struct Camera* c = (struct Camera*)p;
	c->x = 0;
	c->y = 0;
	c->z = 0;
	c->rx = 0;
	c->ry = 0;
	c->rz = 0;
}

void cam_setY(void* p,float y){
	struct Camera* c = (struct Camera*)p;
	c->y = y;
}
void cam_setZ(void* p,float z){
	struct Camera* c = (struct Camera*)p;
	c->z = z;
}
void cam_setRX(void* p,float rx){
	struct Camera* c = (struct Camera*)p;
	c->rx = rx;
}
void cam_setRY(void* p,float ry){
	struct Camera* c = (struct Camera*)p;
	c->ry = ry;
}
void cam_setRZ(void* p,float rz){
	struct Camera* c = (struct Camera*)p;
	c->rz = rz;
}
float cam_getX(void *p){
	struct Camera* c = (struct Camera*)p;
	return c->x;
}
float cam_getY(void *p){
	struct Camera* c = (struct Camera*)p;
	return c->y;
}
float cam_getZ(void *p){
	struct Camera* c = (struct Camera*)p;
	return c->z;
}
float cam_getRX(void *p){
	struct Camera* c = (struct Camera*)p;
	return c->rx;
}
float cam_getRY(void* p){
	struct Camera* c = (struct Camera*)p;
	return c->ry;
}
float cam_getRZ(void* p){
	struct Camera* c = (struct Camera*)p;
	return c->rz;
}
void cam_offsetX(void* p,float v){
	struct Camera* c = (struct Camera*)p;
	c->x+=v;
}
void cam_offsetY(void* p,float v){
	struct Camera* c = (struct Camera*)p;
	c->y+=v;
}
void cam_offsetZ(void* p,float v){
	struct Camera* c = (struct Camera*)p;
	c->z+=v;
}
//void cam_refreshPerspect(void* p){
//	struct Camera* c = (struct Camera*)p;
//}

void cam_refreshModel(void *p){
	struct Camera* c = (struct Camera*)p;
	cam_setModePosition(p,c->x,c->y,c->z,c->rx,c->ry,c->rz);
	ex_getIns()->bForceRender = 1;
}