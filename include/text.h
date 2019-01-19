#ifndef _TEXT_H_
#define _TEXT_H_

struct TextField
{
	void* base;

	//文本内容
	char* textPtr;

	//文本屏幕坐标
	int x,y;

	//文本颜色
	float r,g,b;

	//文本的高度
	int height;
	
	//缓冲区大小
	int bufferSize;
	
	//文本总长度
	int textWidth;
};

struct TextField* tf_create(int bufferSize,int x,int y,float r,float g,float b);
void tf_dispose(struct TextField* tf);
void tf_setPos(struct TextField* tf,int x,int y);
void tf_setText(struct TextField* tf,const char* str);
/*
 *	获取当前文本的宽度
 */
int tf_getWidth(struct TextField* tf);
void tf_copy(struct TextField *tf_source,struct TextField *tf_dest);
void tf_render(int data);
/*
	获取文本
*/
//void tf_getStr(struct TextField* tf,char* const buffer,const int bufferSize);
#endif