#ifndef _SPRITE_HIT_H_
#define _SPRITE_HIT_H_
#include "vmath.h"
struct SpriteHit{
	/*
		��Sprire��Ե�0,0�������õ���ק��Χ,������õ���0,û�з�Χ����
	*/
	int hitX,hitY,hitWidth,hitHeight;
	/*
		--0:�����϶�����,1�����϶�����
	*/
	int	dragDirection;

	/*
		������ʰȡ��ʱ��ʹ��
	*/
	float *hitTriangle;
};

void spritehit_dispose(struct SpriteHit* phit);

//���ÿɵ������
void spritehit_hitTriangle(struct SpriteHit* phit,struct Vec2 a1,struct Vec2 a2,struct Vec2 a3,struct Vec2 a4);
#endif