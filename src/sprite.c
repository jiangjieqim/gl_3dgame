#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "tools.h"
#include "ex.h"
#include "tmat.h"
#include "sprite.h"
#include "atlas.h"
#include "gettime.h"
#include "obj.h"
#include "obj_vbo.h"
#include "tl_malloc.h"
#include "base.h"
#include "fbotex.h"
#include "camera.h"
#include "map.h"

//#define _SET_POS_DEBUG_

//#define _UV_DEBUG_


static void f_refreshChildPos(void* ptr);

static f_refreshLocalPos(struct Sprite* spr){
	if(spr->parent){
		struct Sprite* parent = (struct Sprite*)spr->parent;
		spr->localx = spr->screenX - parent->screenX;
		spr->localy = spr->screenY - parent->screenY ;
	}
}
static char*
f_getName(struct Sprite* p){
	struct HeadInfo* b =(struct HeadInfo*)p->base;
	return b->name;
}


/* chiid是否在parent中*/
static int 
f_isHasChild(struct Sprite* pp,int child){
	struct LStackNode* s = (struct LStackNode* )pp->childs;
	void* top,*p;
	int data;
	if(!pp->childs)	return 0;

	top = s;
	p=top;

	while((int)LStack_next(p)){
		//struct Sprite* tpp=0;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		if(data == child)	
			return 1;
	}
	return 0;
}

//构造两个三角形所需要的数据大小(Glfloat数据的个数)
enum {
	//uv vertex
	SPRITE_VERTEX_COUNT_30=30,

	//uv normal vertex
	SPRITE_VERTEX_COUNT_48=48,
	
	//uv需要GLfloat个数
	UV_COUNT_6 = 6,

	UV_COUNT_4 = 4,
};

//顶点解析类型
enum
{
	Type_6Vertex = 1,
	Type_4Vertex = 2
};

/*
 *获取索引中0号位置的索引
 */
static struct ObjVBO*
getvbo(struct Sprite* p)
{
	if(p->vbo)
	{
		return (struct ObjVBO*)LStatk_getAddressByIndex(p->vbo->ptrList,0);
	}
	return 0;
}
/*
	 顶点跨度
*/
#define  VBO_VERTEX_SPLINE 5	//u v x y z
static void updateSpriteMat4x4(struct Sprite* b,float x,float y,float z,float scaleX,float scaleY,float scaleZ,float _rx,float _ry,float _rz);
//打印uv GL_T2F_N3F_V3F
static void 
printfVertex(GLfloat* vert,int vertLength)
{
	int i;

	int n = 0;
	for(i = 0;i < vertLength* VBO_VERTEX_SPLINE;i++)
	{
		if(n>=VBO_VERTEX_SPLINE){
			printf("\n");
			n=0;
		}
		
		printf("%f,",vert[i]);
		n++;
	}
}

/*
 *	float * v 为 sizeof(float) * 12 = 48 bytes
 */
static void
SetUV_6Vertex(float *v,const int gap,struct Vec2 a1,struct Vec2 a2,struct Vec2 a3,struct Vec2 a4)
{
	
	int a = 1;
	int b = 0;
	int i = 0;
	
	//if(filp)
	//{
	//	a = 0;
	//	b = 1;
	//}
	
	v[i+a] = a1.x;
	v[i+b] = a1.y;		
	i+=gap;
	//2
	v[i+a] = a2.x;
	v[i+b] = a2.y;	
	i+=gap;
	//3
	v[i+a] = a3.x ;
	v[i+b] = a3.y;	
	i+=gap;
	//4 = 3
	v[i+a] = a3.x;
	v[i+b] = a3.y;
	i+=gap;
	//5 = 2
	v[i+a] = a2.x;
	v[i+b] = a2.y;	
	i+=gap;
	//6
	v[i+a] = a4.x;
	v[i+b] = a4.y;
}


static void 
SetUV_4Vertex(float *v,const int gap,struct Vec2 a1,struct Vec2 a2,struct Vec2 a3,struct Vec2 a4)
{
	int a = 1;
	int b = 0;
	int i = 0;

	v[i+a] = a1.x;
	v[i+b] = a1.y;		
	i+=gap;
	//2
	v[i+a] = a2.x;
	v[i+b] = a2.y;	
	i+=gap;
	//3
	v[i+a] = a3.x ;
	v[i+b] = a3.y;	
	i+=gap;
	//4
	v[i+a] = a4.x;
	v[i+b] = a4.y;
}


