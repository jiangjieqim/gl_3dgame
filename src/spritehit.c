#include "tl_malloc.h"

#include "spritehit.h"

void spritehit_dispose(struct SpriteHit* phit){
	tl_free(phit);
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