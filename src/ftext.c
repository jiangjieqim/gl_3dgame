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
 *������Ⱦʶ�𲻵�alphaͨ��!
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

	//����Ԥ�ƻ�����,���ڴ洢��ʱ�������ֽ�������
	void* _buffer;
	//��������С
	int _bufferLength;
	float n;//��������
}FText;

void*
ftext_create(char* txtName,int fw,int fh){
	int txtWidth = 64;
	int txtHeight= 64;
	//struct Sprite* sp = sprite_create("text",0,0,32,32,0);
	struct Sprite* spr;
	float n;

	FText* txt = (FText*)tl_malloc(sizeof(FText));
	memset(txt,0,sizeof(FText));
	//txt->size = 20;//18
	txt->n = 1;
	n = txt->n;
//6 12
//����ʹ��12,11,��ĸ����ʹ���κγߴ������
	txt->fw = fw*n;
	txt->fh = fh*n;
	//txt->fw = 18*n;
	//txt->fh = 18*n;
	
	txt->spr = sprite_create(txtName,0,0,txtWidth,txtHeight,0);
	txt->_bufferLength = txt->fw * txt->fh*4;//������Ҫ�Ļ������Ĵ�С
	txt->_buffer = tl_malloc(txt->_bufferLength);

	spr = txt->spr;
	
	//ex_setv(spr,FLAGS_SPRITE_RENDER_LINE);

	//printf("��ʱ�������Ĵ�С %d bytes\n",txt->_bufferLength);

	sprite_rotateZ(spr,-PI/2);
	
	sprite_set_scale_z(spr,1/n);

	//spr->material = tmat_create_rgba("font1",64,64,GL_BGRA);//"font"
	{

		int size = 64;
		spr->material = tmat_create_rgba("font1",size,size,GL_BGRA);//"font"
	}
	/*
	{
		int cw=0;
		int w,h;
		ftext_set(txt,"R",0,0,&w,&h);
		cw+=w;
	//	ftext_set(txt,"A",cw,0,&w,&h);
		cw+=w;
	//	ftext_set(txt,"r",cw,0,&w,&h);
		
		ftext_setpos(txt,100,100);
	}
	*/
	return txt;
}

void 
ftext_setpos(void* p,int x,int y){
	FText* txt = (FText*)p;
	struct Sprite* spr = txt->spr;
	float n = txt->n;
	/*x = (float)x/txt->n;
	y = (float)y/txt->n;*/
	/*if(n!=1){
		x = x - spr->mWidth*(spr->zScale);
		y = y - spr->mHeight*(spr->zScale);
	}*/
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
	//printf("ft_load:%s:%d %d\n",s,iWidth,iHeight);
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