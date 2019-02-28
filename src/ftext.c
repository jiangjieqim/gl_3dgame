#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "tools.h"
#include "tl_malloc.h"
#include "ex.h"
#include "tmat.h"
#include "obj_vbo.h"
#include "ftext.h"
#include "sprite.h"
typedef struct FText
{
	struct Sprite* spr;	
}FText;

void*
ftext_create(){
	//struct Sprite* sp = sprite_create("text",0,0,32,32,0);

	FText* txt = (FText*)tl_malloc(sizeof(FText));
	memset(txt,0,sizeof(FText));
	
	txt->spr = sprite_create("txt0",0,0,128,64,0);
	

	{

		struct Sprite* spr = txt->spr;
		spr->material = tmat_createTex("font",128,64);
		
	}

	return 0;
}

void
ftext_dispose(void* p){
	FText* txt = (FText*)p;
	sprite_dipose(txt->spr);

	tl_free(p);
}