#ifndef _RENDER_NODE_H_
#define _RENDER_NODE_H_

#include "tlgl.h"
#include "obj_vbo.h"
#include "anim.h"
#include "collide.h"


struct Node
{
	struct HeadInfo* base;
	struct Obj_vbo_model* ptrVBO;
	/*
	*	动作管理器
	*/
	void* anim;	
	struct CollideBox* ptrCollide;//碰撞盒引用
	void (*renderCallBack)(int,struct ObjVBO*);//渲染回调
};

struct Node* node_create(const char* name);
/*
 *销毁node
 */
void node_dispose(struct Node* p);

/*
	没有动作的就不调用该接口,动作管理器初始化
*/
void node_bindAnim(struct Node* p);
/*
	渲染
*/
void node_render(struct Node* p);
///*
//	设置坐标
//*/
//void node_setPos(struct Node* p,float x,float y,float z);

#endif
