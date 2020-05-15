#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

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
#include "spritehit.h"

#define USE_VBO	//�Ƿ�ʹ��VBOģʽ

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


/* chiid�Ƿ���parent��*/
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

//������������������Ҫ�����ݴ�С(Glfloat���ݵĸ���)
enum {
	//uv vertex
	SPRITE_VERTEX_COUNT_30=30,

	//uv normal vertex
	SPRITE_VERTEX_COUNT_48=48,
	
	//uv��ҪGLfloat����
	UV_COUNT_6 = 6,

	UV_COUNT_4 = 4,
};

//�����������
enum
{
	Type_6Vertex = 1,
	Type_4Vertex = 2
};

/*
 *��ȡ������0��λ�õ�����
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
	 ������
*/
#define  VBO_VERTEX_SPLINE 5	//u v x y z
//��ӡuv GL_T2F_N3F_V3F
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
 *	float * v Ϊ sizeof(float) * 12 = 48 bytes
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
	����sprite��UV,����VBO����glBufferSubData����VBO����
*/
static void f_sprite_setUV(struct Sprite* sprite,float x,float y,float w,float h)
{
	//int i=0;
	//����a,b����תUV
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
	//return;//�����������õ�

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
		//VBOģʽ
		struct ObjVBO* vbo=getvbo(sprite);
		if(vbo->uvID==0)
		{
			printf("vbo->uvID = 0\n");
			assert(0);
		}
		else
		{
			float* __ptr;

			//Specifies the size in bytes of the data store region being replaced.(�������ڴ���ֽڳߴ�)
			int _bytesSize;

			if(sprite->parseType == Type_6Vertex)
			{
				float ptr[UV_COUNT_6*UV_GAP];
				
				//6��UV�������� * 2(һ��������Ҫ��uv����) * һ��float��ռ�õ��ֽڴ�С
				_bytesSize = UV_COUNT_6*UV_GAP*sizeof(float);
				
				SetUV_6Vertex(ptr,UV_GAP,a1,a2,a3,a4);
				__ptr = ptr;
			}
			else if(sprite->parseType == Type_4Vertex)
			{
				float ptr[UV_COUNT_4*UV_GAP];

				//4��UV�������� * 2(һ��������Ҫ��uv����) * һ��float��ռ�õ��ֽڴ�С
				_bytesSize = UV_COUNT_4*UV_GAP*sizeof(float);
				SetUV_4Vertex(ptr,UV_GAP,a1,a2,a3,a4);
				__ptr = ptr;
			}
			
			//����UV��VBO
			glBindBuffer(GL_ARRAY_BUFFER,vbo->uvID);
			glBufferSubData(GL_ARRAY_BUFFER,0,_bytesSize,__ptr);
		}
	}
}

