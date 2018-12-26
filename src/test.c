
//18:04 2018/12/26 星期三
////窗口宽高
//#define _WIDTH_		1200
//
//#define _HEIGHT_	450
//主程序根目录

#define _MAIN_PATH_ "d:\\gl_source"

//class Box
//{
//public:
//	double length;   // Length of a box
//	double breadth;  // Breadth of a box
//	double height;   // Height of a box
//};

//
////warning C4005: '' : macro redefinition	重复定义的宏
//
////warning C4311: 'type cast'	类型转化

//#pragma comment(lib,"my.lib")

//用默认的MALLOC会引起相关的bug,谨慎用之


#include <windows.h>
#include <stdio.h>
#include <assert.h>

//#include <conio.h>

#pragma comment (lib,"lua5.1.lib")
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

//=============================================
#include "tools.h"
#include "gettime.h"

#include "ex.h"
#include "tmat.h"

#include "md5.h"
#include "jgl.h"
#include "atlas.h"

#include "sprite.h"

#include "obj_vbo.h"

#include "node.h"
#include "animtor.h"
#include "tl_malloc.h"
//=============================================


















static void multChange(struct HeadInfo* base);




//===============================================================
//#include "lua.h"  
//#include "lauxlib.h"  
//#include "lualib.h"  







//#include "stdarg.h" 
//void simple_va_fun(int start, ...) 
//{  
//	va_list arg_ptr;
//	int nArgValue =start; 
//	int nArgCout="0";  //可变参数的数目 
//	va_start(arg_ptr,start);  //以固定参数的地址为起点确定变参的内存起始地址。 
//	do 
//	{ 
//		++nArgCout; 
//		printf("the %d th arg: %d",nArgCout,nArgValue); //输出各参数的值 
//		nArgValue = va_arg(arg_ptr,int);  //得到下一个可变参数的值 
//	} while(nArgValue != -1);
//	return; 
//}








#ifndef _SCENE_H_
#define _SCENE_H_
struct Scene{
	/*
	引擎引用
	*/
	struct EX* enginePtr;
	/*
	怪物配置
	*/
	struct LStackNode* monsterCfgList;

	struct XMLSList* monster_xml;
	/*
	主角
	*/
	struct HeadInfo* hero;

	//测试立方体
	struct HeadInfo* box;

	/*
		测试用的md5模型
	*/
	struct MD5* md5;
};
/*
	角色配置
*/
struct AvatarCfg
{
	char* id;
	char* name;
	char* path;
	char* tex;

	//模型路径
	char* mesh;

	//动作配置
	char* anim;

	//模型类型
	int type;

	//动作的fps
	float fps;

	//默认动作
	char* defaultAnim;

	//模型描述
	char* desc;

	//是否可以射线拾取
	int ray;
};


//引擎引用
//EngineX* engine;
struct EX* ex=NULL;

struct Scene* scene=NULL;

struct Sprite *spr;

//void* uimat;//界面材质




//struct Atals* myAtals;//图集

/*
创建场景
*/
struct Scene* scene_create(struct EX* ex);
/*
销毁场景
*/
void scene_dispose(struct Scene* scene);
/*
加载怪物配置
*/
void scene_loadAvtCfg(struct Scene* scene,const char* path);

/*
	按键回掉
*/
void scene_specialKeys(struct Scene* p,int key, int x, int y);
#endif
static int isEqualId(struct AvatarCfg* cfg,const char* _str){
	return !strcmp(cfg->id,_str);
}
/*
	根据xmlnode创建一个cfg数据结点
*/
static struct AvatarCfg* avatarCfg_create(struct XmlNode* node){
	struct AvatarCfg* cfg = (struct AvatarCfg*)tl_malloc(sizeof(struct AvatarCfg));
	memset(cfg,0,sizeof(struct AvatarCfg));
	cfg->id = xml_getCopystr(node,"id");
	cfg->path = xml_getCopystr(node,"path");
	cfg->tex = xml_getCopystr(node,"tex");
	cfg->name = xml_getCopystr(node,"name");
	cfg->type = xml_getint(node,"type");
	cfg->ray = xml_getint(node,"ray");
	cfg->anim = xml_getCopystr(node,"anim");
	cfg->mesh = xml_getCopystr(node,"mesh");
	cfg->defaultAnim = xml_getCopystr(node,"defaultAnim");
	cfg->desc = xml_getCopystr(node,"desc");
	cfg->fps = xml_getfloat(node,"fps");
	//show();
	//printf("=================================\n");
	//printf("id:%s 模型描述:%s\n",this->id,this->desc);
	return cfg;
}
/*
	销毁配置
*/
static void avatarCfg_dispose( struct AvatarCfg* cfg ){
	printf("%s MonsterCfg dispose\n",cfg->id);
	tl_free(cfg->id);
	tl_free(cfg->tex);
	tl_free(cfg->path);
	tl_free(cfg->name);
	tl_free(cfg->anim);
	tl_free(cfg->mesh);
	tl_free(cfg->defaultAnim);
	tl_free(cfg->desc);
	tl_free(cfg);
	cfg=NULL;
}


void initMonsterListCfg(struct Scene* scene,struct XMLSList* monster_xml){

	void* _mlist = monster_xml->list;
	struct LStackNode* s = (struct LStackNode* )_mlist;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		struct XmlNode* node;
		struct AvatarCfg* _cfg;

		p=(void*)LStack_next(p);
		data = LStack_data(p);
		node = (struct XmlNode*)data;
		if(!scene->monsterCfgList){
			scene->monsterCfgList=LStack_create();
		}

		_cfg = avatarCfg_create(node);
		LStack_push(scene->monsterCfgList,(int)_cfg);
	}
}
void scene_loadAvtCfg(struct Scene* scene,const char* path){

	struct	EX* ex = scene->enginePtr;

	struct XMLSList* monster_xml = xml_parse(tl_loadfile(path,NULL));
	scene->monster_xml = monster_xml;
	scene->monsterCfgList = NULL;
	//初始化怪物配置表
	initMonsterListCfg(scene,monster_xml);
}

struct Scene* scene_create(struct EX* ex){
	struct Scene* s = (struct Scene*)tl_malloc(sizeof(struct Scene));
	memset(s,0,sizeof(struct Scene));
	
	//初始化camera的坐标
	//setCamPos(0.0,0.0f,3950.0f);//30.0
	
	//setCamPos(0.0,0.0f,30.0f);//30.0

	//启用射线拾取碰撞检测
	//setRayCollision(ex,TRUE);
	setv(&(ex->flags),EX_FLAGS_RAY);

	//设置背景
	setBgColor(0.5f,0.5f,0.5f);

	s->enginePtr = ex;
	return s;
}
////切换线框渲染
//static void f_switchBoxLine()
//{
//	if(scene->box){
//		int* _flags = &(scene->box->flags);
//		if(getv(_flags,FLAGS_LINE_RENDER)){
//			resetv(_flags,FLAGS_LINE_RENDER);
//		}else{
//			setv(_flags,FLAGS_LINE_RENDER);
//		}
//	}
//}

void scene_specialKeys(struct Scene* p,int key, int x, int y){
	float _cameMoveSpeed = -5.0f;
	struct EX* ex = p->enginePtr;
	
	switch(key) 
	{ 
	case GLUT_KEY_F1 : 
		ex_info();
		break;
	case GLUT_KEY_F2:
		setv(&(ex->flags),EX_FLAGS_RAY);
		break;

	case GLUT_KEY_F3:
		resetv(&(ex->flags),EX_FLAGS_RAY);
		break;

	case GLUT_KEY_F11:
		{
			//f_switchBoxLine();
			break;
		}
	case GLUT_KEY_F12:
		//
		break;

	case GLUT_KEY_UP:
		ex->camz-=_cameMoveSpeed;
		break;
	case GLUT_KEY_DOWN:
		ex->camz+=_cameMoveSpeed;
		break;
	case GLUT_KEY_LEFT:
		ex->camx-=_cameMoveSpeed;
		break;
	case GLUT_KEY_RIGHT:
		ex->camx+=_cameMoveSpeed;
		break;
	case GLUT_KEY_PAGE_UP:
		ex->camy+=_cameMoveSpeed;
		break;
	case GLUT_KEY_PAGE_DOWN:
		ex->camy-=_cameMoveSpeed;
		break;
	}
	setCamPos(ex->camx,ex->camy,ex->camz);
}
struct AvatarCfg* getMonsterCfg(struct Scene* scene,const char* id){
	struct LStackNode* s = (struct LStackNode* )scene->monsterCfgList;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		struct AvatarCfg* n;

