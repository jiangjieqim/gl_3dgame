#include "common.h"
#include "tl_malloc.h"

#include "tmat.h"
#include "base.h"
#include "sprite.h"
#include "fbospr.h"

void*
fbospr_create(void* mirrorTexture,int texW,int texH){
	struct Sprite* spr = 0;
	void* mat= tmat_create_empty("fbotex");

	char buffer[64];
	tl_newName(buffer,64);
	//tmat_setTexFromGPU(mat,0,mirrorTexture);
	tmat_setID(mat,1);

	tmat_pushTex(mat,(GLuint)mirrorTexture);//mirrorTexture
	//printf("mirrorTexture:%d\n",mirrorTexture);
	//fbo->mat = mat;

	spr = sprite_create(buffer,0,0,texW,texH,0);
	sprite_rotateZ(spr,-PI/2);//spriteĞı×ª90¶È
	sprite_rotateX(spr,PI);
	base_setv(spr,FLAGS_REVERSE_FACE);

	spr->material = mat;
	return spr;
}

