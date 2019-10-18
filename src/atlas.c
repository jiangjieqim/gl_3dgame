#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tools.h"
#include "tl_malloc.h"
#include "atlas.h"
#include "tmat.h"
#include "xml.h"
#include "fbotex.h"
#include "ex.h"
#include "sprite.h"
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
static void f_onceCallBack(void* fbo,void* sprite){
	ex_ptr_remove(sprite);//从渲染列表中移除
}
//从图集中创建一块纹理数据,并返回
GLuint
atals_new_tex(struct Atals* atals,const char* icon){
	void* fbo = 0;
	//void* spr;//用于展示的Sprite,用于观察渲染的对象是否正确
	//void* material;
	GLuint tex;
	//struct Atals* atals = ex_get_ui_atals();//图集
	struct AtlasNode p;
	void* sprite;//用于在frame buffer object中渲染

	atals_tex(atals,icon,&p);
	fbo = fbo_init(p.width,p.height);

	tex = (GLuint)fbo_getTex(fbo);

	//spr =sprite_createEmptyTex(p.width,p.height,ex_getIns()->_2dcam);
	//material = sprite_get_material(spr);
	//tmat_pushTex(material,(GLuint)tex);

	sprite = (void*)sprite_create("temp_sprite",0,0,p.width,p.height,0,fbo_get2dCam(fbo));
	sprite_bindAtals(sprite,atals);
	sprite_texName(sprite,icon,0);
	ex_add(sprite);
	//ex_add(spr);
	ex_add_fbo(fbo);

	//callBack(tex,parms);

	//fbo_dispose(fbo,0);
	//ex_remove_fbo(fbo);
	//ex_ptrRemove(sprite);
	// 
	fbo_set_once(fbo,f_onceCallBack,sprite);

	return tex;
}

void 
atals_dispose(struct Atals* ptr)
{
	xml_del(ptr->xml);
	tmat_dispose(ptr->material);
	tl_free(ptr);
}
