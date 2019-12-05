#include <memory.h>
#include "tools.h"
#include "tl_malloc.h"
#include "tlgl.h"
#include "line.h"
#include "vmath.h"

struct Line{
	struct LStackNode *vectlist;
};

void* line_create(){
	struct Line* p = (struct Line*)tl_malloc(sizeof(struct Line));
	memset(p,0,sizeof(struct Line));
	p->vectlist = LStack_create();
	return p;
}
static int f_delNode(int data,int parm){
	//delXmlNode((struct XmlNode*)data);
	tl_free((void*)data);
	return 1;
}
void line_push(void* ptr,float x,float y,float z){
	struct Line* p = (struct Line*)ptr;
	struct Vec3 *pos = (struct Vec3*)tl_malloc(sizeof(struct Vec3));
	pos->x = x;
	pos->y = y;
	pos->z = z;
	LStack_push(p->vectlist,(void*)pos);
}

void line_dispose(void* ptr){
	struct Line* p = (struct Line*)ptr;

	LStack_ergodic(p->vectlist,f_delNode,0);

	LStack_delete(p->vectlist);
}
static int f_line_render(int data,int parm){
	struct Vec3 *pos = (struct Vec3*)data;
	glVertex3f(pos->x,pos->y,pos->z);
	return 1;
}
void line_render(void* ptr){
	struct Line* p = (struct Line*)ptr;
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	LStack_ergodic(p->vectlist,f_line_render,(int)ptr);
	glEnd();
}