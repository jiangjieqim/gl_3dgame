#ifndef _SPRITE_HIT_H_
#define _SPRITE_HIT_H_
#include "vmath.h"

//SpriteHit为逻辑点击模块,和渲染设备无关
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

	/*
		以Sprire相对的0,0坐标设置的拖拽范围,如果设置的是0,没有范围限制
	*/
	int	dragX,dragY,dragWidth,dragHeight,oldx,oldy;
		//鼠标相对于面板的点击局部坐标
	int mouseLocalX,mouseLocalY;
};

void spritehit_dispose(struct SpriteHit* phit);

//设置可点击区域
void spritehit_hitTriangle(struct SpriteHit* phit,float screenX,float screenY);

//设置碰撞的数据
void spritehit_setData(struct SpriteHit* pSpr,
					   int	dragX,int dragY,int dragWidth,int dragHeight,int oldx,int oldy);

/*
	是否设置了拖拽范围区间
*/
int
spritehit_haveDragScope(struct SpriteHit* ptr);


void spritehit_changeDragXY(struct SpriteHit* pHit,
							int w,int h,
							int ppx,int ppy,
							int screenX,int screenY,
							int* px,int* py,float* pProgress);
#endif