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
	int size;
}FText;

void*
ftext_create(){
	//struct Sprite* sp = sprite_create("text",0,0,32,32,0);
	struct Sprite* spr;
	FText* txt = (FText*)tl_malloc(sizeof(FText));
	memset(txt,0,sizeof(FText));
	txt->size = 20;//18
	txt->spr = sprite_create("txt0",10,20,65,65,0);

	spr = txt->spr;

	sprite_rotateZ(spr,-PI/2);

	//spr->zScale = 0.5;

	spr->material = tmat_create_rgba("font1",64,64);//"font"
	{
		int cw=0;
		int w,h;
		ftext_set(txt,"¹ú",0,0,&w,&h);
		cw+=w;
		ftext_set(txt,"¼Ò",cw,0,&w,&h);
		cw+=w;
		ftext_set(txt,"r",cw,0,&w,&h);
	}
	return 0;
}

void 
ftext_set(void* p,char* s,int x,int y,int* pw,int* ph){
	FText* txt = (FText*)p;
	struct Sprite* spr = txt->spr;
	GMaterial* mat = spr->material;
	
	int _size = txt->size;
	int iWidth,iHeight;
	unsigned char* rgba = (unsigned char*)tl_malloc(_size*_size*4);
	//*eFormat = GL_BGRA;
	//*iComponents = GL_RGBA;

	ft_load(rgba,_size,_size,&iWidth,&iHeight,s);
	printf("%s:%d %d\n",s,iWidth,iHeight);
	*pw = iWidth;
	*ph = iHeight;
	jsl_sub(tmat_getTextureByIndex(mat,0),rgba,GL_BGRA,GL_UNSIGNED_BYTE,x,y,iWidth,iHeight);
	tl_free((void*)rgba);
}

void
ftext_dispose(void* p){
	FText* txt = (FText*)p;
	sprite_dipose(txt->spr);
	
	tl_free(p);
}