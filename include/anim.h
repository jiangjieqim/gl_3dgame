#ifndef _ANIM_
#define _ANIM_

#include "animtor.h"

/*
	将VBO转化为动作列表
*/
void* anim_create(struct LStackNode* list);
/*
	销毁
*/
void anim_dispose(void* anim);

/*
	播放指定索引号的动作
*/
void anim_play(void* anim,int index,void (*callBack)(int,int),int param);

/*
	根据FPS选择播放自适应的VBO节点
*/
void anim_playByFPS(void* p,void (*callBack)(int,int),int param);

/*
	自定义设置动作的fps,该值被设置之后会采用设置的帧率计算关键帧
*/
void anim_set_fps(void* p,int fps);

/*
	获取动作管理器
*/
struct Animtor* anim_get_animtor(void* p);
/*
	获取关键帧的总数量
*/
int
anim_total(void* p);

/*
	获取当前的关键帧索引号 0开始
*/
int
anim_curFrame(void* p);
#endif