		p=(void*)LStack_next(p);
		data = LStack_data(p);
		//delete (MonsterCfg*)data;
		n = (struct AvatarCfg*)data;
		//if(n->isEqualId(id)){
		if(isEqualId(n,id)){		
			return n;
		}
	}
	return NULL;
}

//float _fvalue = 0.0f;
static void f_md2TranslateM(struct HeadInfo* base){
/*
	struct Vec3 v2;
	struct Vec3 v1;
	struct Vec3 v3;

	v2.x = base->x;
	v2.y = base->y;
	v2.z = base->z;
	
	
	//	v1代表一个方向
	v1.x = 0;
	v1.y = 0;
	v1.z = 1;
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	//glRotatef(-90.0f,0.0f,0.0f,1.0f);
	
	v3.x = v2.x;
	v3.y = v2.y;
	v3.z = v2.z;

	//vec3Normalize(&v3);
	
	glRotatef(vecToAngle(&v1,&v3),0.0f,0.0f,1.0f);

	//gluLookAt(0.0,0.0,5.0,  0.0,0.0,0.0,  0.0,1.0,0.0); 
	//_fvalue+=0.1f;
*/
}

static void disposeMonsterList(struct Scene* scene){
	struct LStackNode* s = (struct LStackNode* )scene->monsterCfgList;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		//delete (MonsterCfg*)data;
		avatarCfg_dispose((struct AvatarCfg*)data);
	}
	LStack_delete(scene->monsterCfgList);
}
void scene_dispose(struct Scene* scene){
	xml_del(scene->monster_xml);
	scene->monster_xml = NULL;
	disposeMonsterList(scene);
}

static void avatarCfg_show(struct AvatarCfg* cfg){
	printf("mesh:%s\n",cfg->mesh);
	printf("tex:%s\n",cfg->tex);
	printf("path:%s\n",cfg->path);
	printf("type %d\n%s\nname:%s\n anim:%s\n\n",cfg->type,cfg->id,cfg->name,cfg->anim);
	//printf("%s\n",id);
}

//===================================================================================================





























/*
解析成两个char*
*/
static void
f_pa1rse_key_value(const char*str,char*key,char*v){
	int n = tl_strpos(str,",",1);
	int l = strlen(str);
	memset(key,0,G_BUFFER_64_SIZE);
	memset(v,0,G_BUFFER_64_SIZE);
	tl_strsub(str,key,0,n - 1);
	tl_strsub(str,v,n + 1,l - 1);
}



//场景引用
//Scene3D* scene3D;
/*
主渲染循环
*/
//void renderLoop(void){
//	render(ex);
//	//Sleep(108.333333f);
//}
void onSpecialKeys(int key, int x, int y) {
	scene_specialKeys(scene,key,x,y);
}
void playCallBack(void* data){
	struct HeadInfo* _pObj = (struct HeadInfo*)data;
	base_curAnim(_pObj->frameAnim,"stand");
	//base_playOnce(_pObj->frameAnim,"stand");
	//printf("处理完成 %s 设置动作 %ld\n",_pObj->name,get_longTime());
}

/*
*	鼠标2D转3D
*/
void pickScreenTo3D(GLint button, GLint state, GLint x, GLint y){
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLint realy;
	GLdouble wx, wy, wz;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) 
		{
			glGetIntegerv(GL_VIEWPORT, viewport);
			glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
			realy = viewport[3] - y - 1;
			printf("Coordinate at curosr are (%4d, %4d, %4d)\n", x, y,viewport[3]);
			gluUnProject(x, realy, 0, mvmatrix, projmatrix, viewport, &wx,
				&wy, &wz);
			printf("World coords at z=0 are (%f, %f, %f)\n", wx, wy, wz);
			gluUnProject(x, realy, 1, mvmatrix, projmatrix, viewport, &wx,
				&wy, &wz);
			printf("World coords at z=1 are (%f, %f, %f)\n", wx, wy, wz);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			//exit(0); 
			break;
		}
	default: break;
	}
}

//void mousePlotFunc(GLint button, GLint state, GLint x, GLint y){
//	//pickScreenTo3D(button,state,x,y);
//	mousePlot(ex,button,state,x,y);
//}

struct BaseLoopVO* tmp=NULL;
struct BaseLoopVO* tmp1=NULL;

void clickFloor(float x,float y,float z){
	struct HeadInfo* _target;

	ramp_stopPlaying(tmp);
	ramp_stopPlaying(tmp1);
	//printf("*************\n");

	//测试对一个模型进行位移
	_target = ex_find(ex,scene->hero->name);

	if(_target){
		long time = 2000.0f;//播放的时间
		if(base_isCanAnimation(_target)){
			base_curAnim(_target->frameAnim,(const char*)"run");
			tmp1=ramp_delay(playCallBack,_target,time);
		}
		tmp=ramp_vec_trace(&_target->x,&_target->y,&_target->z,x,_target->y,z,time);
	}
}

/*
	射线拾取多边形
*/
static void f_rayPickCallBack(struct HitResultObject* last){
	struct HeadInfo* _node = ex_find(ex,last->name);
	
	struct HeadInfo* ehead;
	if(_node){
		
		if(_node->luaPickCallBack){
			int n = (int)ex_findNodeByName(ex,(const char*)_node->name);
			ex_callIntLuaFun(_node->luaPickCallBack,n);//lua拾取回调	
		}
		
		//f_switchFlags(_node);

		if(ex->pick3DObject){

			//printf("state:%d\n",strcmp(last.name,"floor"));

			//设置拾取的模型坐标
			ehead	 = ex->pick3DObject->base;
			ehead->x = last->x;
			ehead->y = last->y;
			ehead->z = last->z;

			//点击回调
			//raySelectCallBack(&last);

			
		}

		

		
		if(!strcmp(last->name,"floor")){
			//点击地板回调
			clickFloor(last->x,last->y,last->z);
		}
		
		if(TRUE)
		{
		//	ex_switch_baseFlags(_node, FLAGS_GLSL_OUTLINE );//FLAGS_GLSL_OUTLINE,FLAGS_VISIBLE,FLAGS_GLSL_OUTLINE,FLAGS_DRAW_PLOYGON_LINE
		}

		printf("(%s)射线拾取到的3D坐标:%f,%f,%f\t%s\t type=%d\n",last->name,last->x,last->y,last->z,_node->suffix,_node->curType);

	}
}
void f_Testmd5(void* p)
{
	md5_drawSkeleton(scene->md5);
}

/*
	lua_pcall(lua_State *L, int nargs, int nresults, int errfunc)
	nargs:参数个数，nresults：返回值个数，errFunc：错误处理函数，0表示无，表示错误处理函数在栈中的索引
*/
void onSpriteClickCallBack(struct Sprite* sprite,int localX,int localY)
{
	//printf("testBtnClickCallBack	spriteName:%s,%d,%d\n",((struct HeadInfo*)sprite->base)->name,localX,localY);	
	//sprite_dipose(spr);

	{
		////C调用lua脚本	btnCallBack
		//if((size_t)strlen((const char*)sprite->callLuaFunName))
		//{
		//	lua_State* L =mylua;
		//	lua_getglobal(L,(const char*)sprite->callLuaFunName);
		//	lua_pushinteger(L,(int)sprite);
		//	//lua_pushinteger(L,sprite->ptr_luaCallBack);
		//	//lua_pushlstring();
		//	if(lua_pcall(L,1,0,0)!=0){
		//		printf("error %s\n", lua_tostring(L,-1));
		//	}
		//}


		//鼠标点击了Sprite

		struct HeadInfo* b = base_get2(sprite);

		if((int)strlen((const char*)sprite->callLuaFunName)>0)
		{
			sprite->mouseLocalX = localX;
			sprite->mouseLocalY = localY;
			ex_callParmLuaFun((const char*)sprite->callLuaFunName,b->name);
		}
	}

}

/*
	copy一个MoveAction
*/
static void
copyMoveFuncStr(struct Sprite* ptr,const char* func)
{
	int bufferSize;
	if(func==NULL){
		return;
	}
	
	bufferSize = strlen(func);
	ptr->callLuaDragMove = tl_malloc(bufferSize+1);
	memset(ptr->callLuaDragMove,0,bufferSize+1);
	memcpy(ptr->callLuaDragMove,func,bufferSize);
}

