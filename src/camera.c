#include <string.h>
#include "tl_malloc.h"

#include "tlgl.h"
#include "camera.h"

#include "vmath.h"

struct Camera{
	float x,y,z,rx,ry,rz;
	Matrix44f perctive;
	Matrix44f model;
};

void* cam_create(){
	void* p = tl_malloc(sizeof(struct Camera));
	struct Camera* c = (struct Camera*)p;
	memset(p,0,sizeof(struct Camera));
	
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
	mat4x4_identity(c->perctive);
	mat4x4_orthoPerspect(c->perctive,0,sw,0,sh,0,-zbuffer);	
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

void cam_refreshModdel(void *p){
	struct Camera* c = (struct Camera*)p;
	cam_setModePosition(p,c->x,c->y,c->z,c->rx,c->ry,c->rz);
}