#ifndef _ATALS_
#define _ATALS_
#include "tools.h"
#include <gl/glew.h>

struct Atals
{
	//图集名
	char name[G_BUFFER_64_SIZE];
	
	//图集宽高
	float width,height;

	//材质对象引用,图集里绑定材质引,可以复用纹理和材质资源
	void* material;

	//配置
	struct XMLSList* xml;
};
/************************************************************************/
/* 一个图集结构体,坐标,宽高,在一张位图中取一块矩形像素
/************************************************************************/
struct AtlasNode
{
	float x,y,width,height;
};
/*
	加载图集
*/
struct Atals* atals_load(const char* path,const char* name);

/*
	销毁图集
*/
void atals_dispose(struct Atals* ptr);

/*
	获取图集节点信息数据
*/
void atals_tex(struct Atals* atals,const char* name,struct AtlasNode* ptrOut);

//从图集中创建一块纹理数据,并返回
GLuint
atals_new_tex(struct Atals* atals,const char* icon);
#endif