/*
	设置sprite的UV,若是VBO就是glBufferSubData更新VBO数据
*/
void sprite_setUV(struct Sprite* sprite,float x,float y,float w,float h)
{
	//int i=0;
	//交换a,b可旋转UV
	/*int a = 1;
	int b = 0;*/
	

	struct Vec2 a1,a2,a3,a4;
	float* v;

	//if(!strcmp(f_getName(sprite),"table: 04BBEBA0")){
	//if(sprite->screenX == 0 && sprite->screenY == 0){
	//if(sprite->material!=0 && tmat_getID(sprite->material)==1){
	//	return;
	//}
	
	//if(strcmp(sprite->base->))
	//return;//仅仅做测试用的

	a1.x = x;
	a1.y = y;

	a2.x = x + w;
	a2.y = y;

	a3.x = x ;
	a3.y = y + h;

	a4.x = x + w;
	a4.y = y + h;

	v = sprite->vertexs;
	if(v){
		SetUV_6Vertex(v,VBO_VERTEX_SPLINE,a1,a2,a3,a4);
	}
	if(sprite->vbo)
	{	
		//VBO模式
		struct ObjVBO* vbo=getvbo(sprite);
		if(vbo->uvID==0)
		{
			printf("vbo->uvID = 0\n");
			assert(0);
		}
		else
		{
			float* __ptr;

			//Specifies the size in bytes of the data store region being replaced.(包含的内存块字节尺寸)
			int _bytesSize;

			if(sprite->parseType == Type_6Vertex)
			{
				float ptr[UV_COUNT_6*UV_GAP];
				
				//6个UV顶点数据 * 2(一个顶点需要的uv个数) * 一个float所占用的字节大小
				_bytesSize = UV_COUNT_6*UV_GAP*sizeof(float);
				
				SetUV_6Vertex(ptr,UV_GAP,a1,a2,a3,a4);
				__ptr = ptr;
			}
			else if(sprite->parseType == Type_4Vertex)
			{
				float ptr[UV_COUNT_4*UV_GAP];

				//4个UV顶点数据 * 2(一个顶点需要的uv个数) * 一个float所占用的字节大小
				_bytesSize = UV_COUNT_4*UV_GAP*sizeof(float);
				SetUV_4Vertex(ptr,UV_GAP,a1,a2,a3,a4);
				__ptr = ptr;
			}
			
			//更新UV的VBO
			glBindBuffer(GL_ARRAY_BUFFER,vbo->uvID);
			glBufferSubData(GL_ARRAY_BUFFER,0,_bytesSize,__ptr);
		}
	}
}

//加载一个四边形面片,返回一块缓存区引用,存储顶点数据
static GLfloat* 
LoadQuadObj(int* ptrVertexLength)
{
/*
	struct EX* ex=ex_getInstance();
	int length = ex->spriteVert.vertLen;
	GLfloat* vertsPtr ;

	*ptrVertexLength = length / VBO_VERTEX_SPLINE;
	
	//copy顶点数据
	vertsPtr = (GLfloat*)tl_malloc(sizeof(GLfloat)*length);
	memcpy(vertsPtr,ex->spriteVert.vertex,(size_t) sizeof(GLfloat) * length);
	
	return vertsPtr;
*/
	char* _objStr;
	int _dataCount;
	float* verts;
	_objStr=tl_loadfile("\\resource\\obj\\quad.obj",0);
	obj_parse(_objStr,&_dataCount,(int*)&verts,OBJ_UV_VERTEX);
	*ptrVertexLength = _dataCount;
	tl_free(_objStr);
	return verts;
}

static struct Obj_vbo_model* 
load_vbo_6Vertex()
{	
    //uv vertex

	GLfloat _lverts[SPRITE_VERTEX_COUNT_30]={
		0.000000,0.000000,		-0.500000,-0.500000,0.000000,
		1.000000,0.000000,		-0.500000,0.500000,0.000000,
		0.000000,1.000000,		0.500000,-0.500000,0.000000,
		0.000000,1.000000,		0.500000,-0.500000,0.000000,
		1.000000,0.000000,		-0.500000,0.500000,0.000000,
		1.000000,1.000000,		0.500000,0.500000,0.000000
	};
	const int dataType = OBJ_UV_VERTEX;

	struct Obj_vbo_model* vbo;
	char buffer[G_BUFFER_64_SIZE];
	tl_newName(buffer,G_BUFFER_64_SIZE,0);

	vbo = objVBO_create(buffer,dataType);

	objVBO_pushNode(vbo ,(GLfloat*)_lverts,sizeof(_lverts) / sizeof(GLfloat));
	return vbo;
}

//static char* _gobj_str = 0;//共享顶点数据

static struct Obj_vbo_model* 
f_objVBO_load4Vertex(){
	int verts,_bufferSize;
	char* _objStr;
	const int dataType = OBJ_UV_VERTEX;
	struct Obj_vbo_model* vbo;
	char buffer[G_BUFFER_64_SIZE];
	//printf("_gobj_str = %0x\n",_gobj_str);
	struct MapNode * node ;
	const char* path = "\\resource\\obj\\quad.obj";
	node = map_get(ex_getIns()->mapptr,path);
	if(node){
		_objStr = node->value;
		//printf("复用_gobj_str %0x\n",_objStr);
	}else{
		_objStr = tl_loadfile(path,0);
		map_set(ex_getIns()->mapptr,path,_objStr);
	}
	/*if(_gobj_str == 0){
		_objStr=tl_loadfile("\\resource\\obj\\quad.obj",0);
		_gobj_str = _objStr;
	}
	else{
		printf("复用_gobj_str %0x\n",_gobj_str);
		_objStr = _gobj_str;
	}*/
	//printf("f_objVBO_load4Vertex = %0x\n",_objStr);

	tl_newName(buffer,G_BUFFER_64_SIZE,0);
	vbo = objVBO_create(buffer,dataType);
	//printf("vbo objVBO_create = %0x\n",vbo);

	obj_parse((char*)_objStr,&_bufferSize,&verts,dataType);

	//printf("obj_parse\n");

	objVBO_pushExportObj(vbo,_objStr);

	//printf("objVBO_pushExportObj\n");

	//tl_free(_objStr);
	//printf("last vbo = %0x\n",vbo);

	return vbo;
}

/*
 *构造VBO
 * uv normal verter和uv vertex都支持
 */
static struct Obj_vbo_model* 
load_vbo(int ParseType){
	if(ParseType == Type_6Vertex){
		return load_vbo_6Vertex();
	}
	else if(ParseType == Type_4Vertex){
		return f_objVBO_load4Vertex();
	}
	return 0;
}

