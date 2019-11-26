#ifndef _FTEXT_H_
#define _FTEXT_H_

/*
 *创建一个文本
 *	txtName:ftext对象中的sprite对象的唯一标识
 */
void*
ftext_create(char* txtName,int texw,int texh,int fw,int fh,void* cam);
/************************************************************************/
/* 在指定坐标设置一个字符数据(一个汉字或者是一个英文)
 * outpw,outph输出字符的像素宽高
/************************************************************************/
void 
ftext_set(void* p,char* s,int x,int y,int* outpw,int* outph);

/*
 *	设置文本的屏幕坐标
 */
void 
ftext_setpos(void* p,int x,int y);

/*
 *  增量增加
 *	将str字符串转换成贴图数据展示出来,该接口会增量增加后续的文本渲染
 *	outw,outh表示当前ftext的文本渲染区域的宽高
 */
void
ftext_parse(void* p,const char* str,int *outw,int *outh);
/************************************************************************/
/* 擦除掉所有的文本字体                                                                   */
/************************************************************************/
void
ftext_clear(void* p);
/************************************************************************/
/* 显示隐藏                                                                     */
/************************************************************************/
void
ftext_vis(void* p,int vis);
//获取文本对象的文本数据
char* 
ftext_get_str(void* p);
//获取文本对象的宽高
void
ftext_get_size(void* p,int* w,int *h);
//获取当前文本的坐标
void
ftext_get_wordpos(void* p,int* w,int *h);
/************************************************************************/
/* 销毁字体句柄                                                                     */
/************************************************************************/
void
ftext_dispose(void* p);
/************************************************************************/
/* 设置文本对象的输入缓冲区大小,当文本数据需要更大的缓冲区的时候可以调用改接口修改                              
/************************************************************************/
void
ftext_set_buffer(void* p,int bufferSize);
/*
 *获取文本对象的显示容器(Sprite)
 */
void* 
ftext_get_container(void* p);
//开启字符缓冲区存储链表
void
ftext_open_wordlist(void* p);
//弹出一个字符,即删掉一个字符
void
ftext_pop_word(void* p);
//设置文本是否换行
void 
ftext_set_wordWrap(void* p,int v);

//设置文本的点击区域和回调函数
void
ftext_set_hit(void* p,
			  void (*clickCallBack)(void* ,int ,int ),
			  int x,int y,int w,int h);

#endif