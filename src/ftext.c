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

/*
 *字体渲染识别不到alpha通道!
 */



/*
*[0]: width: 6  height: 12
[1]: width: 7  height: 13
[2]: width: 7  height: 14
[3]: width: 8  height: 15
[4]: width: 8  height: 16
[5]: width: 9  height: 17
 *
 **/
typedef struct FText
{
	struct Sprite* spr;	
	int size;
	int fw,fh;

	//字体预制缓冲区,用于存储临时的像素字节流数据
	void* _buffer;
	//缓冲区大小
	int _bufferLength;
}FText;

void*
ftext_create(){
	int txtWidth = 129;
	int txtHeight= 129;
	//struct Sprite* sp = sprite_create("text",0,0,32,32,0);
	struct Sprite* spr;
	float n = 1;
	FText* txt = (FText*)tl_malloc(sizeof(FText));
	memset(txt,0,sizeof(FText));
	//txt->size = 20;//18
//6 12
//中文使用12,11,字母可以使用任何尺寸的字体
	txt->fw = 12*n;
	txt->fh = 11*n;
	txt->spr = sprite_create("txt0",0,0,txtWidth,txtHeight,0);
	txt->_bufferLength = txt->fw * txt->fh*4;//计算需要的缓冲区的大小
	txt->_buffer = tl_malloc(txt->_bufferLength);

	spr = txt->spr;
	//ex_setv(spr,FLAGS_SPRITE_RENDER_LINE);

	//printf("临时缓冲区的大小 %d bytes\n",txt->_bufferLength);

	sprite_rotateZ(spr,-PI/2);

	//spr->zScale = 0.5;

	spr->material = tmat_create_rgba("font1",64,64,GL_BGRA);//"font"
	{
		int cw=0;
		int w,h;
		ftext_set(txt,"f",0,0,&w,&h);
		cw+=w;
	//	ftext_set(txt,"A",cw,0,&w,&h);
		cw+=w;
	//	ftext_set(txt,"r",cw,0,&w,&h);
		
		ftext_setpos(txt,100,100);
	}
	return txt;
}

void 
ftext_setpos(void* p,int x,int y){
	FText* txt = (FText*)p;
	struct Sprite* spr = txt->spr;
	sprite_setpos(spr,x,y);
}

void 
ftext_set(void* p,char* s,int x,int y,int* pw,int* ph){
	FText* txt = (FText*)p;
	struct Sprite* spr = txt->spr;
	GMaterial* mat = spr->material;
	
	//int _size = txt->size;
	int iWidth,iHeight;
	unsigned char* rgba = txt->_buffer;//(unsigned char*)tl_malloc(_size*_size*4);
	//*eFormat = GL_BGRA;
	//*iComponents = GL_RGBA;
	memset(txt->_buffer,0,txt->_bufferLength);

	ft_load(rgba,txt->fw,txt->fh,&iWidth,&iHeight,s);
	printf("ft_load:%s:%d %d\n",s,iWidth,iHeight);
	*pw = iWidth;
	*ph = iHeight;
	jsl_sub(tmat_getTextureByIndex(mat,0),rgba,GL_BGRA,GL_UNSIGNED_BYTE,x,y,iWidth,iHeight);
	
	//tl_free((void*)rgba);
}

void
ftext_dispose(void* p){
	FText* txt = (FText*)p;
	sprite_dipose(txt->spr);
	
	tl_free(p);
}