/*
	构建2个点击三角形,这两个三角形构成一个矩形的点击区域,用来检测是否点击到该区域
*/
static void
setHitTriangle(struct Sprite* spr){
	int i = 0;
	struct Vec2 a1,a2,a3,a4;
	
	if(!spr->hitTriangle){
		//没有设置可碰撞属性
		return;
	}

	a1.x =  spr->screenX + spr->hitX;
	a1.y =	spr->screenY + spr->hitY;
	
	a2.x = spr->screenX +  spr->hitWidth;//+spr->mWidth;
	a2.y = spr->screenY;

	a3.x = spr->screenX+ spr->hitWidth;//spr->mWidth;
	a3.y = spr->screenY+ spr->hitHeight;//spr->mHeight;

	a4.x = spr->screenX;
	a4.y =  spr->screenY + spr->hitHeight;//spr->mHeight;

	//a1
	spr->hitTriangle[0] = a1.x ;
	spr->hitTriangle[1] = a1.y ;
	spr->hitTriangle[2] = 0;

	//a2
	spr->hitTriangle[3] = a2.x;
	spr->hitTriangle[4] = a2.y;
	spr->hitTriangle[5] = 0;

	//a3
	spr->hitTriangle[6] = a3.x;
	spr->hitTriangle[7] = a3.y;
	spr->hitTriangle[8] = 0;

	////===================================================

	//a1
	spr->hitTriangle[9] =  a1.x;
	spr->hitTriangle[10] = a1.y;
	spr->hitTriangle[11] = 0;

	//a4
	spr->hitTriangle[12] = a4.x;
	spr->hitTriangle[13] = a4.y;
	spr->hitTriangle[14] = 0;

	//a3
	spr->hitTriangle[15] =  a3.x;
	spr->hitTriangle[16] =  a3.y;
	spr->hitTriangle[17] = 0;
}
//
///*
//	设置层级
//*/
//static void
//setLayer(struct Sprite* sprite,int layer)
//{
//	sprite->pos_z = layer;
//}


void
sprite_set2dCam(void* spr,void* _2dcam){
	/*struct Sprite* sprite = (struct Sprite*)spr;
	sprite->_2dcam = _2dcam;*/
	base_set_cam(spr,_2dcam);
}


/*
	设置sprite的屏幕坐标,(屏幕以左上角为0,0起始点)
*/
static void
setSpriteScreenPos(struct Sprite* sprite,float x,float y){
	float w,h;
	void* cam = base_get_cam((void*)sprite);//sprite->_2dcam;//ex_getIns()->_2dcam; 
	/*if(sprite->_2dcam){
		cam = sprite->_2dcam;
		printf("change...\n");
	}*/
	cam_get_wh(cam,&w,&h);

	sprite->screenX = x;
	sprite->screenY = y;
	sprite->pos_x = x;
	//sprite->pos_y = ex->_screenHeight-y-sprite->mHeight;	
	sprite->pos_y = h-y-sprite->mHeight;

#ifdef _SET_POS_DEBUG_
	{
		int _stat = 0;
		struct HeadInfo* b = base_get(sprite);
		
		printf("设置坐标%s\t%.3f,%.3f\t%.3f,%.3f x = %f y = %f,是否是默认的cam %d,screenw=%f,screenh=%f\n",
			b->name,sprite->pos_x,sprite->pos_y,sprite->screenX,sprite->screenY,x,y,
			(cam == ex_getIns()->_2dcam ? 1 : 0),
			ex_getIns()->_screenWidth,ex_getIns()->_screenHeight
			);

		if(!strcmp(b->name,"stage2d")){
		//	getchar();
		}
	}
#endif
}

/*
	重置sprite的尺寸
*/
void sprite_resize(struct Sprite* spr,int w,int h){
	spr->mWidth = w;
	spr->mHeight = h;
	sprite_setpos(spr,spr->screenX,spr->screenY);
}

void sprite_set_width(struct Sprite* spr,int w){
	spr->mWidth = w;
	sprite_setpos(spr,spr->screenX,spr->screenY);
}

void sprite_set_height(struct Sprite* spr,int h){
	spr->mHeight = h;
	sprite_setpos(spr,spr->screenX,spr->screenY);
}

/*
 *如果此sprite是其他的sprite的子对象的时候,使用该接口设置其相对于父对象的坐标
 */
void sprite_set_self_pos(void* p,int x,int y){
	struct Sprite* spr = (struct Sprite*)p;	
	if(spr->parent){
		struct Sprite* parent = (struct Sprite*)spr->parent;
		int realx = x + parent->screenX;
		int realy = y + parent->screenY;
		
		spr->localx = x;
		spr->localy = y;

		x = realx;
		y = realy;
	}
	//printf("%d,%d\n",x,y);
	sprite_setpos(spr,x,y);
}

//void sprite_refresh_local_pos(void* p){
//	struct Sprite* spr = (struct Sprite*)p;	
//	if(spr->parent){
//		struct Sprite* parent = (struct Sprite*)spr->parent;
//		spr->localx = parent->screenX - spr->screenX;
//		spr->localy = parent->screenY - spr->screenY;
//	}
//}

//设置绝对坐标
void sprite_setpos(struct Sprite* spr,int x,int y){
	//printf("%d,%d\n",x,y);
	
	//evt_once(NextFrame)到下一个关键帧的时候更新数据,这样不会抖屏
	
	setSpriteScreenPos(spr,x,y);//更新屏幕坐标
	setHitTriangle(spr);//更新点击区域
	f_refreshLocalPos(spr);
	f_refreshChildPos(spr);//更新sprite的子对象的坐标
}