static void
copyMouseDownFuncStr(struct Sprite* ptr,const char* func)
{
	int bufferSize;
	if(func==NULL){
		return;
	}
	if(ptr->callLuaMouseDown)
	{
		tl_free(ptr->callLuaMouseDown);
		ptr->callLuaMouseDown = NULL;
	}
	bufferSize = strlen(func);
	ptr->callLuaMouseDown = tl_malloc(bufferSize+1);
	memset(ptr->callLuaMouseDown,0,bufferSize+1);
	memcpy(ptr->callLuaMouseDown,func,bufferSize);
}
/*加载图集*/
static int
REG_atals_load(lua_State *L){
	int n = lua_gettop(L);
	const char* path = lua_tostring(L,1);
	const char* name = lua_tostring(L,2);
	struct Atals* ptr = atals_load(path,name);
	lua_pushinteger(L,(int)ptr);
	return 1;
}
/*
注册创建Sprite
spr=sprite_create("btnName",0,100,72,24)
*/
static int 
REG_sprite_create(lua_State *L)
{
	int n = lua_gettop(L);//获取脚本中的参数个数

	//name
	const char* _name = lua_tostring(L,1);
	
	//x
	int x = lua_tointeger(L,2);

	//y
	int y = lua_tointeger(L,3);

	//width
	int w = lua_tointeger(L,4);
	
	//height
	int h = lua_tointeger(L,5);
	
	//点击的回调函数文本
	const char* _clickFuncString = lua_tostring(L,6);
	
	//移动函数文本
	const char* _move = lua_tostring(L,7);
	
	//鼠标按下去的回调
	const char* _mouseDown = lua_tostring(L,8);
	
	//拖动的方向
	int direction = lua_tointeger(L,9);//9
	//////////////////////////////////////////////////////////////////////////
	struct Sprite* ptr;
	if(_move!=NULL && !strcmp(_move,""))
	{
		_move = NULL;
	}

	if(_mouseDown!=NULL	&& !strcmp(_mouseDown,""))
	{
		_mouseDown = NULL;
	}

	ptr = sprite_create(ex,(char*)_name,x,y,w,h,onSpriteClickCallBack,_clickFuncString);
	
	copyMoveFuncStr(ptr,_move);
	copyMouseDownFuncStr(ptr,_mouseDown);
	
	ptr->dragDirection = direction;//拖拽的方向

	//这里去引擎中的图集myAtals
	//ptr->atals = ex_getInstance()->myAtals;
	{
		//指定图集
		int atals = lua_tointeger(L,10);//10
		ptr->atals = (struct Atals*)atals;
	}
	lua_pushinteger(L,(int)ptr);
	return 1;
}
//为sprite设置图集
static int 
REG_sprite_bindAtals(lua_State *L)
{
	int n = lua_gettop(L);//获取脚本中的参数个数
	struct Sprite* sprite = (struct Sprite*)lua_tointeger(L,1);//1
	int atals = lua_tointeger(L,2);//2

	//设置图集
	sprite->atals = (struct Atals*)atals;
	return 0;
}

static int
REG_sprite_bindMaterial(lua_State *L){
	struct Sprite* sprite = (struct Sprite*)lua_tointeger(L,1);//1
	int material = lua_tointeger(L,2);//2
	sprite->material = (void*)material;
	return 0;
}

static int
REG_sprite_setDragScope(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	int x = lua_tointeger(L,2);
	int y = lua_tointeger(L,3);
	int w = lua_tointeger(L,4);
	int h = lua_tointeger(L,5);
	sprite_setDragScope((struct Sprite*)ptr,x,y,w,h);
	return 0;
}

//void sprite_setDragScope(struct Sprite* pSpr,int x,int y,int w,int h);


//	void sprite_texName(struct Sprite* spr,const char* texName)
static int 
REG_sprite_texName(lua_State *L)
{
	struct AtlasNode node;

	int ptr=lua_tointeger(L,1);
	const char* tex=lua_tostring(L,2);
	
	sprite_texName((struct Sprite*)ptr,tex,&node);

	lua_pushnumber(L,node.x);
	lua_pushnumber(L,node.y);
	lua_pushnumber(L,node.width);
	lua_pushnumber(L,node.height);
	return 4;
}

static int 
REG_tf_create(lua_State *L)
{
	int n = lua_gettop(L);
	int bufferSize = lua_tointeger(L,1);
	int x = lua_tointeger(L,2);
	int y = lua_tointeger(L,3);
	int r,g,b;
	struct TextField* tf;
	if(n==3){
		r = g = b = 1.0f;//无参数,就默认的现实白色的文本
	}else{
		r = lua_tonumber(L,4);
		g = lua_tonumber(L,5);
		b = lua_tonumber(L,6);
	}
	tf=tf_create(bufferSize,x,y,r,g,b);
	lua_pushinteger(L,(int)tf);
	lua_pushinteger(L,tf->height);
	//printf("REG_tf_create参数个数%d\n",n);
	return 2;
}
static int 
REG_tf_setPos(lua_State *L)
{
	int tf = lua_tointeger(L,1);
	int x = lua_tointeger(L,2);
	int y = lua_tointeger(L,3);
	tf_setPos((struct TextField*)tf,x,y);
	return 0;
}
static int 
REG_tf_setText(lua_State *L)
{
	int tf = lua_tointeger(L,1);
	const char* str = lua_tostring(L,2);
	tf_setText((struct TextField*)tf,str);
	return 0;
}

static int 
REG_sprite_setpos(lua_State *L)
{
	int sprite = lua_tointeger(L,1);
	int x = lua_tointeger(L,2);
	int y = lua_tointeger(L,3);
	sprite_setpos((struct Sprite*)sprite,x,y);
	return 0;
}
static int 
REG_tf_dispose(lua_State *L)
{
	int tf = lua_tointeger(L,1);
	tf_dispose((struct TextField*)tf);
	return 0;
}


/*
	注册setUV,设置2D精灵的UV
*/
static int
REG_setUV(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	float x = lua_tonumber(L,2);
	float y = lua_tonumber(L,3);
	float w = lua_tonumber(L,4);
	float h = lua_tonumber(L,5);
	sprite_setUV((struct Sprite*)ptr,x,y,w,h);
	return 0;
}

/*
	注册setv
*/
static int
REG_setv(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	int flags = lua_tointeger(L,2);
	//printf("flags=%d\n",flags);
	if(!flags){
		printf("设置了一个(%d)flags\n",flags);
		assert(0);
	}
	
	if(ptr!=0){
		struct HeadInfo* base=base_get2((void*)ptr);
		setv(&base->flags,flags);
	}
	else
	{
		printf("ptr=%d 取ptr错误\n",ptr);
		assert(0);
	}
	return 0;
}
/*
	注册resetv
*/
static int
REG_resetv(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	int flags = lua_tointeger(L,2);
	struct HeadInfo* base=base_get2((void*)ptr);
	resetv(&base->flags,flags);
	return 0;
}
/*
	注册getv
*/
static int
REG_getv(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	int flags = lua_tointeger(L,2);
	struct HeadInfo* base=base_get2((void*)ptr);
	lua_pushinteger(L,getv(&base->flags,flags));
	return 1;
}

/*
	注册tmat_createMaterial
*/
static int
REG_tmat_createMaterial(lua_State *L)
{
	const char* glslName = lua_tostring(L,1);
	const char* tex0 = lua_tostring(L,2);//第1张贴图
	const char* tex1 = lua_tostring(L,3);//第2张贴图
	void* material;

	#ifdef _DEBUG_MODE_
		int _t = get_longTime();
		log_color(0xff00ff,"######## 开始构建材质,使用shader(%s)\n",glslName);
	#endif

	material = tmat_createMaterial(glslName,(char*)tex0,(char*)tex1);

	#ifdef _DEBUG_MODE_
		log_color(0xff00ff,"######## 构建材质及其相关纹理 0x%0x 共消耗 %ld 毫秒\n",(int)material,(get_longTime()-_t));
	#endif

	lua_pushinteger(L,(int)material);

	return 1;
}
/************************************************************************/
/* 创建一个指定宽高的位图                                                                     */
/************************************************************************/
static int
REG_tmat_createMaterialTex(lua_State *L){
	const char* glslName = lua_tostring(L,1);
	int w = lua_tointeger(L,2);
	int h = lua_tointeger(L,3);
	void* material = tmat_createTex(glslName,w,h);
	lua_pushinteger(L,(int)material);
	return 1;
}

