#ifndef _ANIMTOR_H_
#define _ANIMTOR_H_
#include "tools.h"

/*
	动画片段切割器
*/
struct Animtor
{
	struct LStackNode* list;

	//当前的动作
	char cur_anim[G_BUFFER_64_SIZE];
	int cur_start;
	int cur_end;
	//是否在播放
	int isPlaying;
};
struct Animtor* animtor_create();

void animtor_dispose(struct Animtor* p);
/*
	绑定一个动作
*/
void animtor_push(struct Animtor* p,const char* animKey,const int start,const int end);

/*
	设置当前的动作
*/
int animtor_setcur(struct Animtor* p,char* const animKey);

/*
	计算关键帧
*/
void animtor_calculateFrame(struct Animtor* p,int* const pCurIndex);

/*
	播放
*/
void animtor_play(struct Animtor* p);
/*
	暂停
*/
void animtor_pause(struct Animtor* p);

/*
	是否在播放
*/
int animtor_isPlaying(struct Animtor* p);
/*
	指定区间索引播放动画
*/
void animtor_play_start_end(struct Animtor* p,const int s,int e);
#endif