static void 
InitType(struct Sprite* pSpr)
{
	pSpr->useVBO = 1;
	if(pSpr->useVBO){
		pSpr->parseType = Type_4Vertex;//Type_4Vertex,Type_6Vertex;
		
		pSpr->vbo=load_vbo(pSpr->parseType);//使用VBO
		//printf("pSpr->vbo = %0x\n",pSpr->vbo);
	}else
		pSpr->vertexs = LoadQuadObj(&pSpr->vertLen);//加载顶点数据(非VBO实现)
}
/*
	是否有鼠标事件,有回调句柄说明有拾取事件
*/
int 
sprite_isCanClick(void* n){
	struct Sprite* pSpr = (struct Sprite*)n;
	return (int)pSpr->clickCallBack;
}

void
sprite_set_clickHandle(void* p,void (*clickCallBack)(void* ,int ,int )){
	struct Sprite* pSpr = (struct Sprite*)p;
	pSpr->clickCallBack = clickCallBack;
}
void*
sprite_createEmptyTex(int texW,int texH,void* _2dCam){
	struct Sprite* spr = 0;
	void* mat= tmat_create_empty("fbotex");//fbotex

	char buffer[64];
	tl_newName(buffer,64,"sprite");
	tmat_setID(mat,1);

	//tmat_pushTex(mat,(GLuint)mirrorTexture);		//void* mirrorTexture,

	spr = sprite_create(buffer,0,0,texW,texH,0,_2dCam);
	//sprite_rotateZ(spr,-PI/2);//sprite旋转90度
	//sprite_rotateX(spr,PI);
	//base_setv(spr,FLAGS_REVERSE_FACE);
	
	//sprite_setUV(spr,0,0,1,1);

	spr->material = mat;
	return spr;
}
struct Sprite* 
sprite_create(char* _spriteName,
			  int x,int y,int width,int height,
			  void (*clickCallBack)(struct Sprite* ,int ,int ),void* _2dCam)
{
	//&ex->myButtonPtr;//
	struct HeadInfo* base = NULL;
	//初始化按钮
	struct Sprite* pSpr =	(struct Sprite*)tl_malloc(sizeof(struct Sprite));//new Button;
	
	//log_color(0,"sprite_create name=(%s)\n",_spriteName);

	memset(pSpr,0,sizeof(struct Sprite));
	
	pSpr->m_bPressed = 0;
	//pSpr->_renderState = 1;

	pSpr->parent = 0;

	pSpr->mWidth  = width;
	pSpr->mHeight = height;
	pSpr->hitWidth = width;
	pSpr->hitHeight = height;
	pSpr->hitTriangle = tl_malloc(sizeof(float)*SPRITE_TRIANGLE_COUNT);
	//printf("pSpr = %0x\n",pSpr);
	//设置顶点组织方式
	InitType(pSpr);
	//printf("InitType end = %0x\n",pSpr);
	
	pSpr->base = base_create(TYPE_SPRITE_FLIE,_spriteName,0,0,0);
	
	if(_2dCam){
		//pSpr->_2dcam = _2dCam;
		base_set_cam((void*)pSpr,_2dCam);
	}else{
		//pSpr->_2dcam = ex_getIns()->_2dcam;
		base_set_cam((void*)pSpr,ex_getIns()->_2dcam);
	}
	
	sprite_setpos(pSpr,x,y);
	
	base = pSpr->base;
	base->parent = pSpr;
	if(base){
		setv(&base->flags,FLAGS_VISIBLE);//FLAGS_RAY
		pSpr->clickCallBack = clickCallBack;
		//ex_add(pSpr);
	}
	//pSpr->pos_z = ex_newPosZ();//此处不设置z的值,在ex_add的时候再设置
	pSpr->zScale = 1.0;
	sprite_setUV(pSpr,0,0,1,1);//初始化设置UV
	

	
	return pSpr;
}
void
sprite_set_z(void* p,float v){
	struct Sprite* pSpr = (struct Sprite*)p;
	pSpr->pos_z = v;
}
void 
sprite_setDragScope(struct Sprite* pSpr,int x,int y,int w,int h)
{
	struct HeadInfo* base = base_get((void*)pSpr);

	if(pSpr->mWidth>w ||pSpr->mHeight>h)
	{
		log_color(0xff0000,"滑动区域设置太小sprite所屏幕像素单位(mWidth = %d,拖拽区域w=%d)(mHeight = %d,拖拽区域h=%d)\n",(int)pSpr->mWidth,w,(int)pSpr->mHeight,h);
		//assert(0);
		return;
	}

	log_color(0,"Sprite name=[%s]设置FLAGS_DRAG\n",base->name);

	setv(&base->flags,FLAGS_DRAG);
	pSpr->oldx = pSpr->screenX;
	pSpr->oldy = pSpr->screenY;
	pSpr->dragX = x;
	pSpr->dragY = y;
	pSpr->dragWidth = w;
	pSpr->dragHeight = h;
}

/*
	是否可以渲染
*/
int 
sprite_isEnable(int data)
{
	int objType;
	struct HeadInfo* base = base_get((void*)data);
	
	objType = base->curType;

	if(objType == TYPE_SPRITE_FLIE && getv(&(base->flags),FLAGS_VISIBLE))
	{
		return 1;
	}
	return 0;
}

