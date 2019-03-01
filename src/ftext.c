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
#include "jgl.h"
#include "ftfont.h"
typedef struct FText
{
	struct Sprite* spr;	
}FText;

void*
ftext_create(){
	//struct Sprite* sp = sprite_create("text",0,0,32,32,0);
	struct Sprite* spr;
	FText* txt = (FText*)tl_malloc(sizeof(FText));
	memset(txt,0,sizeof(FText));
	
	txt->spr = sprite_create("txt0",10,20,65,65,0);

	spr = txt->spr;

	sprite_rotateZ(spr,-PI/2);

	spr->material = tmat_create_rgba("font",64,64);
	{
		GMaterial* mat = spr->material;
		int _size = 16;
		int iWidth,iHeight;
		unsigned char* rgba = (unsigned char*)tl_malloc(_size*_size*4);
		//*eFormat = GL_BGRA;
		//*iComponents = GL_RGBA;
	
		ft_load(rgba,_size,_size,&iWidth,&iHeight,"f");
		printf("%d %d\n",iWidth,iHeight);
		jsl_sub(tmat_getTextureByIndex(mat,0),rgba,GL_BGRA,GL_UNSIGNED_BYTE,0,0,iWidth,iHeight);
		tl_free((void*)rgba);
	}
	return 0;
}

void
ftext_dispose(void* p){
	FText* txt = (FText*)p;
	sprite_dipose(txt->spr);
	
	tl_free(p);
}