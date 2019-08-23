#include <stdio.h>
#include <string.h>

#include "tools.h"
#include "tl_malloc.h"
#include "evt.h"
#include "ftext.h"

#include "input.h"
#include "sprite.h"
#include "ex.h"

//****************************************************************//
typedef struct Input{
	//ftext句柄
	void* t;	
}Input;

//键盘事件
static void
f_key2(int evtId,void* data,void* thisObj){
	#define _WORD_SIZE_ 2
	struct Input* ptr = (struct Input*)thisObj;
	struct E_KeyBoard* pkey = (struct E_KeyBoard*)data;
	void* txt = ptr->t;
	char _word[_WORD_SIZE_];
	if(ex_getIns()->curFocus!=ftext_get_container(ptr->t)/*ptr->bg*/){
		//不在input焦点上,知己诶返回
		return;
	}
	
	if(pkey->key == 8){
		//退格
		ftext_pop_word(txt);
		return;
	}else if(pkey->key == 13){
		//回车
		//printf("input key = %d,(%s)\n",pkey->key,input_get_heap_str(ptr));
		//evt_dispatch(ptr);
		//是否清空输入框中的数据
		//ftext_clear(txt);
		return;
	}
	
	memset(_word,0,_WORD_SIZE_);
	sprintf_s(_word,_WORD_SIZE_,"%c",pkey->key);

	ftext_parse(txt,_word,0,0);
}
static void
focusChangeHandle(int evtId,void* data,void* thisObj){
	struct Sprite* p = (struct Sprite*)data;
	//printf("%0x 焦点变化为 %0x!\n",p,ex_getInstance()->curFocus);
	
	//struct Input* ptr = (struct Input*)thisObj;
	//if(ex_getInstance()->curFocus == ptr->bg){
	//	ex_setv(ptr->bg,FLAGS_DRAW_PLOYGON_LINE);
	//}else{
	//	ex_resetv(ptr->bg,FLAGS_DRAW_PLOYGON_LINE);
	//}
}
static void 
clickCallBack(struct Sprite* spr,int x,int y){

}
void intput_set_pos(void *p,int x,int y){
	struct Input* ptr=(struct Input*)p;
	sprite_set_self_pos(/*ptr->bg*/ftext_get_container(ptr->t),x,y);
}
//void input_set_bg(void* p,const char* url){
//	struct Input* ptr=(struct Input*)p;
//	//sprite_set_default_tex(ptr->bg);
//	sprite_texName(ptr->bg,url,0);
//}
char* input_get_heap_str(void* p){
	struct Input* ptr=(struct Input*)p;
	return ftext_get_str(ptr->t);
}

void input_clear(void* p){
	struct Input* ptr=(struct Input*)p;
	ftext_clear(ptr->t);
}

void* input_get_container(void* p){
	struct Input* ptr=(struct Input*)p;
	return	ftext_get_container(ptr->t);//ptr->bg;
}
void* input_create(int w){
	#define _NAME_SIZE_ 64

	struct Input* ptr=(struct Input*)tl_malloc(sizeof(struct Input));
	int h;

	int fh = 14;//文本高度
	//struct Sprite* bg;
	
	char name[_NAME_SIZE_];
	
	h = w;

	evt_on(ex_getIns(),EVENT_ENGINE_KEYBOARD,f_key2,ptr);
	evt_on(ex_getIns(),EVENT_ENGINE_SPRITE_FOCUS_CHANGE,focusChangeHandle,ptr);
	
	//#########################################
	//memset(name,0,_NAME_SIZE_);
	//sprintf_s(name,_NAME_SIZE_,"sprite%d",newid());
	//bg = sprite_create(name,0,0,w,fh,clickCallBack);
	//bg->atals = ex_get_ui_atals();
	//ptr->bg = bg;	
	//#########################################
	memset(name,0,_NAME_SIZE_);
	sprintf_s(name,_NAME_SIZE_,"input%d",newid());
	ptr->t = ftext_create(name,w,h,fh*1.1,fh);//13,12	20,15
	ftext_open_wordlist(ptr->t);
	ftext_setpos(ptr->t,0,0);
	
	//设置不换行
	ftext_set_wordWrap(ptr->t,0);
	//sprite_addChild(ptr->bg,ftext_get_container(ptr->t));

	ftext_set_hit(ptr->t,clickCallBack,0,0,w,fh);

	return ptr;
}

void input_dispose(void* p){
	
	struct Input* ptr=(struct Input*)p;

	//sprite_dipose(ptr->bg);
	ftext_dispose(ptr->t);
	ptr->t = 0;

	tl_free(p);
}