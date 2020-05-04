#include <string.h>
#include <stdio.h>

#include "tools.h"
#include "node.h"
#include "anim.h"
#include "ex.h"
#include "collide.h"
#include "tl_malloc.h"
#include "tmat.h"
#include "base.h"
//创建
struct Node* node_create(const char* name)
{
	struct Node* ptr = (struct Node*)tl_malloc(sizeof(struct Node));
	memset(ptr,0,sizeof(struct Node));

	ptr->base = base_create(TYPE_OBJ_VBO_FILE,name,0,0,0);
	//ptr->base->isNode = 1;
	//ptr->base->parent = ptr;
	base_updateMat4x4(ptr->base);

	return ptr;
}
//销毁
void node_dispose(struct Node* p)
{
	//long time = get_longTime();
	//char* name = p->base->name;

	base_dispose(p->base);
	objVBO_dispose(p->ptrVBO);
	
	if(p->anim)
	{
		anim_dispose(p->anim);
	}

	//射线拾取盒子
	//if(p->collideBox.vertex)
	//{
	//	tl_free(p->collideBox.vertex);
	//	p->collideBox.vertex=0;
	//}

	if(p->ptrCollide){
		collide_dispose(p->ptrCollide);
		p->ptrCollide = 0;
	}
	LStack_delNode(ex_getIns()->renderList,(int)p);//从渲染节点列表中移除
	tl_free(p);

	//log_color(0xff0000,"node_dispose删除%s消耗%ld毫秒\n",name,get_longTime() - time);
}

void 
node_bindAnim(struct Node* p)
{
	//////////////////////////////////////////////////////////////////////////
	//构建动作管理器
	p->anim=anim_create(p->ptrVBO->ptrList);
}

static void 
drawCollion(struct Node* p)
{
	struct HeadInfo* b = base_get(p);
	struct VertexData* vd =collide_cur(p->ptrCollide);
	tlgl_drawColorLine(*b->m,b->tmat,vd->vertex,vd->vertLen,BOX_R,0,0,b->cam);
}

/*
	根据一个着色器渲染一个批次
*/
static void 
addRenderNode(struct Node* p)
{
	anim_playByFPS(p->anim,objVBO_render,(int)p);
}

static void 
f_setShader(struct HeadInfo* base,const char* shader)
{
	//struct GMaterial* tmat = base->tmat;
	//memset(tmat->curGlslType,0,G_BUFFER_32_SIZE);
	//memcpy(tmat->curGlslType,shader,strlen(shader));
}
/*
	设置默认的材质
*/
static void 
setDefaultShader(struct Node* ptr)
{
	//struct HeadInfo* base =	base_get(ptr);
	//char _shaderName[G_BUFFER_32_SIZE];
	//tmat_getShader(base->tmat,_shaderName,G_BUFFER_32_SIZE);
	//f_setShader(base,_shaderName);
}

static int 
checkLineShader(struct ObjVBO* vbo)
{
	if(!vbo->normalID)
	{
		//检测是有拥有法线数据,没有法线数据就报异常,剔除多边形背面
		printf("着色器必须要有normal数据\n");
		//assert(0);
		return 0;
	}

	return 1;
}

static void 
lineVBO_CallBack(int input,struct ObjVBO* vbo)
{
	if(!checkLineShader(vbo))
	{
		return;
	}
	if(input)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}
/*
 *统一的渲染调用接口
 */
void
node_render(struct Node* p){
	struct HeadInfo* base =	base_get(p);

	/*if(!getv(&(base->flags),FLAGS_VISIBLE))
		return;*/

	//////////////////////////////////////////////////////////////////////////
	//objVBO_render作为节点渲染代理接口
	//有动作的播放动作,无动作的,渲染静态模型
	setDefaultShader(p);
	p->renderCallBack = NULL;

	addRenderNode(p);

	if(getv(&(base->flags),FLAGS_GLSL_OUTLINE))
	{
		//轮廓边缘渲染
		f_setShader(base,"linevbo");
		p->renderCallBack = lineVBO_CallBack;
		addRenderNode(p);
	}

	//////////////////////////////////////////////////////////////////////////
	//绘制碰撞盒
	if(getv(&(base->flags),FLAGS_RAY) && getv(&(base->flags),FLAGS_DRAW_RAY_COLLISION))
	{
		if(p->ptrCollide){
			drawCollion(p);
		}else{
			printf("ptrCollide = null!\n");
		}
	}
}

//void 
//node_setPos(struct Node* p,float x,float y,float z)
//{
//	base_setPos(p->base,x,y,z);
//}