/*
	更新sprite矩阵
*/
static void
updateSpriteMat4x4(struct Sprite* p,
				   float x,float y,float z,
				   float sx,float sy,float sz,
				   float rx,float ry,float rz)
{

	Matrix44f xyz,m_scale,m_rx,m_ry,m_rz, result,tmp2,tmp3;
	mat4x4_zero(p->mat4x4);
	mat4x4_zero(result);
	mat4x4_zero(tmp2);
	mat4x4_zero(tmp3);

	//x,y,z
	mat4x4_identity(xyz);
	mat4x4_translate(xyz,x,y,z);

	//scale
	mat4x4_identity(m_scale);
	mat4x4_scale(m_scale,sx,sy,sz);

	//rx
	mat4x4_identity(m_rx);
	mat4x4_rotateX(m_rx,rx);

	//ry
	mat4x4_identity(m_ry);
	mat4x4_rotateY(m_ry,ry);

	//rz
	mat4x4_identity(m_rz);
	mat4x4_rotateZ(m_rz,rz);//PI/2

	multi2(tmp2,m_ry,m_rx);		//ry,rx复合变换
	multi2(tmp3,tmp2,m_rz);	//rz  *(ry * rx)复合变换
	multi2(result,tmp3,m_scale);//缩放矩阵
	
	multi2(p->mat4x4,xyz,result);//位移矩阵
}
void
sprite_set_default_tex(void* ptr){
	struct Sprite* p = (struct Sprite*)ptr;
	//ex_setv(p,FLAGS_DRAW_PLOYGON_LINE);
	p->atals = ex_get_ui_atals();
	sprite_texName(p,"gundi.png",0);
}
//获取Sprite引用的材质引用,
//可以从图集中获取,也可以自定义材质引用(有图集用图集,无图集用自身的材质)
static void* 
getMaterial(struct Sprite* p){
	/*
	//测试fbo
	if(p->material!=0 && tmat_getID(p->material)==1){
		struct FBOTex* fbo = (struct FBOTex*)ex_getIns()->fbo;
		//printf("name:%s  %d\n",f_getName(p),strcmp(f_getName(p),"stage2d"));
		//printf("%s\n",f_getName(p));
		return fbo->mat;
	}
	*/
	if(p->atals){
		return p->atals->material;
	}
	
	if(p->material){
		return p->material;
	}
	//HeadInfo* head = (HeadInfo*)p->base;
	//printf("getMaterial %s ,没有设置图集,也没有默认材质\n",head->name);
	//assert(0);//这里需要对没有设置材质的对象进行修复!!!
	//sprite_set_default_tex(p);
	// 
	
	if(p->base){
		HeadInfo* head = (HeadInfo*)p->base;
		if(head->tmat){
			return head->tmat;
		}
	}
	//printf("%s,",base_get(p)->name);

	return 0;
}

//渲染一个四边形
static void 
renderSprite(struct Sprite* p)
{
	void* material = getMaterial(p);//p->atals->material;

	if(material){
		//void* material = getMaterial(p);//p->atals->material;
		struct HeadInfo* base = base_get(p);
		char* shaderName = 0;



		if(!material){
			printf("(%s)Sprite没有赋予材质\n",((struct HeadInfo*)p->base)->name);
			assert(0);
		}
		

		


		{
			GMaterial* __mat = (GMaterial*)material;
			//char* type = __mat->glslType;
			shaderName = __mat->glslType;
		}
		if(p->useVBO){
			//渲染管线采用vbo模式
			


//#ifdef _SPRITE_RENDER_LINE_
			if(g_sprite_line){
			//if(getv(&base->flags,FLAGS_SPRITE_RENDER_LINE)){
				//添加线框渲染
				objVBO_renderNode(getvbo(p),material,
					"spritevbo1",
					p->mat4x4,
					GL_LINE,base,0,p->grid9);
			//}
			}else{
//#else
			objVBO_renderNode(getvbo(p),material,
				//"spritevbo",
				shaderName,
				p->mat4x4,
				base_get_ploygonLineMode(base),base,0,p->grid9);
			}

//#endif
		}else
			//采用固定管线方式
			tmat_renderSprite(material,
							//"sprite",
							shaderName,
							p->mat4x4,p->vertexs,p->vertLen,
							GL_T2F_V3F,
							base_get_ploygonLineMode(base));//非vbo模式
	}
}

/*
 *更新矩阵
 */
static void 
updateMatrix(struct Sprite* p)
{
	struct HeadInfo* b = p->base;
	//p->zScale = 1.5;
	float sc = p->zScale;
	updateSpriteMat4x4(p,b->x,b->y,b->z,p->mWidth*sc,p->mHeight*sc,sc,b->rx,b->ry,b->rz);
}
#define  _Time_Delay_  10//点击的延迟时间(毫秒)		10毫秒恢复状态
//按钮特效
static void
f_btn_push(struct Sprite* spr,int* pChange){
	struct HeadInfo* base = spr->base;
	if(spr->m_bPressed )
	{
		float targetScale = 0.95;

		if(spr->zScale!=targetScale)
		{
			//这里点击了一次

			spr->zScale = targetScale;//点击时,按钮宽高比变化
			//printf("鼠标一直点着按钮Sprite %f,%f\n",pBtn->m_fWidth,pBtn->m_fHeight);
			base->time=get_longTime()+_Time_Delay_;
			*pChange = 1;
		}
	}
	else
	{
		spr->zScale = 1.0;

		if(base->time!=0 && base->time - get_longTime()<=0)
		{
			//_Time_Delay_毫秒之后会处理
			*pChange = 1;
			base->time = 0;
		}
	}
}

