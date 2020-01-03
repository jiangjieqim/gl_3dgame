#include <memory.h>
#include <assert.h>
#include "linenode.h"
#include "tlgl.h"
#include "tl_malloc.h"
#include "base.h"
#include "ex.h"
#include "obj_vbo.h"
#include "tmat.h"


/*
 * 点绘制的时候的点的尺寸
 */
#define POINT_SIZE 2

struct LineNode{
	struct HeadInfo* base;
	struct ObjVBO* vbo;
	GLfloat* verts;
	unsigned short index;//GLfloat的数量
};
void* linenode_create(const char* name,int vertcnt,void* cam,int point){
	struct GMaterial* tmat;
	struct LineNode* line = (struct LineNode*)tl_malloc(sizeof(struct LineNode));
	memset(line,0,sizeof(struct LineNode));
	line->verts = (GLfloat*)tl_malloc(vertcnt*sizeof(struct Vec3));
	line->base = base_create(TYPE_LINENODE,name,0,0,0);
	//setv(&line->base->flags,FLAGS_DISABLE_CULL_FACE);
	base_set_cam(line,cam);
	tmat = tmat_create_empty("linenode");

	if(point){
		tmat->rendermode = GL_POINT;
		glPointSize(POINT_SIZE);
	}else{
		tmat->rendermode = GL_LINE_STRIP;
	}
	line->base->tmat = tmat;
	base_set_visible(line->base,1);
	return line;
}

void linenode_set_color(void* p,float r,float g,float b){
	struct LineNode* line = (struct LineNode*)p;
	struct GMaterial* tmat = line->base->tmat;
	struct Vec4* color = &tmat->_lineColor;
	color->x = r;
	color->y = g;
	color->z = b;
	color->w = 1;
}

void linenode_push(void* p,float x,float y,float z){
	struct LineNode* line = (struct LineNode*)p;
	struct Vec3 pos;
	pos.x = x;
	pos.y = y;
	pos.z = z;
	memcpy(line->verts+line->index,&pos,sizeof(struct Vec3));
	line->index+=3;
}

void linenode_mod(void* p,int index,float x,float y,float z){
	struct LineNode* line = (struct LineNode*)p;
	struct Vec3 pos;
	pos.x = x;
	pos.y = y;
	pos.z = z;
	if(index>line->vbo->renderVertCount-1)
		assert(0);//指定的 index 索引溢出了
	else
		vbo_sub_data(line->vbo->vertexID,index*3*sizeof(GLfloat),(GLfloat*)&pos,sizeof(struct Vec3));
}

void f_freeVert(struct LineNode* line){
	if(line->verts){
		tl_free(line->verts);
		line->verts = 0;
	}
}
void linenode_end(void* p){
	struct LineNode* line = (struct LineNode*)p;
	line->vbo = objVBO_createNode(OBJ_VERTEX,line->verts,line->index*sizeof(GLfloat));
	f_freeVert(line);
}
void linenode_render(void* p){
	struct LineNode* line = (struct LineNode*)p;
	struct HeadInfo* base = line->base;
	if(line->vbo){
		objVBO_renderNode(line->vbo,base->tmat,base->m,base->flags,0);
	}
}
void linenode_dispose(void* p){
	struct LineNode* line = (struct LineNode*)p;
	vbo_dispose(line->vbo);
	base_dispose(line->base);
	f_freeVert(line);
	LStack_delNode(ex_getIns()->renderList,(int)line);
	tl_free(line);
}