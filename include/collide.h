#ifndef _COLLIDE_H_
#define _COLLIDE_H_
#include "tlgl.h"

/*
 *射线拾取碰撞盒
 **/
struct CollideBox
{
	struct LStackNode* list;
	int index;//当前的索引
};
/*
 *加载一个碰撞盒子
 *int frameCount:0的时候加载所有的关键帧作为碰撞盒子,2代表加载2个关键帧作为碰撞盒
 */
struct CollideBox* collide_load(const char* url,int frameCount);
/*
 *销毁碰撞盒
 */
void collide_dispose(struct CollideBox* ptr);
/*
 *获取索引对应的数据,index从0开始
 */
struct VertexData*  collide_getIndex(struct CollideBox* ptr,int index);
/*
 *当前的VertexData
 */
struct VertexData* collide_cur(struct CollideBox* ptr);

#endif