/*
	注册设置材质
	void 
	setMaterial(void* ptr,void* material)
*/
static int
REG_setMaterial(lua_State* L){
	int ptr = lua_tointeger(L,1);
	int material = lua_tointeger(L,2);
	setMaterial((void*)ptr,(void*)material);
	return 0;
}
/*
	注册	int load_model(const char* url,char* name,float x,float y,float z,float scale);
*/
static int
REG_load_model(lua_State *L)
{
	const char* name=lua_tostring(L,1);
	const char* url = lua_tostring(L,2);
	float x = lua_tonumber(L,3);
	float y = lua_tonumber(L,4);
	float z = lua_tonumber(L,5);
	float scale = lua_tonumber(L,6);
	lua_pushinteger(L,load_model((char*)name,(char*)url,x,y,z,scale));
	return 1;
}

/************************************************************************/
/* 单元测试                                                             */
/************************************************************************/
int REG_test_unit_01(lua_State *L);
static int
REG_test_unit_02(lua_State *L){
	const char* name=lua_tostring(L,1);
	float value = lua_tonumber(L,2);
	int n = (int)ex_findNodeByName(ex,name);
	struct HeadInfo* h = ex_find(ex,name);
	

	Quat4_t s;
	Quat4_t e;
	Quat4_t o;

	s[W] = 0;
	s[X] = 0;
	s[Y] = 1;
	s[Z] = 0;

	e[W] = 1;
	e[X] = 1;
	e[Y] = 0;
	e[Z] = 0;
	
	Quat_slerp(s,e,value,o);
	
	Quat_to_matrrix(o,h->quat_m);
	updateMat4x4(h);
	return 0;
}

/*
	输出当前已经使用了的字节大小
*/
static int
REG_tl_showCurMemery512(lua_State *L)
{
	const char* str = lua_tostring(L,1);
	tl_showCurMemery512(str);
	return 0;
}

/*
	加载VBO模型
*/
static int
REG_load_VBO_model(lua_State *L)
{
	const char* name = lua_tostring(L,1);
	const char* url = lua_tostring(L,2);
	int ptr = load_VBO_model((char*)name,(char*)url);
	lua_pushinteger(L,ptr);
	return 1;
}

static float
toFloat(const char* str)
{
	float f;
	sscanf_s(str,"%f",&f);
	return f;
}
static int
toInt(const char* str)
{
	int f;
	sscanf_s(str,"%d",&f);
	return f;
}
//static void 
//updateRotateY(struct HeadInfo* base)
//{
//	Matrix44f tm;
//	Matrix44f sm;
//	Matrix44f ry;
//
//	mat4x4_zero(base->m);
//
//	//x,y,z
//	mat4x4_identity(tm);
//	mat4x4_translate(tm,base->x,base->y,base->z);
//
//	//scale
//	mat4x4_identity(sm);
//	//mat4x4_scale(sm,base->scale,base->scale,base->scale);
//
//	//ry
//	mat4x4_identity(ry);
//	mat4x4_rotateY(ry,base->ry);
//
//	multi2(base->m,tm,sm);//先偏移到原点后缩放,之后位移
//	
//	mat4x4_rotateY(base->m,base->ry);
//}






/*
 *获取对象的属性
 */
static int
REG_get_attr(lua_State *L)
{
	int ptr = lua_tointeger(L,1);			//对象指针如Sprite,Ent3d,md2,md5,text
	const char* attrKey = lua_tostring(L,2);//获取的属性关键字
	//////////////////////////////////////////////////////////////////////////
	struct HeadInfo* base =base_get2((void*)ptr);
	/*if(!base)
	{
		printf("找不到对象0x%0x\n",ptr);
		return 0;
	}*/
	//获取文本的宽度
	if(!strcmp(attrKey,"textWidth"))
	{
		int textWidth=tf_getTextWidth((struct TextField*)ptr);
		lua_pushinteger(L,textWidth);
	}
	
	//获取对象的类型
	if(!strcmp(attrKey,"type")){
		lua_pushinteger(L,base->curType);
	}

	//当前的动作帧(0开始)
	if(!strcmp(attrKey,"getCurFrame"))
	{
		if(base->curType == TYPE_MD5_FILE){
			int frame =  md5_getCurFrame((struct MD5*)ptr);
			lua_pushinteger(L,frame);
		}
		else
		{
			log_code(ERROR_GET_FRAME);
		}
		return 1;
	}
	
	//获取当前md5,md2的关键帧数量
	if(!strcmp(attrKey,"frameCount"))
	{
		if(base->curType == TYPE_MD5_FILE){
			lua_pushinteger(L,md5_getCurAnimFrameCount((struct MD5*)ptr));
		}
		else{
			log_code(ERROR_PTR_TYPE_FAIL);
			lua_pushinteger(L,1);
		}
		return 1;
	}


	//获取鼠标点击的坐标
	if(!strcmp(attrKey,"spriteLocalXY"))
	{
		struct Sprite* sprite = (struct Sprite*)ptr;
		lua_pushinteger(L,sprite->mouseLocalX);
		lua_pushinteger(L,sprite->mouseLocalY);
		return 2;
	}

	if(!strcmp(attrKey,"spriteSize"))
	{
		struct Sprite* sprite = (struct Sprite*)ptr;
		lua_pushnumber(L,sprite->mWidth);
		lua_pushnumber(L,sprite->mHeight);
		return 2;
	}

	if(!strcmp(attrKey,"spritePos"))
	{
		struct Sprite* sprite = (struct Sprite*)ptr;
		lua_pushnumber(L,sprite->screenX);
		lua_pushnumber(L,sprite->screenY);
		return 2;
	}
	
	if(!strcmp(attrKey,"dragDirection")){
		struct Sprite* sprite = (struct Sprite*)ptr;
		lua_pushnumber(L,sprite->dragDirection);
		return 1;
	}

	if(!strcmp(attrKey,"x"))
	{
		float x = base->x;
		lua_pushnumber(L,x);
	}
	if(!strcmp(attrKey,"y"))
	{
		float y = base->y;
		lua_pushnumber(L,y);
	}
	if(!strcmp(attrKey,"z"))
	{
		float z = base->z;
		lua_pushnumber(L,z);
	}
	if(!strcmp(attrKey,"suffix"))
	{
		//获取文件后缀
		lua_pushstring(L,base->suffix);
		return 1;
	}
	if(!strcmp(attrKey,"campos"))
	{
		//float x = base->x;
		lua_pushnumber(L,ex->camx);
		lua_pushnumber(L,ex->camy);
		lua_pushnumber(L,ex->camz);
		return 3;
	}
	if(!strcmp(attrKey,"get_longTime")){
		lua_pushnumber(L,get_longTime());
		return 1;
	}
	
	
	//获取当前时间 精确到豪秒
	if(!strcmp(attrKey,"get_longTime"))
	{
		lua_pushnumber(L,get_longTime());
	}
	if(!strcmp(attrKey,"get_name"))
	{
		lua_pushstring(L,base->name);
		return 1;
	}

	
	
	return 1;
}

/*
	设置Camera坐标
*/
static int 
f_SetCameraPosition(lua_State *L)
{
	float x= lua_tonumber(L,2);
	float y= lua_tonumber(L,3);
	float z= lua_tonumber(L,4);
	
	setCamPos(x,y,z);

	ex_updatePerspctiveModelView();
	
	return 0;
}

/*
	设置坐标
*/
static int 
f_SetNodePosition(lua_State* L)
{
	struct Node* ptrNode = (struct Node*)lua_tointeger(L,2);
	
	float x = lua_tonumber(L,3);
	float y = lua_tonumber(L,4);
	float z = lua_tonumber(L,5);
	if(ptrNode)
		base_setPos(ptrNode->base,x,y,z);
	else
		assert(!"ptrNode对象引用为NULL");

	return 0;
}
/*
	设置缩放
*/
static int 
SetScale(lua_State* L)
{
	struct Node* ptrNode = (struct Node*)lua_tointeger(L,2);
	float scaleValue = lua_tonumber(L,3);
	if(ptrNode!=NULL)
	{
		ptrNode->base->scale=scaleValue;
		updateMat4x4(ptrNode->base);
	}
	else
	{
		printf("ptrNode=NULL\n");
	}
	return 0;
}
/*
	寻找节点
*/
static int 
L_FindNode(lua_State* L)
{
	void* node = ex_findNodeByName(ex,lua_tostring(L,2));
	lua_pushinteger(L,(int)node);
	return 1;
}
/*
	寻找节点
*/
static int 
L_GetName(lua_State* L)
{
	struct Node* ptr = (struct Node*)lua_tointeger(L,2);
	struct HeadInfo* base =base_get2((void*)ptr);
	lua_pushstring(L,base->name);
	return 1;
}
//获取系统时间
static int
GetLongTime(lua_State* L)
{
	lua_pushnumber(L,get_longTime());
	return 1;
}
//输出日志
static int
LogPut(lua_State* L){
	const char* str = lua_tostring(L,2);
	int color = lua_tonumber(L,3);
	log_put(str,color);
	return 0;
}

