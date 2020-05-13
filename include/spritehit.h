#ifndef _SPRITE_HIT_H_
#define _SPRITE_HIT_H_
#include "vmath.h"
struct SpriteHit{
	/*
		以Sprire相对的0,0坐标设置的拖拽范围,如果设置的是0,没有范围限制
	*/
	int hitX,hitY,hitWidth,hitHeight;
	/*
		--0:横向拖动滑块,1纵向拖动滑块
	*/
	int	dragDirection;

	/*
		做射线拾取的时候使用
	*/
	float *hitTriangle;
};

void spritehit_dispose(struct SpriteHit* phit);

//设置可点击区域
void spritehit_hitTriangle(struct SpriteHit* phit,struct Vec2 a1,struct Vec2 a2,struct Vec2 a3,struct Vec2 a4);
#endif