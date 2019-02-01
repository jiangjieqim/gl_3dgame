#ifndef _ANIM_
#define _ANIM_

#include "animtor.h"

/************************************************************************
*	动作管理器
*	无关乎底层是用VBO还是直接模式,只是管理动画配置,fps
/************************************************************************/
struct VBOAnim
{
	int allLength;//动作总长度
	int _tempLength;
	int curIndex;//当前动作索引

	int* nodeList;//int 数组储存struct ObjVBO*

	/*
	 * 只有>0的时候才是有动作的
	 * 每一帧的间隔时间
	 * fps = 60;
	 * fpsInterval = 1000 / fps = 16.6666;
	 */
	long fpsInterval;
	
	/*
	*	插值
	*/
	long interpolation;

	/*
	*	动作管理器
	*/
	struct Animtor* pAnimtor;
};


/*
	将VBO转化为动作列表
*/
struct VBOAnim* anim_create(struct LStackNode* list);
/*
	销毁
*/
void anim_dispose(struct VBOAnim* anim);

/*
	播放指定索引号的动作
*/
void anim_play(struct VBOAnim* anim,int index,void (*callBack)(int,int),int param);

/*
	根据FPS选择播放自适应的VBO节点
*/
void anim_playByFPS(struct VBOAnim* p,void (*callBack)(int,int),int param);

/*
	自定义设置动作的fps,该值被设置之后会采用设置的帧率计算关键帧
*/
void anim_set_fps(struct VBOAnim* p,int fps);

/*
	获取动作管理器
*/
struct Animtor* anim_get_animtor(struct VBOAnim* p);
#endif