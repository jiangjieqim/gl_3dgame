#include "tl_malloc.h"

#include "spritehit.h"

void spritehit_dispose(struct SpriteHit* phit){
	tl_free(phit);
}

void spritehit_setData(struct SpriteHit* pSpr,
					   int	dragX,int dragY,int dragWidth,int dragHeight,
					   int oldx,int oldy){
	pSpr->oldx = oldx;
	pSpr->oldy = oldy;
	pSpr->dragX = dragX;
	pSpr->dragY = dragY;
	pSpr->dragWidth = dragWidth;
	pSpr->dragHeight = dragHeight;
}

void spritehit_hitTriangle(struct SpriteHit* spr,struct Vec2 a1,struct Vec2 a2,struct Vec2 a3,struct Vec2 a4){
	//a1
	spr->hitTriangle[0] = a1.x ;
	spr->hitTriangle[1] = a1.y ;
	spr->hitTriangle[2] = 0;

	//a2
	spr->hitTriangle[3] = a2.x;
	spr->hitTriangle[4] = a2.y;
	spr->hitTriangle[5] = 0;

	//a3
	spr->hitTriangle[6] = a3.x;
	spr->hitTriangle[7] = a3.y;
	spr->hitTriangle[8] = 0;

	////===================================================
	
	//a1
	spr->hitTriangle[9] =  a1.x;
	spr->hitTriangle[10] = a1.y;
	spr->hitTriangle[11] = 0;

	//a4
	spr->hitTriangle[12] = a4.x;
	spr->hitTriangle[13] = a4.y;
	spr->hitTriangle[14] = 0;

	//a3
	spr->hitTriangle[15] =  a3.x;
	spr->hitTriangle[16] =  a3.y;
	spr->hitTriangle[17] = 0;
}



/*
	是否设置了拖拽范围区间
*/
int
spritehit_haveDragScope(struct SpriteHit* ptr){
	//if(ptr->m_fPosX+ptr->dragWidth - ptr->m_fWidth)
	if(ptr->dragWidth || ptr->dragHeight){
		return 1;
	}
	return 0;
}

void spritehit_changeDragXY(struct SpriteHit* pHit,
							int w,int h,
							int ppx,int ppy,
							int screenX,int screenY,
							int* px,int* py,float* pProgress
							){

	int cx0,cx1,cy0,cy1,oldx,oldy;
	
	oldx = pHit->oldx;
	oldy = pHit->oldy;

	cx0 = pHit->dragX+oldx + ppx;
	cx1 = oldx+pHit->dragWidth - w + ppx;

	cy0 = pHit->dragY+oldy + ppy; 
	cy1 = oldy+pHit->dragHeight- h + ppy;

	if(*px <= cx0)
		*px = cx0;
	else if(*px>=cx1) 
		*px = cx1;

	if(*py <= cy0) 
		*py = cy0;
	else if(*py>=cy1)
		*py = cy1;

	//是否是纵向
	if(pHit->dragDirection)
		*pProgress = (float)(screenY-pHit->oldy-ppy)/(pHit->dragHeight - w);//纵向比率
	else
		*pProgress = (float)(screenX-pHit->oldx-ppx)/(pHit->dragWidth - h);//横向比率
}