#ifndef _OBJ_VBO_
#define _OBJ_VBO_
#include <gl/glew.h>
#include <gl/glut.h>

#include "tools.h"
#include "anim.h"

/*
 *VBO结构体
 */
struct ObjVBO
{
	GLuint vertexID;
	GLuint uvID;
	GLuint normalID;
	GLuint indexID;
	int renderVertCount;//需要渲染的顶点个数
#ifdef CALCULATE_VBO_SIZE
	//VBO所占用的尺寸
	int byteSize;
#endif
};
/*
	使用VBO模式
*/
struct Obj_vbo_model
{
	//vbo缓存列表.当为一个节点的时候是静态模型,如果是多个节点的时候就是MeshAnimation
	struct LStackNode* ptrList;

	//VBO数据组织的方式
	int dataType;
};
/*
	销毁
*/
void objVBO_dispose(struct Obj_vbo_model* p);

/*
	VAO模式加载模型数据,添加一个VBO节点

	GLfloat* verts	uv,normal vertex数据缓冲区指针
	注意:verts数据的组织方式喝_pvboModel->dataType要保持一致
	verts结构:vertex_x,vertex_y,vertex_z,u,v,normalX,normalY,normalZ

	int	_bufferSize float的个数
*/
void objVBO_pushNode(struct Obj_vbo_model* _pvboModel,GLfloat* verts,int _bufferSize);

//获取到一个struct ObjVBO*对象
void* 
objVBO_createNode(int vertsType,GLfloat* verts,int size);
/*
	按照索引方式导入数据
*/
void objVBO_pushExportObj(struct Obj_vbo_model* _pvboModel,const char* str);

/*
	创建
*/
struct Obj_vbo_model* objVBO_create(const char* name,int type);
/*
	渲染
*/
void objVBO_render(int data,int parms);

/*
 * 渲染一个vbo节点
 * renderCallBack渲染回调
 */
void objVBO_renderNode(
						struct ObjVBO* vbo,
						struct GMaterial* tmat,
						Matrix44f m,//当前模型的矩阵
						int flag,
						void (*renderCallBack)(int,struct ObjVBO*));

#endif