/*
	执行引擎函数
*/
static int
REG_dofunc(lua_State *L)
{
	const char* attrKey = lua_tostring(L,1);//改变的属性关键字
	if(!strcmp(attrKey,"tf_copy"))
	{
		struct TextField *tf_source = (struct TextField*)lua_tointeger(L,2);
		struct TextField *tf_dest= (struct TextField *)lua_tointeger(L,3);
		if(tf_dest)
		{
			tf_copy(tf_source,tf_dest);
		}
		return 0;
	}
	if(!strcmp(attrKey,"tf_get_str"))
	{
		struct TextField *tf_source = (struct TextField*)lua_tointeger(L,2);
		lua_pushstring(L,(const char*)tf_source->textPtr);
		return 1;
	}
	
	//设置材质的参数
	if(!strcmp(attrKey,"materialSetVar"))
	{
		struct Node* ptr = (struct Node*)lua_tointeger(L,2);
		struct HeadInfo* base =base_get2((void*)ptr);
		const char* var=lua_tostring(L,3);
		if(!strcmp(var,"_LineWidth"))
		{
			struct GMaterial* m = (struct GMaterial*)base->tmat;
			m->lineWidth = lua_tonumber(L,4);
		}

		//设置透明度
		if(!strcmp(var,"_Alpha"))
		{
			struct GMaterial* m = (struct GMaterial*)base->tmat;
			m->_Alpha = lua_tonumber(L,4);
		}
		return 0;
	}
	
	//reload贴图
	if(!strcmp(attrKey,"reloadTexture")){
		//testTex
		struct Node* ptr = (struct Node*)lua_tointeger(L,2);
		struct HeadInfo* base =base_get2((void*)ptr);
		struct GMaterial* m = (struct GMaterial*)base->tmat;
		const char* texName = lua_tostring(L,3);
		GLuint tex = tmat_getTextureByIndex(m,0);
		jgl_subImage(tex,texName,0,0,256,256);
	}

	if(!strcmp(attrKey,"SetCameraPosition"))
		return f_SetCameraPosition(L);
	
	if(!strcmp(attrKey,"SetNodePosition"))
		return f_SetNodePosition(L);

	if(!strcmp(attrKey,"SetScale"))
		return SetScale(L);

	if(!strcmp(attrKey,"FindNode"))
		return L_FindNode(L);

	if(!strcmp(attrKey,"GetName"))
	{
		return L_GetName(L);
	}

	if(!strcmp(attrKey,"GetLongTime"))
	{
		return GetLongTime(L);
	}

	if(!strcmp(attrKey,"LogPut")){
		return LogPut(L);
	}

	return 0;
}