void*
sprite_get_material(void* sprite){
	return getMaterial((struct Sprite*)sprite);
}

/*
	绘制一个sprite 
*/
void 
sprite_drawRender(int data)
{
	//struct EX* e = ex_getIns();

	struct Sprite* spr = (struct Sprite*)data;
	float x = spr->pos_x+spr->mWidth/2;
	float y = spr->pos_y+spr->mHeight/2;
	float z = spr->pos_z;//-2.0只是为了按钮可见,可用作界面的深度值
	
	//////////////////////////////////////////////////////////////////////////
	//isChange == 1的时候才会更新矩阵
	int isChange = 0;

	struct HeadInfo* base = spr->base;
	
	if(!sprite_isEnable((int)spr))
	{
		return;
	}
	if(!getMaterial(spr)){
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//位移发生变化,更新矩阵
	if(base->x!=x ||base->y!=y||base->z!=z)
	{
		isChange = 1;
	}
	if (!strcmp(base->name,"name1"))
	{
		//printf("%s,%d,",base->name,base_getv(spr,FLAGS_VISIBLE));
	}
	base->x = x;
	base->y = y;
	base->z = z;
	
	if(getv(&base->flags,FLAGS_BUTTON_EFFECT)){
		f_btn_push(spr,&isChange);//按钮特效代码,需要移到业务组件层!!!
	}
	if(spr->m_bPressed)
	{
		//左键鼠标按下的时候,会一直执行这里逻辑
		ex_lua_evt_dispatch(spr,EVENT_ENGINE_SPRITE_CLICK_DOWN,base->name);
	}
	else
	{
	}
	if(isChange)
	{
		updateMatrix(spr);
	}

	renderSprite(spr);
}

/*
	是否设置了拖拽范围区间
*/
static int
haveDragScope(struct Sprite* ptr){
	//if(ptr->m_fPosX+ptr->dragWidth - ptr->m_fWidth)
	if(ptr->dragWidth || ptr->dragHeight)
	{
		return 1;
	}
	return 0;
}
//static void 
//fLuaDragMove(const char* luaFunName,const char* name,float progress)
//{
//	lua_State* L =ex_getInstance()->mylua;
//	lua_getglobal(L,luaFunName);
//	lua_pushstring(L,name);
//	lua_pushnumber(L,progress);
//	if(lua_pcall(L,2,0,0)!=0){
//		printf("error %s\n", lua_tostring(L,-1));
//	}
//}
/*
	是否是纵向
*/
static int 
IsDragDirection(struct Sprite* sprite)
{
	return sprite->dragDirection;
}
//========================================================
//#define _ZERO_


//被拖拽控件的宽
static float 
GetWidth(struct Sprite* p)
{
#ifdef _ZERO_
	return 0.0f;
#endif
	return p->mWidth;
}

//被拖拽控件的高
static float 
GetHeight(struct Sprite* p)
{
#ifdef _ZERO_
	return 0.0f;
#endif
	return p->mHeight;
}

//p的父节点的绝对坐标
static void
f_get_global(struct Sprite* p,int* px,int* py){
	if(p->parent){
		 
		struct Sprite* ps = (struct Sprite*)p->parent;

		if(ps->parent){
			*px = ps->screenX;//-ps->localx;
			*py = ps->screenY;//-ps->localy;
		}else{
			
		}
		p->localx = p->screenX - ps->screenX;
		p->localy = p->screenY - ps->screenY;

		//printf("%0x %0x parent sx sy = %d,%d==>%d,%d\n",p,ps,*px,*py,p->localx,p->localy);
	}else{
		*px = 0;
		*py = 0;
		p->localx = 0;
		p->localy = 0;
	}
}
static void 
changeDragXY(struct Sprite* p,int* px,int* py){
//检查ScrollBar ScrollBarBG_Click接口
	if(haveDragScope(p)){
		int cx0,cx1,cy0,cy1,oldx,oldy;
		float progress;
		//sprite_refresh_local_pos((void*)p);

		int ppx=0,ppy=0;
		f_get_global(p,&ppx,&ppy);
	/*	ppx-=p->localx;
		ppy-=p->localy;*/

		
		oldx = p->oldx;
		oldy = p->oldy;

		cx0 = p->dragX+oldx + ppx;
		cx1 = oldx+p->dragWidth - GetWidth(p) + ppx;

		cy0 = p->dragY+oldy + ppy; 
		cy1 = oldy+p->dragHeight- GetHeight(p) + ppy;

		if(*px <= cx0)
			*px = cx0;
		else if(*px>=cx1) 
			*px = cx1;

		if(*py <= cy0) 
			*py = cy0;
		else if(*py>=cy1)
			*py = cy1;
		
		if(IsDragDirection(p))
			progress = (p->screenY-p->oldy-ppy)/(p->dragHeight - GetWidth(p));//纵向比率
		else
			progress = (p->screenX-p->oldx-ppx)/(p->dragWidth - GetHeight(p));//横向比率
		
		//if(p->callLuaDragMove)
		//	fLuaDragMove(p->callLuaDragMove,progress);//发送更新事件
		//if(p->callLuaDragMove)
		//{
		//	struct HeadInfo* b = base_get(p);
		//	fLuaDragMove(p->callLuaDragMove,b->name,progress);
		//}

	
		 //发送事件给lua
//		char _str[G_BUFFER_64_SIZE];
//		struct HeadInfo* b = base_get(p);
//		sprintf_s(_str, G_BUFFER_64_SIZE,"%s,%d,%d,%.3f",b->name,*px,*py,progress);	
			
		//printf("c=[%s]lx = %.3f,ly = %.3f %d %d,0x%0x	\t cx0 = %d cx1 = %d\n",_str,p->screenX,p->screenY,p->localx,p->localy,p->parent,cx0,cx1);
		ex_lua_evt_dispatch_f(p,EVENT_ENGINE_SPRITE_CLICK_MOVE,progress);
	}
}
//========================================================
void 
sprite_mouseMove(int data)
{
	struct EX* e = ex_getIns();	
	if(sprite_isEnable(data))
	{
		struct HeadInfo* base = base_get((void*)data);

		if(getv(&base->flags,FLAGS_DRAG))
		{
			struct Sprite* ptr = (struct Sprite*)data;
			
			//printf("c = %s,y = %s\n",base_get(ex_getIns()->curFocus)->name,base_get(ptr)->name);
			if(ex_getIns()->curFocus == ptr){
				if(ptr->m_bPressed){
					//鼠标按下,只有在鼠标坐标发生变化的时候				
					int x = e->mouseState.moveX - ptr->mouseDownX;
					int y = e->mouseState.moveY - ptr->mouseDownY;

					//x,y 当前鼠标拾取到的绝对坐标
					//printf("******* mx = %d,my = %d ===>%d %d\n",x,y,e->mouseState.moveX,ptr->mouseDownX);
					changeDragXY(ptr,&x,&y);
					//printf("mx = %d,my = %d\n",x,y);
					sprite_setpos(ptr,x,y);



					//sprite_set_self_pos(ptr,x,y);
				}
			}
		}
	}
}
/*
	鼠标按键
*/
void sprite_action(const int data)
{
	if(sprite_isEnable(data))
	{
		struct EX* ex = ex_getIns();

		struct Sprite* btn = (struct Sprite*)data;
		struct HeadInfo* base;
		if(!btn)
		{
			return;
		}

		base = btn->base;

		if(ex->mouseState.button==GLUT_LEFT_BUTTON){
			switch(ex->mouseState.action)
			{
				case GLUT_DOWN:
					
					

					//左键按下：
					//if( OnMouseDown(btn, ex->mouseState.xMouse, ex->mouseState.yMouse) ){
						//printf("to do something!\n");
					//}
					break;

				case GLUT_UP:
					//OnMouseUp(btn);
					btn->m_bPressed = 0;
					break;
			}
		}
	}
}

//渲染一个正投sprite
void sprite_updatePos(int data)
{
	if(sprite_isEnable(data))
	{
		struct Sprite* sprite = (struct Sprite*)data;
		sprite_setpos(sprite,sprite->screenX,sprite->screenY);
	}	
}
static char* 
f_get_name(void* p){
	struct Sprite* spr = (struct Sprite* )p;
	struct HeadInfo* b = base_get(p);
	return b->name;
}
static void 
f_sprite_removeSelf(void* p){
	struct Sprite* spr = (struct Sprite* )p;
	struct Sprite* parent = (struct Sprite* )spr->parent;
	if(parent){
		log_color(0,"sprite_removeSelf %s\n",f_get_name(p));
		sprite_removeChild(parent,p);
	}
}
/*
	销毁sprite
*/
void sprite_dipose(struct Sprite* spr){
	f_sprite_removeSelf((void*)spr);
	if (spr->childs){
		LStack_delete(spr->childs);
		spr->childs = 0;
	}

	//spr->ptr_luaCallBack = 0;
	if(spr->hitTriangle)
		tl_free(spr->hitTriangle);

	if(spr->vertexs)
		tl_free(spr->vertexs);
	
	//删除材质句柄
	if(spr->material){
		tmat_dispose(spr->material);
		spr->material = 0;
	}
	/*if(spr->material)
		tmat_dispose(spr->material);*/
	spr->atals = 0;
	
	if(spr->base){
		base_dispose(spr->base);
		spr->base = 0;
	}
	if (spr->grid9){
		tl_free(spr->grid9);
	}

	if(spr->vbo)
	{
		objVBO_dispose(spr->vbo);
	}
	LStack_delNode(ex_getIns()->renderList,(int)spr);//从渲染节点列表中移除

	tl_free(spr);
}

/*
	为sprite设置图集
*/
void 
sprite_texName(struct Sprite* ptr,
					const char* texName,
					struct AtlasNode* _pNode){
	float width,height;
	struct AtlasNode n;
	struct AtlasNode __Node;
	struct AtlasNode* ptrNode = &__Node;
	if(ptr->atals){
	//获取图集数据,填充到ptrNode引用中
		atals_tex(ptr->atals,texName,ptrNode);

		width = ptr->atals->width;
		height = ptr->atals->height;
	}
	else
	{
		log_code(ERROR_NOT_EXIST_Atals);
		printf("当sprite没有图集Atals的时候,无法调用sprite_texName接口!\n");
		assert(0);
		
		ptrNode->x = 0;
		ptrNode->y = 0;
		ptrNode->width = ptr->mWidth;
		ptrNode->height = ptr->mHeight;
		width=ptr->mWidth;
		height=ptr->mHeight;
	}

	n = *ptrNode;
#ifdef _UV_DEBUG_
	printf("修改UV,%s:%.3f,%.3f,%.3f,%.3f",texName,
		1.0f-(height-n.y)/height,
		1.0f-(width-n.x)/width,
		n.height/height,
		n.width/width
		);
#endif
	
	//设置图集UV.
	sprite_setUV(ptr,		
		1.0f-(height-n.y)/height,
		1.0f-(width-n.x)/width,
		n.height/height,
		n.width/width
		); 

	

	if (_pNode!=0){
		memcpy(_pNode,ptrNode,sizeof(struct AtlasNode));
	}
}

//创建->childs栈
static void
f_init_childsContner(void* p){
	struct Sprite* spr = (struct Sprite* )p;
	if(!spr->childs){
		spr->childs = LStack_create();
	}
}

/*
 *为spr添加一个子对象
 **/
void sprite_addChild(void* p,void* child){
	struct Sprite* spr = (struct Sprite* )p;
	struct Sprite* childspr = (struct Sprite* )child;
	childspr->parent = p;//child;		//设置父对象引用
	if(!spr->parent){
		f_init_childsContner(p);
	}
	sprite_setpos(childspr,spr->screenX,spr->screenY);
	childspr->localx = 0;
	childspr->localy = 0;
	//if(childspr->pos_z<spr->pos_z){
	//	sprite_set_z(childspr,ex_newPosZ());
	//	printf("====================>%0x修改层级\n",childspr);
	//}
	if(f_isHasChild(spr,(int)child)){
		//printf("is exist!!!\n");
		return;
	}
	
	//设置相对坐标
	LStack_push(spr->childs,child);
}

/*移除子对象*/
void sprite_removeChild(void* spr,void* child){
	struct Sprite* childspr = (struct Sprite* )child;
	struct Sprite* parent = (struct Sprite* )spr;
	childspr->parent = 0;
	if(LStack_delNode(parent->childs,(int)child)){
		log_color(0,"(parent: %s child: %s)	sprite_removeChild 成功!\n",f_get_name(spr),f_get_name(child));
	}else{
		//log_color(0xff0000,"(parent: %s child: %s)	sprite_removeChild 失败!\n",f_get_name(spr),f_get_name(child));
		log_color(0xff0000,"%s ===>%0x sprite_removeChild fail,父对象%0x已经被dispose掉了\n",f_get_name(child),child,spr);
	}
}

/*
 *刷新子对象列表的sprite
 */
static void f_refreshChildPos(void* ptr){
	struct Sprite* pp = (struct Sprite* )ptr;
	struct LStackNode* s = (struct LStackNode* )pp->childs;
	
	void* top,*p;
	if(!pp->childs)	
		return;

	top = s;
	p=top;

	while((int)LStack_next(p)){
		int data=0;
		struct Sprite* tpp=0;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		tpp=(struct Sprite*)data;
		sprite_setpos(tpp,pp->screenX + tpp->localx,pp->screenY+tpp->localy);
	}
}

////设置坐标，相对于局部坐标
//void sprite_setLocalPos(void* ptr,int x,int y){
//	//struct Sprite* p = (struct Sprite* )ptr;
//	//
//	//if(p->parent){
//	//	struct Sprite* par = (struct Sprite* )p->parent;
//	//	//p->localx = x;
//	//	//p->localy = y;
//	//	//sprite_setpos(p,par->screenX+x,par->screenX+y);
//	//	sprite_setpos(p,x,y);
//	//}else{
//	//	//p->localx = x;
//	//	//p->localy = y;
//	//	sprite_setpos(p,x,y);
//	//}
//	//f_refreshChildPos(ptr);
//	sprite_setpos(ptr,x,y);
//}

void
sprite_rotateX(struct Sprite* ptr,float rx){
	struct HeadInfo* b = (struct HeadInfo*)ptr->base;
	b->rx = rx;
	updateMatrix(ptr);
}
void
sprite_rotateY(struct Sprite* ptr,float ry){
	struct HeadInfo* b = (struct HeadInfo*)ptr->base;
	b->ry = ry;
	updateMatrix(ptr);
}
void
sprite_rotateZ(struct Sprite* ptr,float rz){
	struct HeadInfo* b = (struct HeadInfo*)ptr->base;
	b->rz = rz;
	updateMatrix(ptr);
}
void 
sprite_set_scale_z(struct Sprite* spr,float v){
	struct HeadInfo* base = spr->base;
	if(getv(&base->flags,FLAGS_BUTTON_EFFECT)){
		printf("设置了按钮点击缩放特效,无法使用该接口\n");
		assert(0);
	}
	spr->zScale = v;
}
void
sprite_bindAtals(void* p,void* atals){
	struct Sprite* sprite = (struct Sprite*)p;
	//设置图集
	sprite->atals = (struct Atals*)atals;
}

void
sprite_set_hit_rect(void*p,int x,int y,int w,int h){
	struct Sprite* ptr = (struct Sprite* )p;
	ptr->hitX = x,	ptr->hitY = y, ptr->hitWidth = w,ptr->hitHeight = h;
	setHitTriangle(ptr);
}

void
sprite_set_grid9(void* ptr,float left,float right,float top,float bottom,float w,float h){
	struct Grid9Node* grid9 = 0;
	struct Sprite *_sprite = (struct Sprite *)ptr;
	if(!_sprite->grid9){
		_sprite->grid9 = (struct Grid9Node*)tl_malloc(sizeof(struct Grid9Node));
		memset(_sprite->grid9,0,sizeof(struct Grid9Node));
	}
	grid9 = _sprite->grid9;
	grid9->left = left;
	grid9->right = right;
	grid9->top = top;
	grid9->bottom = bottom;
	grid9->width = w;
	grid9->height = h;
	grid9->sx = _sprite->mWidth / w;
	grid9->sy = _sprite->mHeight / h;
}



