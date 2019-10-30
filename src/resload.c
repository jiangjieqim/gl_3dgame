#include <string.h>

#include <stdio.h>
#include "resload.h"
#include "tools.h"
#include "tl_malloc.h"
#include "evt.h"
#include "atlas.h"
#include "ex.h"
#include "map.h"

struct RES_Load{
	struct LStackNode* list;
	void(*endCallBack)(void* pResload,void* param);
	void* param;
};
//void resload_tex(const char* tex, void(*endCallBack)(void*,void* param){
void* resload_create(void(*endCallBack)(void*,void*),void* param){
	struct RES_Load* res = tl_malloc(sizeof(struct RES_Load));
	res->endCallBack = endCallBack;
	res->param = param;
	res->list = LStack_create();
	return (void*)res;
}

void resload_push(void* ptr,const char* tex){
	struct RES_Load* res = (struct RES_Load*)ptr;
	char* icon = (char*)tl_malloc(strlen(tex)+1);
	memset(icon,0,strlen(tex)+1);
	memcpy(icon,tex,strlen(tex));
	LStack_push((void*)res->list,(void*)icon);
}
static void
f_render1(int id,void* p,void* thisObj){
	
	resload_start(thisObj);
}
static void
f_load_textCallBack(void* tex,void* param){
	struct MapNode* node =(struct MapNode*)map_getNodeByValue(ex_getIns()->texmap,tex);
	
	//printf("纹理加载结束 句柄 tex = %d\n",tex);
	
	//ex_lua_evt_dispatch(0,EVENT_ENGINE_TEX_LOAD_COMPLETE,(const char*)node->key);//纹理构建完成
	
	if(param){
		//struct RES_Load* res = (struct RES_Load*)param;
		//printf("下一个资源节点index = %d\n\n\n",ex_getIns()->index);
		//evt_once(ex_getIns(),EVENT_ENGINE_RENDER_3D,f_render1,param);
		resload_start(param);
	}
}

static void
f_render(int id,void* p,void* thisObj){

	struct RES_Load* res = (struct RES_Load*)thisObj;
	
	int length = LStack_length(res->list);
	
	//printf("*****资源长度length = %d\n",length);

	if(length>0){
		int data;
		char* icon;
		LStack_pop(res->list,&data);
		icon = (char*)data;
		//printf("加载的icon=%s\n",icon);
		
		atals_new_tex(ex_get_ui_atals(),icon,f_load_textCallBack,thisObj);
		//printf("free %d\n",icon);
		tl_free(icon);
		
	}else{
		//printf("***********结束!\n");
		res->endCallBack(res,res->param);

		LStack_delete(res->list);
		tl_free(res);
	
	}
}
void resload_start(void* ptr){
	//printf("********0x%0x",ptr);
	f_render(0,0,ptr);
	//evt_once(ex_getIns(),EVENT_ENGINE_RENDER_3D,f_render,ptr);
}