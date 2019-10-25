#include <stdio.h>
#include <assert.h>
#include <string.h>


#include "common.h"
#include "tools.h"
#include "tl_malloc.h"
#include "atlas.h"
#include "tmat.h"
#include "xml.h"
#include "fbotex.h"
#include "ex.h"
#include "sprite.h"
#include "map.h"
#include "gettime.h"

//#define DEBUG_SHOW_TIME

/*
	根据图集资源id获取配置

	struct AtlasNode* ptrOut:根据name获取到的数据会填充到ptrOut中
*/
static int
getNode(struct XMLSList* xml,const char* name,struct AtlasNode* ptrOut)
{
	struct XmlNode* node=xml_getrow(xml,"n",name);
	if(!node){
		return 0;//未找到节点
	}

	ptrOut->x=	xml_getfloat(node,"x");
	ptrOut->y=	xml_getfloat(node,"y");
	ptrOut->width=	xml_getfloat(node,"w");
	ptrOut->height=	xml_getfloat(node,"h");

	return 1;
}
static void
getTextureInfo(struct XMLSList* xml,const char* name,float* pWidth,float* pHeight)
{
	char bufferXmlURL[G_BUFFER_128_SIZE];
	struct XmlNode* node;

	memset(bufferXmlURL,0,G_BUFFER_128_SIZE);
	sprintf_s(bufferXmlURL,G_BUFFER_128_SIZE,"%s.tga",name);
	node=xml_getrow(xml,"imagePath",name);
	*pWidth=xml_getfloat(node,"width");
	*pHeight=xml_getfloat(node,"height");
}
struct Atals* 
atals_load(const char* path,const char* name){
	struct Atals* ptr = tl_malloc(sizeof(struct Atals));

	char bufferXmlURL[G_BUFFER_128_SIZE];
	char tgaURL[G_BUFFER_128_SIZE];
	//char alphaURL[G_BUFFER_128_SIZE];

	memset(bufferXmlURL,0,G_BUFFER_128_SIZE);
	sprintf_s(bufferXmlURL,G_BUFFER_128_SIZE,"%s%s.xml",path,name);

	memset(tgaURL,0,G_BUFFER_128_SIZE);
	sprintf_s(tgaURL,G_BUFFER_128_SIZE,"%s%s.tga",path,name);

	//memset(alphaURL,0,G_BUFFER_128_SIZE);
	//sprintf_s(alphaURL,G_BUFFER_128_SIZE,"%s%s_alpha.tga",path,name);

	memset(ptr,0,sizeof(struct Atals));
	
	if(strlen(name)>G_BUFFER_64_SIZE){
		printf("文件名(%s)太长\n",name);
		assert(0);
	}
	memcpy(ptr->name,name,strlen(name));
	{

		int len;
		char* _s = tl_loadfile(bufferXmlURL,&len);
		ptr->xml =  xml_parse(_s,len);
		tl_free(_s);
	}
	getTextureInfo(ptr->xml,name,&ptr->width,&ptr->height);
	
	//创建一个材质
	ptr->material = tmat_create("spritevbo",1,tgaURL);

	return ptr;
}

void
atals_tex(struct Atals* atals,const char* name,struct AtlasNode* ptrOut)
{
	//printf("===============>%s,%s\n",atals->name,name);
	if(!getNode(atals->xml,name,ptrOut))
	{
		printf("未找到图集(%s)中的资源(%s)\n",atals->name,name);
		assert(0);
	}
}


#define _ICON_SIZE_  32
struct Atals_params
{
	void* sprite;
	GLuint tex;
	char icon[_ICON_SIZE_];
	void* fbo;
	void (*callBack)(void*,void*);
	void* parms;
};
#ifdef DEBUG_SHOW_TIME
static long time;
#endif
//
//static void 
//f_onceCallBack(void* fbo,void* atals_params){
//
//}

static void
f_callLater(void*p){
	struct Atals_params* ap = (struct Atals_params*)p;

	struct FBOTexNode* fbo = ap->fbo;
	//fbo->enable=0;          parms);
	//fbo->onceCallBack(fbo,fbo->parms);
	//f_onceCallBack(fbo,fbo->parms);
	
	ex_ptr_remove(ap->sprite);//从渲染列表中移除
	map_set(ex_getIns()->texmap,ap->icon,(void*)ap->tex); 
#ifdef DEBUG_SHOW_TIME
	printf("atals_new_tex消耗 : %ld 毫秒\n",get_longTime()-time);
#endif
	ex_remove_fbo(fbo);
	fbo_dispose(fbo,0);
	// 
	printf("f_callLater 构造%s纹理%d结束\n",ap->icon,(void*)ap->tex);
	if(ap->callBack)
		ap->callBack((void*)ap->tex,ap->parms);

	tl_free(p);//销毁回调参数对象

	//ex_lua_evt_dispatch(sprite,EVENT_ENGINE_SPRITE_CLICK,b->name);
}
//从图集中创建一块纹理数据,并返回
GLuint
atals_new_tex(struct Atals* atals,const char* icon,
			  void (*callBack)(void*,void*),void* parms){
	GLuint tex;
	//long t = get_longTime();
	struct MapNode * node ;
	
	struct Atals_params* ap;

	node = map_get(ex_getIns()->texmap,icon);
	//printf("map_get消耗 : %ld 毫秒\n",get_longTime()-t);
	if(node){
		//printf("复用键值:%0x\n",node);
		return (GLuint)node->value;
	}
	else{
		void* fbo = 0;
		//void* spr;//用于展示的Sprite,用于观察渲染的对象是否正确
		//void* material;

		//struct Atals* atals = ex_get_ui_atals();//图集
		struct AtlasNode p;
		void* sprite;//用于在frame buffer object中渲染

		char tname[32];

		if(strlen(icon)>_ICON_SIZE_){
			printf("icon的文本长度太大\n");
			assert(0);
		}

#ifdef DEBUG_SHOW_TIME
		time = get_longTime();
#endif

		atals_tex(atals,icon,&p);
		fbo = fbo_init(p.width,p.height);

		tex = (GLuint)fbo_getTex(fbo);

		tl_newName(tname,32,0);

		sprite = (void*)sprite_create(tname,0,0,p.width,p.height,0,fbo_get2dCam(fbo));
		sprite_bindAtals(sprite,atals);
		sprite_texName(sprite,icon,0);
		ex_add(sprite);
		//ex_add(spr);
		ex_add_fbo(fbo);
		ap = (struct Atals_params*)tl_malloc(sizeof(struct Atals_params));
		memset(ap,0,sizeof(struct Atals_params));
		ap->sprite = sprite;
		ap->tex = tex;
		ap->fbo = fbo;
		ap->callBack = callBack;
		memset(ap->icon,0,_ICON_SIZE_);
		memcpy(ap->icon,icon,strlen(icon));
		//printf("icon = %s\n",ap->icon);
		//fbo_set_once(fbo,f_onceCallBack,ap);
		//printf("构造tex:%d\n",tex);
		callLater(f_callLater,ap);
	}
	return tex;
}

void 
atals_dispose(struct Atals* ptr)
{
	xml_del(ptr->xml);
	tmat_dispose(ptr->material);
	tl_free(ptr);
}
