#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "tools.h"
#include "text.h"
#include "ex.h"
#include "tl_malloc.h"
#include "base.h"
static void 
f_address2str(int v,char* buffer,int bufferSize){
	memset(buffer,0,bufferSize);
	sprintf_s(buffer,bufferSize,"0x%0x",v);
}
struct TextField* 
tf_create(int bufferSize,int x,int y,
		  float r,float g,float b)
{
	char buffer[G_BUFFER_64_SIZE];
	struct TextField* tf = tl_malloc(sizeof(struct TextField));
	
	struct HeadInfo* base;

	memset(tf,0,sizeof(struct TextField));
	
	//设置文本高度
	tf->height = 10;//GLUT_BITMAP_TIMES_ROMAN_10
	
	f_address2str((int)tf,buffer,G_BUFFER_64_SIZE);

	tf->base = base_create(TYPE_TEXT_FILE,buffer,0,0,0);
	base = (struct HeadInfo*)tf->base;
	base->parent = tf;
	setv(&(base->flags),FLAGS_VISIBLE);

	//构建文本缓冲区
	tf->textPtr = tl_malloc(bufferSize);
	memset(tf->textPtr,0,bufferSize);
	tf->bufferSize = bufferSize;
	
	tf->x = x;
	tf->y = y;
	tf->r = r;
	tf->g = g;
	tf->b = b;
	ex_add(tf);
	//printf("tf_create:%s ,%d,%d\n",text,x,y);
	return tf;
}

void tf_setPos(struct TextField* tf,int x,int y){
	tf->x = x;
	tf->y = y;
}

/*
	获取文本的总长度
*/
int 
tf_getWidth(struct TextField* tf)
{
	return tf->textWidth;
}

//void 
//tf_getStr(struct TextField* tf,char* const buffer,const int bufferSize)
//{
//	memset(buffer,0,bufferSize);
//	memcpy(buffer,tf->textPtr,strlen(tf->textPtr));
//}

void 
tf_copy(struct TextField *tf_source,struct TextField *tf_dest)
{
	int len = strlen(tf_dest->textPtr);
	memset(tf_source->textPtr,0,tf_source->bufferSize);
	memcpy(tf_source->textPtr,tf_dest->textPtr,len);
}

void tf_setText(struct TextField* tf,const char* text)
{
	if((int)strlen(text)>=tf->bufferSize)
	{
		printf("设置的文本长度超过缓冲区!\n");
		assert(0);
	}
	memset(tf->textPtr,0,tf->bufferSize);
	memcpy(tf->textPtr,text,strlen(text));
}

void 
tf_dispose(struct TextField* tf){
	base_dispose(tf->base);
	tl_free(tf->textPtr);
    LStack_delNode(ex_getIns()->renderList,(int)tf);
	tl_free(tf);
}

void tf_render(int data)
{
	struct HeadInfo* base = base_get((void*)data);
	if(getv(&base->flags,FLAGS_VISIBLE) && base->curType == TYPE_TEXT_FILE)
	{
		struct EX* e = ex_getIns();
		struct TextField* tf = (struct TextField*)data;
		
		int strLength = (int)strlen(tf->textPtr);

		tf->textWidth = 0;
		if(strLength)
		{
			int i,x,y;

			if(strLength){
				x = tf->x;
				y =  e->_screenHeight-tf->y;
				glUseProgram(0);
				glColor3f(tf->r,tf->g,tf->b);
				glRasterPos2i(x,y-tf->height);
				for ( i = 0; i < strLength; i++){
					int character = *(tf->textPtr+i);
					tf->textWidth += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12,character);
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12	, character);//GLUT_BITMAP_TIMES_ROMAN_10,GLUT_BITMAP_8_BY_13
				}
				glPopAttrib();
			}
			else
			{
				//文本长度为0的时候 就不进入渲染序列!
			}
		}
		
	}
}
//==============================================================