//����һ���ı�����Ƭ,����һ�黺��������,�洢��������
static GLfloat* 
LoadQuadObj(int* ptrVertexLength)
{
/*
	struct EX* ex=ex_getInstance();
	int length = ex->spriteVert.vertLen;
	GLfloat* vertsPtr ;

	*ptrVertexLength = length / VBO_VERTEX_SPLINE;
	
	//copy��������
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

//static char* _gobj_str = 0;//����������

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
		//printf("����_gobj_str %0x\n",_objStr);
	}else{
		_objStr = tl_loadfile(path,0);
		map_set(ex_getIns()->mapptr,path,_objStr);
	}
	/*if(_gobj_str == 0){
		_objStr=tl_loadfile("\\resource\\obj\\quad.obj",0);
		_gobj_str = _objStr;
	}
	else{
		printf("����_gobj_str %0x\n",_gobj_str);
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
 *����VBO
 * uv normal verter��uv vertex��֧��
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


//
///*
//	���ò㼶
//*/
//static void
//setLayer(struct Sprite* sprite,int layer)
//{
//	sprite->pos_z = layer;
//}

//���õ�ǰ��sprite��2dcam,��2dcam��ʱ����
static void
f_sprite_set2dCam(void* spr,void* _2dcam){
	/*struct Sprite* sprite = (struct Sprite*)spr;
	sprite->_2dcam = _2dcam;*/
	base_set_cam(spr,_2dcam);
}


/*
	����sprite����Ļ����,(��Ļ�����Ͻ�Ϊ0,0��ʼ��)
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
		
		printf("��������%s\t%.3f,%.3f\t%.3f,%.3f x = %f y = %f,�Ƿ���Ĭ�ϵ�cam %d,screenw=%f,screenh=%f\n",
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
	����sprite�ĳߴ�
*/
void sprite_resize(struct Sprite* spr,int w,int h){
	spr->mWidth = w;
	spr->mHeight = h;
	sprite_setpos(spr,spr->screenX,spr->screenY);
}

static void f_sprite_set_width(struct Sprite* spr,int w){
	spr->mWidth = w;
	sprite_setpos(spr,spr->screenX,spr->screenY);
}

static void 
f_sprite_set_height(struct Sprite* spr,int h){
	spr->mHeight = h;
	sprite_setpos(spr,spr->screenX,spr->screenY);
}

/*
 *��addChild֮��������������
 *�����sprite��������sprite���Ӷ����ʱ��,ʹ�øýӿ�����������ڸ����������
 */
static void f_sprite_set_self_pos(void* p,int x,int y){
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

//���þ�������
void sprite_setpos(struct Sprite* spr,int x,int y){
	//printf("%d,%d\n",x,y);
	
	//evt_once(NextFrame)����һ���ؼ�֡��ʱ���������,�������ᶶ��
	
	setSpriteScreenPos(spr,x,y);//������Ļ����
	spritehit_hitTriangle(spr->pHit,spr->screenX,spr->screenY);//���µ������
	f_refreshLocalPos(spr);
	f_refreshChildPos(spr);//����sprite���Ӷ��������
}

static void 
InitType(struct Sprite* pSpr)
{
	//pSpr->useVBO = 1;
#ifdef USE_VBO
		pSpr->parseType = Type_4Vertex;//Type_4Vertex,Type_6Vertex;
		pSpr->vbo=load_vbo(pSpr->parseType);//ʹ��VBO
		//printf("pSpr->vbo = %0x\n",pSpr->vbo);
#else
		pSpr->vertexs = LoadQuadObj(&pSpr->vertLen);//���ض�������(��VBOʵ��)
#endif
}
/*
	�Ƿ�������¼�,�лص����˵����ʰȡ�¼�
*/
static int 
f_sprite_isCanClick(void* n){
	struct Sprite* pSpr = (struct Sprite*)n;
	return (int)pSpr->clickCallBack!=0;
}

void
sprite_set_clickHandle(void* p,void (*clickCallBack)(void* ,int ,int )){
	struct Sprite* pSpr = (struct Sprite*)p;
	pSpr->clickCallBack = clickCallBack;
}
void*
sprite_createEmptyTex(int texW,int texH,void* _2dCam){
	struct Sprite* spr = 0;
	void* mat= tmat_create_empty("simple;fbotex");//fbotex

	char buffer[64];
	tl_newName(buffer,64,"sprite");
	tmat_setID(mat,1);

	//tmat_pushTex(mat,(GLuint)mirrorTexture);		//void* mirrorTexture,

	spr = sprite_create(buffer,0,0,texW,texH,0,_2dCam);
	//sprite_rotateZ(spr,-PI/2);//sprite��ת90��
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
	//��ʼ����ť
	struct Sprite* pSpr =	(struct Sprite*)tl_malloc(sizeof(struct Sprite));//new Button;
	struct SpriteHit* pHit = (struct SpriteHit*)tl_malloc(sizeof(struct SpriteHit));
	memset(pHit,0,sizeof(struct SpriteHit));
	
	//log_color(0,"sprite_create name=(%s)\n",_spriteName);

	memset(pSpr,0,sizeof(struct Sprite));
	
	pSpr->pHit = pHit;



	pSpr->m_bPressed = 0;
	//pSpr->_renderState = 1;

	pSpr->parent = 0;

	pSpr->mWidth  = width;
	pSpr->mHeight = height;
	
	pSpr->pHit->hitTriangle = tl_malloc(sizeof(float)*SPRITE_TRIANGLE_COUNT);
	pSpr->pHit->hitWidth = width;
	pSpr->pHit->hitHeight = height;
	//printf("pSpr = %0x\n",pSpr);
	//���ö�����֯��ʽ
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
	//base->parent = pSpr;
	if(base){
		//setv(&base->flags,FLAGS_VISIBLE);
		base_set_visible(base,1);
		pSpr->clickCallBack = clickCallBack;
		//ex_add(pSpr);
	}
	//pSpr->pos_z = ex_newPosZ();//�˴�������z��ֵ,��ex_add��ʱ��������
	pSpr->zScale = 1.0;
	f_sprite_setUV(pSpr,0,0,1,1);//��ʼ������UV
	

	
	return pSpr;
}

//����sprite��z���ֵ
static void
f_sprite_set_z(void* p,float v){
	struct Sprite* pSpr = (struct Sprite*)p;
	pSpr->pos_z = v;
	//printf("z = %.3f\n",pSpr->pos_z);
}
/*
	����sprite���϶���Χ
	x,y:����ק������
	w:����ק�Ŀ��
	h:����ק�ĸ߶�
*/
static void 
f_sprite_setDragScope(struct Sprite* pSpr,int x,int y,int w,int h)
{
	struct HeadInfo* base = base_get((void*)pSpr);

	if(pSpr->mWidth>w ||pSpr->mHeight>h)
	{
		log_color(0xff0000,"������������̫Сsprite����Ļ���ص�λ(mWidth = %d,��ק����w=%d)(mHeight = %d,��ק����h=%d)\n",(int)pSpr->mWidth,w,(int)pSpr->mHeight,h);
		//assert(0);
		return;
	}

	log_color(0,"Sprite name=[%s]����FLAGS_DRAG\n",base->name);

	setv(&base->flags,FLAGS_DRAG);
	/*pSpr->oldx = pSpr->screenX;
	pSpr->oldy = pSpr->screenY;
	pSpr->dragX = x;
	pSpr->dragY = y;
	pSpr->dragWidth = w;
	pSpr->dragHeight = h;*/
	spritehit_setData(pSpr->pHit,x,y,w,h,pSpr->screenX,pSpr->screenY);
}

/*
	�Ƿ������Ⱦ
*/
static int 
f_sprite_isEnable(int data)
{
	int objType;
	struct HeadInfo* base = base_get((void*)data);
	
	objType = base->curType;

	if(objType == TYPE_SPRITE_FLIE /*&& getv(&(base->flags),FLAGS_VISIBLE)*/)
	{
		return 1;
	}
	return 0;
}

/*
	����sprite����
*/
static void
updateSpriteMat4x4(struct Sprite* p,
				   float x,float y,float z,
				   float sx,float sy,float sz)
{

	struct HeadInfo* base = p->base;
	void* cam = base->cam;
	Matrix44f xyz,m_scale, result,tmp2,tmp3;
	mat4x4_identity(p->mat4x4);
	mat4x4_identity(result);
	mat4x4_identity(tmp2);
	mat4x4_identity(tmp3);

	//x,y,z
	mat4x4_identity(xyz);
	mat4x4_translate(xyz,x,y,z);

	//scale
	mat4x4_identity(m_scale);
	mat4x4_scale(m_scale,sx,sy,sz);

	{
		Matrix44f am;
		mat4x4_rotate_vec(am,base->angle,base->ax,base->ay,base->az);
		mat4x4_mult(5,p->mat4x4,
			cam_getPerctive(cam),
			cam_getModel(cam),
			xyz,
			am,
			m_scale);
		
		//mat4x4_printf(base->name,p->mat4x4);
	}
}
//void
//sprite_set_default_tex(void* ptr){
//	struct Sprite* p = (struct Sprite*)ptr;
//	//ex_setv(p,FLAGS_DRAW_PLOYGON_LINE);
//	p->atals = ex_get_ui_atals();
//	sprite_texName(p,"gundi.png",0);
//}
//��ȡSprite���õĲ�������,
//���Դ�ͼ���л�ȡ,Ҳ�����Զ����������(��ͼ����ͼ��,��ͼ��������Ĳ���)
static void* 
getMaterial(struct Sprite* p){
	/*
	//����fbo
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
	//printf("getMaterial %s ,û������ͼ��,Ҳû��Ĭ�ϲ���\n",head->name);
	//assert(0);//������Ҫ��û�����ò��ʵĶ�������޸�!!!
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

//��Ⱦһ���ı���
static void 
renderSprite(struct Sprite* p)
{
	void* material = getMaterial(p);//p->atals->material;

	if(material){
		//void* material = getMaterial(p);//p->atals->material;
		struct HeadInfo* base = base_get(p);
		char* shaderName = 0;



		if(!material){
			printf("(%s)Spriteû�и������\n",((struct HeadInfo*)p->base)->name);
			assert(0);
		}
		
		
#ifdef USE_VBO
			//��Ⱦ���߲���vboģʽ
			objVBO_renderNode(
				getvbo(p),
				material,
				&p->mat4x4,
				base->flags,
				0,
				base->cam
				);
#endif
		
	}
}

/*
 *���¾���
 */
static void 
updateMatrix(struct Sprite* p)
{
	struct HeadInfo* b = p->base;
	//p->zScale = 1.5;
	float sc = p->zScale;
	updateSpriteMat4x4(p,b->x,b->y,b->z,p->mWidth*sc,p->mHeight*sc,sc);
}

/*
#define  _Time_Delay_  10//������ӳ�ʱ��(����)		10����ָ�״̬
//��ť��Ч
static void
f_btn_push(struct Sprite* spr,int* pChange){
	//struct HeadInfo* base = spr->base;
	if(spr->m_bPressed )
	{
		float targetScale = 0.95;

		if(spr->zScale!=targetScale)
		{
			//��������һ��

			spr->zScale = targetScale;//���ʱ,��ť��߱ȱ仯
			//printf("���һֱ���Ű�ťSprite %f,%f\n",pBtn->m_fWidth,pBtn->m_fHeight);
			spr->btntime=get_longTime()+_Time_Delay_;
			*pChange = 1;
		}
	}
	else
	{
		spr->zScale = 1.0;

		if(spr->btntime!=0 && spr->btntime - get_longTime()<=0)
		{
			//_Time_Delay_����֮��ᴦ��
			*pChange = 1;
			spr->btntime = 0;
		}
	}
}
*/
//void*
//sprite_get_material(void* sprite){
//	return getMaterial((struct Sprite*)sprite);
//}

/*
	����һ��sprite 
*/
void 
sprite_drawRender(int data)
{
	//struct EX* e = ex_getIns();

	struct Sprite* spr = (struct Sprite*)data;
	float x = spr->pos_x+spr->mWidth/2;
	float y = spr->pos_y+spr->mHeight/2;
	float z = spr->pos_z;//-2.0ֻ��Ϊ�˰�ť�ɼ�,��������������ֵ
	
	//////////////////////////////////////////////////////////////////////////
	//isChange == 1��ʱ��Ż���¾���
	int isChange = 0;

	struct HeadInfo* base = spr->base;
	
	if(!f_sprite_isEnable((int)spr))
	{
		return;
	}
	if(!getMaterial(spr)){
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//λ�Ʒ����仯,���¾���
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
	
	//if(getv(&base->flags,FLAGS_BUTTON_EFFECT)){
	//	f_btn_push(spr,&isChange);//��ť��Ч����,��Ҫ�Ƶ�ҵ�������!!!
	//}
	if(spr->m_bPressed)
	{
		//�����갴�µ�ʱ��,��һֱִ�������߼�
		ex_lua_evt_dispatch(spr,EVENT_ENGINE_SPRITE_CLICK_DOWN,base->name);
	}else{
	}
	if(isChange)
	{
		updateMatrix(spr);
	}

	renderSprite(spr);
}

///*
//	�Ƿ���������ק��Χ����
//*/
//static int
//haveDragScope(struct Sprite* ptr){
//	//if(ptr->m_fPosX+ptr->dragWidth - ptr->m_fWidth)
//	if(ptr->dragWidth || ptr->dragHeight)
//	{
//		return 1;
//	}
//	return 0;
//}
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

//========================================================
//#define _ZERO_


//����ק�ؼ��Ŀ�
static float 
GetWidth(struct Sprite* p)
{
#ifdef _ZERO_
	return 0.0f;
#endif
	return p->mWidth;
}

//����ק�ؼ��ĸ�
static float 
GetHeight(struct Sprite* p)
{
#ifdef _ZERO_
	return 0.0f;
#endif
	return p->mHeight;
}

//p�ĸ��ڵ�ľ�������
static void
f_get_global(struct Sprite* p,int* px,int* py){
	if(p->parent){
		 
		struct Sprite* ps = (struct Sprite*)p->parent;

		//if(ps->parent){
			*px = ps->screenX;//-ps->localx;
			*py = ps->screenY;//-ps->localy;
		//}else{
		//}
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
//���ScrollBar ScrollBarBG_Click�ӿ�
	if(spritehit_haveDragScope(p->pHit)){
		//int cx0,cx1,cy0,cy1,oldx,oldy;
		float progress;

		int ppx=0,ppy=0;

		f_get_global(p,&ppx,&ppy);

		spritehit_changeDragXY(p->pHit,GetWidth(p),GetHeight(p),
			ppx,ppy,p->screenX,p->screenY,
			px,py,&progress);


		 //�����¼���lua
		ex_lua_evt_dispatch_f(p,EVENT_ENGINE_SPRITE_CLICK_MOVE,progress);
	}
}
//========================================================
void 
sprite_mouseMove(int data)
{
	struct EX* e = ex_getIns();	
	if(f_sprite_isEnable(data))
	{
		struct HeadInfo* base = base_get((void*)data);

		if(getv(&base->flags,FLAGS_DRAG))
		{
			struct Sprite* ptr = (struct Sprite*)data;
			
			//printf("c = %s,y = %s\n",base_get(ex_getIns()->curFocus)->name,base_get(ptr)->name);
			if(ex_getIns()->curFocus == ptr){
				if(ptr->m_bPressed){
					//��갴��,ֻ����������귢���仯��ʱ��				
					int x = e->mouseState.moveX - ptr->mouseDownX;
					int y = e->mouseState.moveY - ptr->mouseDownY;

					//x,y ��ǰ���ʰȡ���ľ�������
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
	��갴��
*/
void sprite_action(const int data)
{
	if(f_sprite_isEnable(data))
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
					
					

					//������£�
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

//��Ⱦһ����Ͷsprite
void sprite_updatePos(int data)
{
	if(f_sprite_isEnable(data))
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
	����sprite
*/
void sprite_dipose(struct Sprite* spr){
	f_sprite_removeSelf((void*)spr);
	if (spr->childs){
		LStack_delete(spr->childs);
		spr->childs = 0;
	}

	//spr->ptr_luaCallBack = 0;
	if(spr->pHit->hitTriangle)
		tl_free(spr->pHit->hitTriangle);

	if(spr->vertexs)
		tl_free(spr->vertexs);
	
	//ɾ�����ʾ��
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
	//if (spr->grid9){
	//	tl_free(spr->grid9);
	//}

	if(spr->vbo)
	{
		objVBO_dispose(spr->vbo);
	}
	LStack_delNode(ex_getIns()->renderList,(int)spr);//����Ⱦ�ڵ��б����Ƴ�

	spritehit_dispose(spr->pHit);

	tl_free(spr);
}

/*
	Ϊsprite����ͼ��
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
	//��ȡͼ������,��䵽ptrNode������
		atals_tex(ptr->atals,texName,ptrNode);

		width = ptr->atals->width;
		height = ptr->atals->height;
	}
	else
	{
		log_code(ERROR_NOT_EXIST_Atals);
		printf("��spriteû��ͼ��Atals��ʱ��,�޷�����sprite_texName�ӿ�!\n");
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
	printf("�޸�UV,%s:%.3f,%.3f,%.3f,%.3f",texName,
		1.0f-(height-n.y)/height,
		1.0f-(width-n.x)/width,
		n.height/height,
		n.width/width
		);
#endif
	
	//����ͼ��UV.
	f_sprite_setUV(ptr,		
		1.0f-(height-n.y)/height,
		1.0f-(width-n.x)/width,
		n.height/height,
		n.width/width
		); 

	

	if (_pNode!=0){
		memcpy(_pNode,ptrNode,sizeof(struct AtlasNode));
	}
}

//����->childsջ
static void
f_init_childsContner(void* p){
	struct Sprite* spr = (struct Sprite* )p;
	if(!spr->childs){
		spr->childs = LStack_create();
	}
}

/*
 *Ϊspr���һ���Ӷ���
 **/
void sprite_addChild(void* p,void* child){
	struct Sprite* spr = (struct Sprite* )p;
	struct Sprite* childspr = (struct Sprite* )child;
	childspr->parent = p;//child;		//���ø���������
	if(!spr->parent){
		f_init_childsContner(p);
	}
	sprite_setpos(childspr,spr->screenX,spr->screenY);
	childspr->localx = 0;
	childspr->localy = 0;
	//if(childspr->pos_z<spr->pos_z){
	//	sprite_set_z(childspr,ex_newPosZ());
	//	printf("====================>%0x�޸Ĳ㼶\n",childspr);
	//}
	if(f_isHasChild(spr,(int)child)){
		//printf("is exist!!!\n");
		return;
	}
	LStack_push(spr->childs,child);
}

/*�Ƴ��Ӷ���*/
void sprite_removeChild(void* spr,void* child){
	struct Sprite* childspr = (struct Sprite* )child;
	struct Sprite* parent = (struct Sprite* )spr;
	childspr->parent = 0;
	if(LStack_delNode(parent->childs,(int)child)){
		log_color(0,"(parent: %s child: %s)	sprite_removeChild �ɹ�!\n",f_get_name(spr),f_get_name(child));
	}else{
		//log_color(0xff0000,"(parent: %s child: %s)	sprite_removeChild ʧ��!\n",f_get_name(spr),f_get_name(child));
		log_color(0xff0000,"%s ===>%0x <warnning> sprite_removeChild fail,������%0x�Ѿ���dispose����\n",f_get_name(child),child,spr);
	}
}

/*
 *ˢ���Ӷ����б��sprite
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

////�������꣬����ھֲ�����
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

static void
f_sprite_rotateX(struct Sprite* ptr,float rx){
	struct HeadInfo* b = (struct HeadInfo*)ptr->base;
	//b->rx = rx;
	base_rotate_vec(b,1,0,0,rx);
	updateMatrix(ptr);
}
//��תrz�� PI��n����,ֻ�任��Ⱦ����
static void
f_sprite_rotateY(struct Sprite* ptr,float ry){
	struct HeadInfo* b = (struct HeadInfo*)ptr->base;
	//b->ry = ry;
	base_rotate_vec(b,0,1,0,ry);
	updateMatrix(ptr);
}
static void
f_sprite_rotateZ(struct Sprite* ptr,float rz){
	struct HeadInfo* b = (struct HeadInfo*)ptr->base;
	//b->rz = rz;
	base_rotate_vec(b,0,0,1,rz);
	updateMatrix(ptr);
}
/*����Z����  */
static void 
f_sprite_set_scale_z(struct Sprite* spr,float v){
	struct HeadInfo* base = spr->base;
	//if(getv(&base->flags,FLAGS_BUTTON_EFFECT)){
	//	printf("�����˰�ť���������Ч,�޷�ʹ�øýӿ�\n");
	//	assert(0);
	//}
	spr->zScale = v;
}

//����ͼ��
static void
f_sprite_bindAtals(void* p,void* atals){
	struct Sprite* sprite = (struct Sprite*)p;
	//����ͼ��
	sprite->atals = (struct Atals*)atals;
}
/*
 *����һ���������ķ�Χ
 */
static void
f_sprite_set_hit_rect(void*p,int x,int y,int w,int h){
	struct Sprite* pp = (struct Sprite* )p;
	struct SpriteHit* ptr = pp->pHit;
	ptr->hitX = x,	ptr->hitY = y, ptr->hitWidth = w,ptr->hitHeight = h;
	//setHitTriangle(ptr);
	spritehit_hitTriangle(pp->pHit,pp->screenX,pp->screenY);
}
//���þŹ�������
static void
f_sprite_set_grid9(void* ptr,float left,float right,float top,float bottom,float w,float h){
	struct Grid9Node* grid9 = 0;
	struct Sprite *_sprite = (struct Sprite *)ptr;
	
	struct GMaterial* tmat = getMaterial(_sprite);
	if(tmat){
		if(!tmat->params){
			tmat->params = tl_malloc(sizeof(struct Grid9Node));
			memset(tmat->params,0,sizeof(struct Grid9Node));
		}
		grid9 = tmat->params;

		grid9->left = left;
		grid9->right = right;
		grid9->top = top;
		grid9->bottom = bottom;
		grid9->width = w;
		grid9->height = h;
		grid9->sx = _sprite->mWidth / w;
		grid9->sy = _sprite->mHeight / h;
	}else{
		assert(0);
	}
}
int sprite_get(void* ptr,int flag){
	if(flag == SPRITE_IS_ENABLE){
		return f_sprite_isEnable((int)ptr);
	}else if(flag == SPRITE_MOUSE_ENABLE){
		return f_sprite_isCanClick(ptr);
	}else if(flag == SPRITE_MATERIAL){
		return (int)getMaterial((struct Sprite*)ptr);//��ȡsprite�Ĳ��ʾ��
	}
	return 0;
}
void sprite_set(void* ptr,int flag,...){
	//int flag;
	va_list ap;
	va_start(ap, flag);
	//flag = va_arg(ap, int);
	if(flag == SPRITE_Z){
		float v0;
		//printf("==============>%d\n",flag);
		v0 = va_arg(ap, double);
		//printf("==============>v0 = %.3f\n",v0);

		f_sprite_set_z(ptr,v0);
	}
	else if(flag == SPRITE_SELF_XY){
		float x,y;
		x = va_arg(ap, double);
		y = va_arg(ap, double);
		f_sprite_set_self_pos(ptr,x,y);
		//printf("==============>%.3f %.3f\n",x,y);
	}
	else if(flag == SPRITE_GRID){
		int left,right,top,bottom;
		float w,h;
		left = va_arg(ap, int);
		right = va_arg(ap, int);
		top = va_arg(ap, int);
		bottom = va_arg(ap, int);
		w = va_arg(ap, double);
		h = va_arg(ap, double);
		f_sprite_set_grid9(ptr,left,right,top,bottom,w,h);
	}
	else if(flag == SPRITE_ATALS){
		int a = va_arg(ap, int);
		f_sprite_bindAtals(ptr,(void*)a);
	}
	else if(flag == SPRITE_2DCAM){
		int a = va_arg(ap, int);
		f_sprite_set2dCam(ptr,(void*)a);
	}
	else if(flag == SPRITE_WIDTH){
		int w = va_arg(ap, double);
		//printf("w = %d\n",w);
		f_sprite_set_width(ptr,w);
	}
	else if(flag == SPRITE_HEIGHT){
		int h = va_arg(ap, double);
		//printf("h = %d\n",h);
		f_sprite_set_height(ptr,h);
	}
	else if(flag == SPRITE_DRAG){
		int x = va_arg(ap, int);
		int y = va_arg(ap, int);
		int w = va_arg(ap, int);
		int h = va_arg(ap, int);
		f_sprite_setDragScope((struct Sprite*)ptr,x,y,w,h);
		//printf("==============>%d %d %d %d\n",x,y,w,h);	
	}
	else if(flag == SPRITE_RX){
		float v = va_arg(ap, double);
		f_sprite_rotateX((struct Sprite*)ptr,v);
	}
	else if(flag == SPRITE_RY){
		float v = va_arg(ap, double);
		f_sprite_rotateY((struct Sprite*)ptr,v);
	}
	else if(flag == SPRITE_RZ){
		float v = va_arg(ap, double);
		f_sprite_rotateZ((struct Sprite*)ptr,v);
	}
	else if(flag == SPRITE_SCALE_Z){
		float v = va_arg(ap, double);
		f_sprite_set_scale_z((struct Sprite*)ptr,v);	
	}
	else if(flag == SPRITE_UV){
		float x = va_arg(ap, double);
		float y = va_arg(ap, double);
		float w = va_arg(ap, double);
		float h = va_arg(ap, double);
		f_sprite_setUV((struct Sprite*)ptr,x,y,w,h);
	}
	else if(flag == SPRITE_HIT_RECT){
		int x1 = va_arg(ap, int);
		int y1 = va_arg(ap, int);
		int w1 = va_arg(ap, int);
		int h1 = va_arg(ap, int);
		f_sprite_set_hit_rect(ptr,x1,y1,w1,h1);
	}
	va_end(ap);
}