/*
	更新3D对象的属性
*/
static int
REG_change_attr(lua_State *L)
{
	int ptr = lua_tointeger(L,1);			//对象指针如Sprite,Ent3d,md2,md5
	const char* attrKey = lua_tostring(L,2);//改变的属性关键字
	const char* str = lua_tostring(L,3);	//改变的数据1
	const char* str2 = lua_tostring(L,4);	//改变的数据2
	struct HeadInfo* base =base_get2((void*)ptr);
	struct Node* node = (struct Node*)ptr;
	int change = 0;
	if(!base)
	{
		//printf("找不到对象0x%0x\n",ptr);
		//return 0;
		
		//设置camera rotateX
		if(!strcmp(attrKey,"camRX"))
		{
			//base->z=getFloat(str);
			//updateMat4x4(base);
			ex_getInstance()->camRotateX = toFloat(str);
			change = 1;
		}
		if(!strcmp(attrKey,"camRY"))
		{
			ex_getInstance()->camRotateY = toFloat(str);
			change = 1;
		}
		if(!strcmp(attrKey,"camRZ"))
		{
			ex_getInstance()->camRotateZ = toFloat(str);
			change = 1;
		}
		//=======================================
		//设置camera的坐标
		if(!strcmp(attrKey,"setCamPos"))
		{
			float x,y,z;
			sscanf_s(str,"%f,%f,%f",&x,&y,&z);
			setCamPos(x,y,z);
			change = 1;
		}

		if(!strcmp(attrKey,"gc"))
		{
			//内存池gc
			//memory_get_info();
			memory_gc();
			//memory_get_info();
		}
		return 0;
	}

	//########################################################
	//可以处理成兼容模式
	//设置Sprite属性
	if(!strcmp(attrKey,"sprite_position")){
		//(struct Sprite*)ptr
		//printf("%s:%s\n",attrKey,str);
		float x,y;
		sscanf_s(str,"%f,%f",&x,&y);
		sprite_setpos((struct Sprite*)ptr,x,y);
	}
	//设置TextField坐标
	if(!strcmp(attrKey,"txt_position")){
		float x,y;
		sscanf_s(str,"%f,%f",&x,&y);
		tf_setPos((struct TextField*)ptr,x,y);
	}
	

	//设置材质的标示flags
	if(!strcmp(attrKey,"setMaterialFlags"))
	{
		struct GMaterial* ptrMaterial = (struct GMaterial*)base->tmat;
		int flags=toInt(str);
		setv(&ptrMaterial->flags,flags);

		//printf("%d",getv(&ptrMaterial->flags,SHADER_FLAG_FLAT_COLOR));
	}
	if(!strcmp(attrKey,"glslParms"))
	{
		struct GMaterial* m = (struct GMaterial*)base->tmat;

		char key[G_BUFFER_64_SIZE];
		char v[G_BUFFER_64_SIZE];
		f_pa1rse_key_value(str,key,v);
/*
		int n = tl_strpos(str,",",1);
		int l = strlen(str);
		memset(key,0,G_BUFFER_64_SIZE);
		memset(v,0,G_BUFFER_64_SIZE);
		tl_strsub(str,key,0,n - 1);
		tl_strsub(str,v,n + 1,l - 1);
*/
		//#################################################
		//log_color(0xff0000,"%s\n",str);
		if(!strcmp(key,"lineWidth")){
			m->lineWidth = toFloat(v);	//设置线框宽度
			
		}
		else if(!strcmp(key,"alpha")){
			m->_Alpha = toFloat(v);		//设置透明度
		}
		//#################################################
	}

	//设置sprite Z轴旋转
	if(!strcmp(attrKey,"sprite_rotateZ"))
	{
		sprite_rotateZ((struct Sprite*)ptr,toFloat(str));
		change = 1;
	}

	if(!strcmp(attrKey,"sprite_resize"))
	{
		float w,h;
		sscanf_s(str,"%f,%f",&w,&h);
		sprite_resize((struct Sprite*)ptr,w,h);
		change = 1;
	}

	//=====================================================
	//rx
	if(!strcmp(attrKey,"rx"))
	{	
		base->rx=toFloat(str);
		updateMat4x4(base);
		//updateRotateY(base);
		change = 1;
	}
	//ry
	if(!strcmp(attrKey,"ry"))
	{	
		base->ry=toFloat(str);
		updateMat4x4(base);
		//updateRotateY(base);
		change = 1;
	}
	//ry
	if(!strcmp(attrKey,"rz"))
	{	
		base->rz=toFloat(str);
		updateMat4x4(base);
		//updateRotateY(base);
		change = 1;
	}
	//scale修改其缩放
	if(!strcmp(attrKey,"scale"))
	{
		base->scale=toFloat(str);
		updateMat4x4(base);
		//更新缩放值
		change = 1;
	}
	//======================================
	//x
	if(!strcmp(attrKey,"x"))
	{
		base->x=toFloat(str);
		updateMat4x4(base);
		change = 1;
	}
	//y
	if(!strcmp(attrKey,"y"))
	{
		base->y=toFloat(str);
		updateMat4x4(base);
		change = 1;
	}
	//z
	if(!strcmp(attrKey,"z"))
	{
		base->z=toFloat(str);
		updateMat4x4(base);
		change = 1;
	}

	//设置md5的关键帧
	if(!strcmp(attrKey,"setframe")){
		int v = toInt(str);
		//printf("设置动作索引 %d\n",v);
		
		//v++;
		
		if(v<-1){
			log_code(ERROR_BAD_VALUE);
			assert(0);
		}

		if(base->curType == TYPE_MD5_FILE){
			base->custframe = v;
		}else{
			log_code(ERROR_PTR_TYPE_FAIL);
		}
		
	}
	//设置md5的碰撞半径
	if(!strcmp(attrKey,"set_rayRadius"))
	{
		float radius = toFloat(str);
		if(base->curType == TYPE_MD5_FILE){
			float v = radius * 2;
			tl_setAbsBoundBox(v,v,v,base->rayVertexData.vertex);
		}else{
			log_code(ERROR_PTR_TYPE_FAIL);
		}
	}
	/************************************************************************/
	/* 设置静态盒子颜色                                                                     */
	/************************************************************************/
	if(!strcmp(attrKey,"set_box_color")){
		float r,g,b;
		sscanf_s(str,"%f,%f,%f",&r,&g,&b);
		//sprite_setpos((struct Sprite*)ptr,x,y);
		base->boxR = r;
		base->boxG = g;
		base->boxB = b;
	}

	//绑定拾取回调
	if(!strcmp(attrKey,"set_pickBind"))
	{
		if(!getv(&(base->flags),FLAGS_RAY)){
			log_code(ERROR_FLAGS);
			assert(0);
		}

		base_setLuaPick(base,str);
	}
	//设置xyz
	if(!strcmp(attrKey,"setModelPos"))
	{
		float x,y,z;
		sscanf_s(str,"%f,%f,%f",&x,&y,&z);
		
		if(base){
			base->x = x;
			base->y = y;
			base->z = z;
			updateMat4x4(base);
		}
		else
		{
			printf("setModelPos找不到对象\n");
		}
		change = 1;
	}

	//设置look at target
	if(!strcmp(attrKey,"setLookTarget"))
	{
		float x,y,z;
		sscanf_s(str,"%f,%f,%f",&x,&y,&z);
		setLookTarget((void*)ptr,x,y,z);
		change = 1;
	}
	
	//显示日志
	if(!strcmp(attrKey,"ex_showLog"))
	{
		ex_showLog(str);
	}
	if(!strcmp(attrKey,"test"))
	{
		multChange(base);
	}
	
	//////////////////////////////////////////////////////////////////////////
	//设置fps
	if(!strcmp(attrKey,"fps"))
	{
		float v = toFloat(str);

		if(node){
			if(node->base->curType ==TYPE_MD5_FILE){
				node->base->fpsInterval = v;
			}
			else if(node->base->curType ==TYPE_OBJ_VBO_FILE && node->anim){
				anim_set_fps(node->anim,v);
			}

			//printf("%d\n",node->base->curType);//md5 1,md2 5
		}else{
			printf("fps`s node = NULL\n");
			assert(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//添加关键帧切割
	if(!strcmp(attrKey,"animtor_push"))
	{
		struct Animtor* animtor=anim_get_animtor(node->anim);
		int s,e;
		sscanf_s(str2,"%d,%d",&s,&e);
		animtor_push(animtor,str,s,e);
	}
	//设置当前动作
	if(!strcmp(attrKey,"animtor_setcur"))
	{
		struct Animtor* animtor=anim_get_animtor(node->anim);
		
		if(!animtor_setcur(animtor,(char* const)str))
		{
			printf("为(%s)设置动作:%s失败\n",base->name,str);
		}
	}

	//开始播放当前动作
	if(!strcmp(attrKey,"animtor_play"))
	{
		struct Animtor* animtor=anim_get_animtor(node->anim);
		animtor_play(animtor);
	}
	
	//暂停播放当前动作
	if(!strcmp(attrKey,"animtor_pause"))
	{
		struct Animtor* animtor=anim_get_animtor(node->anim);
		animtor_pause(animtor);
	}

	//指定区间播放动画
	if(!strcmp(attrKey,"animtor_play_start_end"))
	{
		struct Animtor* animtor=anim_get_animtor(node->anim);
		int s,e;
		sscanf_s(str,"%d,%d",&s,&e);
		//printf("%d,%d\n",s,e);
		animtor_play_start_end(animtor,s,e);
	}

	//////////////////////////////////////////////////////////////////////////
	//绑定一个射线拾取盒,只有绑定了射线拾取盒才能被点击拾取
	if(!strcmp(attrKey,"collide_load"))
	{
		int frameCount = 0;
		if(str2)
			sscanf_s(str2,"%d",&frameCount);

		node->ptrCollide=collide_load(str,frameCount);//"\\resource\\obj\\"..objName..".obj"
	}
	if(change)
		ex_updatePerspctiveModelView();
	return 0;
}


/*
	注册	load_md2Anim
*/
static int
REG_load_animConf(lua_State *L)
{
	int md2 = lua_tointeger(L,1);
	const char* animConf = lua_tostring(L,2);
	//const char* defaultAnim=lua_tostring(L,3);
	float fps = lua_tonumber(L,3);
	int stat=load_animConfig((void*)md2,(char*)animConf,(float)fps);
	lua_pushinteger(L,stat);
	return 1;
}
/*
	注册md5_loadAnim
*/
static int
REG_load_md5Anim(lua_State *L)
{
	int md5 = lua_tointeger(L,1);
	const char* md5Anim = lua_tostring(L,2);
	const char* animKey =  lua_tostring(L,3);
	md5_loadAnim((struct MD5*)md5,md5Anim,animKey);
	return 0;
}
/*
	注册setanim
*/
static int
REG_setanim(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	const char* animKey =  lua_tostring(L,2);
	setanim((void*)ptr,animKey);
	return 0;
}

/*
	注册销毁Sprite
*/
static int 
REG_ptr_remove(lua_State *L)
{
	int ptr = lua_tointeger(L,1);
	struct HeadInfo* b;
	//if(ptr==0)
	//{
	//	printf("REG_ptr_remove`s ptr=0\n");
	//	return 0;
	//}
	
	b=base_get2((void*)ptr);
	if(!b)
	{
		log_code(ERROR_PTR_REMOVE_FAIL);
		printf("REG_ptr_remove移除失败,找不到对象0x%0x\n",ptr);
		assert(0);
		return 0;
	}
	if(b->curType==TYPE_SPRITE_FLIE)
	{
		sprite_dipose((struct Sprite*)ptr);
	}
	else if(b->curType==TYPE_TEXT_FILE)
	{
		tf_dispose((struct TextField*)ptr);
	}
	else if(b->curType ==  TYPE_OBJ_VBO_FILE)
	{
		//md2,obj的vbo模式
		//objVBO_dispose((struct Obj_vbo_model*)ptr);
		node_dispose((struct Node*)ptr);
	}
	else if(b->curType == TYPE_MD2_FILE)
	{
		md2_dispose((struct MD2_Object*)ptr);
	}
	else if(b->curType == TYPE_OBJ_FILE)
	{
		ent_dispose((struct Ent3D*)ptr);
	}
	else if(b->curType == TYPE_MD5_FILE)
	{
		md5Model_dispose((struct MD5*)ptr);
	}
	return 0;
}
//执行lua脚本
lua_State* runhelloTest(const char* lua){
	
	lua_State* lua_state = luaL_newstate();
	luaL_openlibs(lua_state);

	//注册接口函数
	
	lua_register(lua_state,"ptr_remove",REG_ptr_remove);
	
	lua_register(lua_state,"setv",REG_setv);
	lua_register(lua_state,"resetv",REG_resetv);
	lua_register(lua_state,"getv",REG_getv);
	
	//创建材质
	lua_register(lua_state,"createMaterial",REG_tmat_createMaterial);
	lua_register(lua_state,"createMaterialTex",REG_tmat_createMaterialTex);
	
	lua_register(lua_state,"load_model",REG_load_model);
	lua_register(lua_state,"load_animConfig",REG_load_animConf);
	lua_register(lua_state,"setMaterial",REG_setMaterial);
	lua_register(lua_state,"md5_loadAnim",REG_load_md5Anim);
	lua_register(lua_state,"setanim",REG_setanim);
	
	//单元测试案例01
	lua_register(lua_state,"test_unit_01",REG_test_unit_01);
	//单元测试案例02
	lua_register(lua_state,"test_unit_02",REG_test_unit_02);

	//获取属性
	lua_register(lua_state,"get_attr",REG_get_attr);
	//设置属性
	lua_register(lua_state,"change_attr",REG_change_attr);
	//执行函数
	lua_register(lua_state,"dofunc",REG_dofunc);
	
	//Sprite
	lua_register(lua_state,"sprite_create", REG_sprite_create);
	lua_register(lua_state,"sprite_texName",REG_sprite_texName);
	lua_register(lua_state,"setUV",REG_setUV);
	lua_register(lua_state,"sprite_setpos",REG_sprite_setpos);
	lua_register(lua_state,"sprite_setDragScope",REG_sprite_setDragScope);
	lua_register(lua_state,"sprite_bindAtals",REG_sprite_bindAtals);
	lua_register(lua_state,"sprite_bindMaterial",REG_sprite_bindMaterial);//绑定材质

	//图集
	lua_register(lua_state,"atals_load",REG_atals_load);

	

	//文本
	lua_register(lua_state,"tf_create",REG_tf_create);
	lua_register(lua_state,"tf_dispose",REG_tf_dispose);
	lua_register(lua_state,"tf_setPos",REG_tf_setPos);
	lua_register(lua_state,"tf_setText",REG_tf_setText);
	lua_register(lua_state,"load_VBO_model",REG_load_VBO_model);
	lua_register(lua_state,"tl_showCurMemery512",REG_tl_showCurMemery512);
	

	
	//void sprite_setpos(struct Sprite* spr,int x,int y)

	if(luaL_dofile(lua_state,lua))
	{
		printf("--------------------(%s)脚本运行错误!\n",lua);
	}
	//lua_close(lua_state);
	return lua_state;
}
static void
multChange(struct HeadInfo* base)
{
	/*Matrix44f m0;
	Matrix44f m1;
	Matrix44f _out;
	mat4x4_identity(m0);
	mat4x4_identity(m1);
	mat4x4_zero(_out);

	mat4x4_translate(m0,4,5,6);
	
	mat4x4_scale(m1,2,2,2);

	multi2(_out,m0,m1);

	mat4x4printf("m0",m0);
	mat4x4printf("m1",m1);
	mat4x4printf("m2",_out);*/

	updateMat4x4(base);
} 
/*
	在引擎初始化之前,初始化着色器
*/
static void
initShader()
{
	tlgl_getShader("vboDiffuse");
	tlgl_getShader("sprite");
}


/* 
初始化场景                                                                     
*/
void initScene(struct Scene* scene,char* luaScript){

	struct EX* ex = scene->enginePtr;
	
	//初始化日志文本缓存区
	ex_showLog("");

	initShader();

	//初始化图集
	//ex_getInstance()->myAtals=atals_load("//resource//texture//","1");
	
	//加载执行lua
	{
		char outStr[G_BUFFER_256_SIZE];
		char __ss[G_BUFFER_256_SIZE];
		sprintf_s(__ss,G_BUFFER_256_SIZE,"//include//lua//%s",luaScript);
		tl_convert_path(__ss,outStr,G_BUFFER_256_SIZE);
		
		printf("执行Lua脚本%s\n",outStr);
		ex_getInstance()->mylua=runhelloTest(outStr);
	}

	//sprite_create(ex,"sp1",0,0,50,20,onSpriteClickCallBack,NULL);
	

	//printf("sizeof(float)=%d,%d\n",sizeof(float),sizeof(double));
}
/*
检测多重采样缓冲区是否存在,以及对每个像素使用几个采样
*/
void checkBuffersSample(){
	GLint bufs,samples;
	glGetIntegerv(GL_SAMPLE_BUFFERS,&bufs);
	glGetIntegerv(GL_SAMPLES,&samples);
	printf("MSAA buffers = %d samples = %d\n",bufs,samples);
}

void tlgl_reshape(int w,int h){

	if (h == 0){
		h = 1;
	}
	ex->_screenWidth = w;
	ex->_screenHeight = h;

	ex_update_uiPos();
	
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	ex_updatePerspctiveModelView();
}
void psv(){
	struct PCsvVO * pcsv;
	int out;
	Array* arr;
	//tl_memState(TRUE);
	//printf("===========>%d bytes\n",tl_memByteSize());
	pcsv = pcsv_load("D:\\source\\resource\\CaiLiaoHeCheng.csv","itemid",1);

	arr=pcsv_getValue(pcsv,"qianghua10");
	array_get(arr,1,&out);
	if(out){
		printf("%s\n",(char*)out);
	}
	pcsv_dispose(pcsv);
	//printf("===========>%d bytes\n",tl_memByteSize());
}
void strTest(){
	
	struct Str* pstr = NULL;
	//tl_memState(TRUE);
	printf("===========>%d bytes\n",tl_memByteSize());

	//printf("%d\n",strcspn("Jack","ck"));
	pstr = str_create("HIhelloABChelloDEFGhelloTF");


	
	/*for(i = 0;i <3;i++){
		str_appand(pstr,"ABC_");
	}*/

	printf("%s\n",pstr->s);
	str_replace(pstr,"hello","abc");
	printf("%s\n",pstr->s);

	
	
	//str_dispose(pstr);
	pstr = NULL;
	
	printf("===========>%d bytes\n",tl_memByteSize());
	
	//psv();
	//getchar();
}

void testGet(char* const str)
{
	int a;


	//s=NULL;
	char _s[128];
	memset(_s,0,128);
	memcpy(_s,str,strlen(str));

	a = getchar();
	printf("%d\n",a);
	if(a==65){
		printf("is OK!\n");
	}
}

//void mat4test(float x,float y,float z,float scale)
//{
//	//结果矩阵
//	Matrix44f allMat4;
//	
//	//位移矩阵
//	Matrix44f _translate_mat4;
//
//	//缩放矩阵
//	Matrix44f _scale_mat4;
//	float _scaleValue = scale;
//
//	Matrix44f _rotateX_mat4;
//
//	mat4x4_clearIdentity(_rotateX_mat4);
//	mat4x4_clearIdentity(_translate_mat4);
//	mat4x4_clearIdentity(allMat4);
//	mat4x4_clearIdentity(_scale_mat4);
//
//	mat4x4_translate(_translate_mat4,x,y,z);//位移
//
//	mat4x4_scale(_scale_mat4,_scaleValue,_scaleValue,_scaleValue);
//	mat4x4_rotateX(_rotateX_mat4,90);
//	//mat4x4_rotateY(_rotateX_mat4,r);
//	//r+=0.01;
//
//	multi2(allMat4,_translate_mat4,_scale_mat4);//先位移再缩放
//	//multi2(allMat4,_scale_mat4,_translate_mat4);//先位移再缩放
//}



void lua1()
{
	//开启lua状态机  
    //lua_State* L = luaL_newstate();  
    ////装载lua库  
    //luaL_openlibs(L);  
    ////执行脚本  
    //luaL_dofile(L, "hello.lua");  
    ////关闭lua状态机  
    //lua_close(L);  

	//char* t[2] = ["texture1","texture2"];
	//const char *vCodeArray[2] = {"texture1","texture2"};

	/*int i;
	for(i = 0;i < 2;i++)
	{
		printf("%d,%s\n",sizeof(vCodeArray),vCodeArray[i]);
	}*/

	/*char s[256];
	get_allTime(s,256);
	printf("%s\n",s);*/
}
//配置解析
struct Config
{
	int isMemory;
	/**
	*	屏幕宽度
	*/
	int screenWidth;
	/************************************************************************/
	/* 屏幕高度                                                                     */
	/************************************************************************/
	int screenHeight;
	float rate;
	char LuaScript[G_BUFFER_64_SIZE];
};

/*
	解析内存池配置
*/
static void 
ParseMemoryCfg(struct XMLSList* xml,int* pIsMemory)
{
	int useMemory;
	char mBuffer[G_BUFFER_64_SIZE];
	struct XmlNode* memoryNode = xml_getrow(xml,"id","memory");

	xml_getstr(memoryNode,"value",mBuffer,G_BUFFER_64_SIZE);
	//printf("Buffer=%s\n",Buffer);
	useMemory = tl_boolString2Int(mBuffer);
	*pIsMemory=useMemory;

	printf("启用内存池状态:%s\n",tl_intConvertBool(useMemory));		
}
/*
	解析Lua脚本入口名
*/
static void 
ParseLuaScript(struct XMLSList* xml,void* buffer)
{
	struct XmlNode* memoryNode = xml_getrow(xml,"id","LuaScript");
	xml_getstr(memoryNode,"value",buffer,G_BUFFER_64_SIZE);
}

//static HANDLE
//GetHandle()
//{
//	HANDLE hOut; 
//
//	//  获取输出流的句柄
//	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//	return hOut;
//}

static void loadXml(struct Config* cfg)
{
	struct XMLSList* xml = NULL;
	struct XmlNode* node=NULL;
	
	char monster[G_BUFFER_64_SIZE];
	char* _data =  tl_loadfile("\\resource\\config.xml",NULL);
	//HANDLE hOut =  GetHandle();

	xml = xml_parse(_data);

	//SetConsoleTextAttribute(hOut, FOREGROUND_GREEN |     /* 前景色_绿色*/FOREGROUND_INTENSITY );// 前景色_加强
	//printf("****************************************\nconfig.xml=\n%s\n****************************************\n",_data);
	//SetConsoleTextAttribute(hOut, FOREGROUND_RED |   /* 前景色_红色*/FOREGROUND_GREEN | /* 前景色_绿色*/FOREGROUND_BLUE ); /* 前景色_蓝色*/
	
	log_color(0x00ff00,"****************************************\nconfig.xml=\n%s\n****************************************\n",_data);
	
	//=============================================
	//解析
	node =	xml_getrow(xml,"id","0");

	if(node)
	{
		//怪物xml路径
		xml_getstr(node,"monster",monster,G_BUFFER_64_SIZE);
		printf("========================>怪物路径为%s\n",monster);
	
		cfg->screenWidth = xml_getint(node,"w");
		cfg->screenHeight = xml_getint(node,"h");
		cfg->rate = xml_getfloat(node,"rate");

	}
	else
		printf("节点数据为NULL\n");


	ParseMemoryCfg(xml,&cfg->isMemory);
	
	ParseLuaScript(xml,&cfg->LuaScript);

	if(xml){
		xml_del(xml);
	}
}

static void leftRayPickCallBack(struct HitResultObject* hit)
{
	struct HeadInfo* _node = ex_find(ex,hit->name);
	if(_node)
	{
		//按着拾取的物体旋转
		_node->ry	+=	0.01f;
		updateMat4x4(_node);
	}
}

static void loopCallBack(void)
{
	if(ex_mouseIsLeftDown())
	{
		hit_mouse(ex->mouseState.xMouse,ex->mouseState.yMouse,ex->_screenWidth,ex->_screenHeight,ex->renderList,leftRayPickCallBack);
	}
}
static void
tStrstr()
{
	const char* s1 ="abcd";
	const char* s2 = "abc";
	printf("strstr ======%d\n",strstr(s1,s2)-s1);
}

//windows 内存 使用率
DWORD getWin_MemUsage(){
	MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);
	return ms.dwMemoryLoad;
}

/*
static void SetPos(int x, int y){             
	COORD point = { x, y };//光标要设置的位置x,y       
	HANDLE HOutput = GetStdHandle(STD_OUTPUT_HANDLE);//使用GetStdHandle(STD_OUTPUT_HANDLE)来获取标准输出的句柄      
	SetConsoleCursorPosition(HOutput, point);//设置光标位置
}
*/
/*
static void 
tabTest(){
	char t =	' ';	//	'\t';//9
	printf("[%c][0x%0x][%d]\n",t,t,t);
}
*/

static void 
Test1(){
	//char v0[G_BUFFER_64_SIZE];
	//
	//sscanf_s("f a","f %s",&v0,G_BUFFER_64_SIZE);
	//printf("%s\n",v0);
	//int i;
	//for(i = 0;i < 32;i++){
	//	void * p = memory_new(64);
	//	//memory_free(p);
	//}
	/*
	int i;
	//memory_get_info();
	for(i = 0;i < 2;i++){
		const char* copy = "abc";
		int len = strlen(copy);
		void* s = memory_new(len);
		memcpy(s,copy,len);
		log_color(0xff0000,"%d[%s]\n",i,s);
		//memory_free(s);
		memory_retrieve(s);
		log_color(0xff0000,"%d[%s]\n\n",i,s);
	}
	//memory_get_info();
	memory_gc();
	//memory_get_info();
*/

	/*
	//log_color(0xff0000,"%ld\n",pow(3,2));
	Quat4_t q;
	Matrix44f m;
	log_color(0xff0000,"%d\n",q - m);
	Quat_normalize(q);
	Quat_to_matrrix(q,m);
	//http://www.cnblogs.com/mengdd/p/3238223.html
	*/
/*
	Matrix44f a0;
	Matrix44f a1;
	mat4x4_identity(a0);
	mat4x4_zero(a1);

	mat4x4_printf("a0",a0);
	mat4x4_printf("a1",a1);

	//printf("%d,%d\t%d\n",&a0,&a1,(&(a1[0]))-(&(a0[0])));
	mat4x4_mult(2,a0,a1);
*/


	/*
	Matrix44f a0;
	Matrix44f a1;
	Matrix44f tmp0;

	mat4x4_identity(a0);
	a0[0] = 16;

	//mat4x4_printf("a0",a0);

	mat4x4_identity(a1);
	a1[0] = 2;
	//mat4x4_printf("a1",a1);
	
	mat4x4_zero(tmp0);
	multi2(tmp0,a0,a1);
	mat4x4_printf("tmp0",tmp0);

	{
		Matrix44f t;
		
		//log_color(0xff0000,"===================================\n");
		mat4x4_mult(2,t,a0,a1);

		mat4x4_printf("测试数据",t);


	}
	*/
}

void main(int argc,char** argv){ 
	float bottom_height = 40.0f;//底部偏移
	float rate = 0.0f;//比率
	
	struct Config cfg;

	//窗口宽,高
	int w,h;
	
	//tabTest();
	
	Test1();

	//memset(&cfg,0,sizeof(struct Config));

	//设置局部路径
	tl_setLocalPath(_MAIN_PATH_);
	tl_memInit("32,512@64,100@512,200@8192,200@81920,200@262144,100@5242880,25");//"64,100@512,200@8192,200@81920,200@262144,100@5242880,200"
	loadXml(&cfg);
	tl_memState(cfg.isMemory);
	
	//gotoxy(0,0);

	w = cfg.screenWidth;
	h = cfg.screenHeight;

	rate = cfg.rate;

	//testGet("dasdsds");

//	printf("系统使用内存:%ld\n",getWin_MemUsage());
	
	tStrstr();
	//strTest();
	lua1();
	//mat4test(0,100,0,1);

	
	
	

	ex = ex_create();

	//printf("---->%.3f\n",exp(8));

	//初始化场景
	scene = scene_create(ex);
	
	

	
	//==========================================================================
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE| GLUT_DEPTH );
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH |  GL_MULTISAMPLE);//GL_MULTISAMPLE多重采样

	checkBuffersSample();//多重采样检测

	//GL_MULTISAMPLE
	//GLUT: Fatal Error in test.exe: pixel format with necessary capabilities not found.

	//==========================================================================
	
	log_color(0x00ff00,"窗口比率rate = %.3f\n",rate);
	
	//log_put("a",0xffff00);
	//glutInitWindowPosition(0,0);//左上角
	glutInitWindowPosition(0,glutGet(GLUT_SCREEN_HEIGHT)-(h*rate) - bottom_height);//左上角
	//glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-(w * rate))/2,(glutGet(GLUT_SCREEN_HEIGHT)-(h*rate))/2);////居中显示,设置窗口坐标
	//glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-w * rate),0.0f);//右上角窗口
	//glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)),0.0f);//分屏左上角窗口
	//glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT)-h*2);//分屏左下角窗口
	//glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-w * rate),glutGet(GLUT_SCREEN_HEIGHT)-h*2);//右下角窗口
	

	//设置窗口的宽高
	glutInitWindowSize(w*rate,h*rate);
	glutCreateWindow("test"/*argv[0]*/);
	//glShf_MousePlotL_SMOOTH);
	glutDisplayFunc(render);
	glutReshapeFunc(tlgl_reshape);
	/*键盘事件*/
	glutSpecialFunc(onSpecialKeys);

	//普通键盘事件监听
	glutKeyboardFunc(onKeyboardCallBack);

	glutIdleFunc(render);

	if(TRUE)
	{
		GLenum err=glewInit();
		if(GLEW_OK!=err){
			printf("glewInit (%s)\n",glewGetErrorString(err));
			return;
		}
	}
	//printf("===================>0x%0x\n",(0xabcdef12>>16)&0xff);

	//获取opengl版本
	tlgl_showGLVersion();

	//初始化引擎
	ex_init(ex,100000);

	//ex->loopCallBack = loopCallBack;

	//hit_callBack(ex->hit,f_rayPickCallBack);
	ex->mRayPickCallBack = f_rayPickCallBack;

	//加载角色模型配置	
	scene_loadAvtCfg(scene,"//resource//monster.xml");

	//初始化场景
	initScene(scene,cfg.LuaScript);

	glutMouseFunc(mousePlot);
	//glutPassiveMotionFunc		//鼠标按下的时候会暂停
	//glutMotionFunc			//鼠标按下的时候继续获取屏幕坐标
	glutMotionFunc(mouseMove);

	glutMainLoop();

	ex_dispose(ex);
	ex = NULL;
	//delete scene3D;
	scene_dispose(scene);
	scene = NULL;

	lua_close(ex_getInstance()->mylua);
}

