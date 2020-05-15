#ifndef _SPRITE_HIT_H_
#define _SPRITE_HIT_H_
#include "vmath.h"

//SpriteHitΪ�߼����ģ��,����Ⱦ�豸�޹�
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

	/*
		��Sprire��Ե�0,0�������õ���ק��Χ,������õ���0,û�з�Χ����
	*/
	int	dragX,dragY,dragWidth,dragHeight,oldx,oldy;
		//�����������ĵ���ֲ�����
	int mouseLocalX,mouseLocalY;
};

void spritehit_dispose(struct SpriteHit* phit);

//���ÿɵ������
void spritehit_hitTriangle(struct SpriteHit* phit,float screenX,float screenY);

//������ײ������
void spritehit_setData(struct SpriteHit* pSpr,
					   int	dragX,int dragY,int dragWidth,int dragHeight,int oldx,int oldy);

/*
	�Ƿ���������ק��Χ����
*/
int
spritehit_haveDragScope(struct SpriteHit* ptr);


void spritehit_changeDragXY(struct SpriteHit* pHit,
							int w,int h,
							int ppx,int ppy,
							int screenX,int screenY,
							int* px,int* py,float* pProgress);
#endif