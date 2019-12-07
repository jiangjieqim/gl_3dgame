#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#include "camera.h"
#include "tools.h"
#include "str.h"
#include "ftfont.h"
#include "tlgl.h"
#include "progrom3D.h"
#include "tmat.h"
#include "ex.h"
#include "md5.h"
#include "gettime.h"
#include "sprite.h"
#include "obj_vbo.h"
#include "node.h"
#include "atlas.h"
#include "obj.h"
#include "md2.h"
#include "tl_malloc.h"
#include "text.h"
#include "evt.h"
#include "xml.h"
#include "tween.h"
#include "base.h"
#include "frame.h"
#include "fbotex.h"
#include "atlas.h"
#include "map.h"
#include "line.h"
#include "timelater.h"
//#define DEBUG_PRINT_HIT	//打印拾取日志

//int g_sprite_line;
long g_delayTime;
int  g_fps=-1;
static struct EX* g_ex;
static long _longTime=0;
struct MD2_Object
{
	/*
	*	头部基础信息,可继承
	*/
	struct HeadInfo* base;

	/*
	*	解析器句柄,该对象只是存储md2模型的数据结构
	*/
	//struct MD2_ParseObj* parseHandle;
	void* parseHandle;

	/*
	*	动作管理器
	*/
	struct FrameAnim* frameAnim;
};

struct ClickInfo{
	struct Sprite* sprite;				//当前点击的sprite
	int local_click_x,local_click_y;	//临时点击相对于sprite的坐标
	float pos_z;
};

struct CallLaterNode
{
	void* parms;
	void (*callBack)(void*);
	int i;
};

//在drawcall的最后回调
void
callLater(void _callBack(void*),void* parms){
	
	struct CallLaterNode* node = (struct CallLaterNode*)tl_malloc(sizeof(struct CallLaterNode));
	
	{
		printf("push parms 0x%0x\n",parms);
	}
	node->callBack = _callBack;
	node->parms = parms;
	//ex_getIns()->lastList;
	node->i = ex_getIns()->index;
	LStack_push(ex_getIns()->lastList,(void*)node);
	printf("push node 0x%0x\n",node);
}





static void
f_pop(){
	void* p = ex_getIns()->lastList;

	//struct CallLaterNode* n = 0;
	if(LStack_length((struct LStackNode*)p)>0){
		int data;
		int status = LStack_pop(p,&data);
		if (status)
		{
			struct CallLaterNode* _node = (struct CallLaterNode*)data;
			_node->callBack(_node->parms);//这里有问题
			//n = _node;
			tl_free(_node);

			
		}

	}

	//if(n){
	//	n->callBack(n->parms);//这里有问题
	//	tl_free(n);
	//}

	//printf("pop\n");
}
static void 
runLastList(){

	f_pop();
	//struct LStackNode* s = ex_getIns()->lastList;
	//struct CallLaterNode* _node;
	//int cnt = 0;
	//void* top,*p;

	////printf("lstack length = %d\n",LStack_length(s));

	//top = s;
	//p=top;
	//while((int)LStack_next(p)){
	//	int data;
	//	p=(void*)LStack_next(p);
	//	data = LStack_data(p);
	//	if(data){
	//		
	//		if(!LStack_delNode(s,data)){
	//			printf("删除失败!\n");
	//		}else{
	//			printf("%0x删除成功\n",p);
	//		}
	//		
	//		_node = (struct CallLaterNode*)data;
	//		printf("** lstack length = %d,0x%0x,执行回调\n",LStack_length(s),_node);
	//		
	//		_node->callBack(_node->parms);//这里有问题

	//		tl_free(_node);

	//		{
	//			struct LStackNode* cn = p;
	//			cn->data = 0;
	//		}
	//	}

	//	
	//}
	//if(LStack_length(s) > 0)
	//	printf("清除栈的时候栈的长度 = %d\n",LStack_length(s));
	//
	////LStack_clear(s);
}

struct Ent3D{
	struct HeadInfo* base;

	/*顶点个数*/
	int vertexCount;

	/*顶点数组*/
	GLfloat* vertex;

};

//=============================================
//frame buffer Object tex(FBO)结构体

static const GLenum windowBuff[] = { GL_BACK_LEFT };



/*
	鼠标左键是否常按着
*/
//static int isLeftDown = 0;
struct EX* ex_getIns(){
	if(!g_ex){
		g_ex = (struct EX*)tl_malloc(sizeof(struct EX));
		memset(g_ex,0,sizeof(struct EX));
	}
	return g_ex;
}//引擎实例引用
/************************************************************************/
/* 是否鼠标左键按着  
/************************************************************************/
static int
f_is_mouse_leftDown(){
	struct EX* ex = ex_getIns();
	struct MouseState* s = &ex->mouseState;
	return s->button == GLUT_LEFT_BUTTON && s->action == GLUT_DOWN;
}
static void f_rayPick(struct HitResultObject* hit);
//static void
//f_3d_pick(){
//	struct EX* p = ex_getInstance();
//	base_hit_mouse(p->mouseState.xMouse,p->mouseState.yMouse,p->_screenWidth,p->_screenHeight,p->renderList,p->perspectiveMatrix,p->modelViewMatrix,f_rayPick);
//}
/*
	/1000/60 fps因子	代表16.66毫秒(即1/60秒)计算一次fps	
*/
#define _FPS_FACTOR_ 16.66
/*
	计算fps
*/
static void 
f_calculate_fps(){
/*
	//此方法会有1秒的计算延迟
	static float _fps = 0;        
	static float lastTime = 0.0f; 
	float currentTime = get_longTime()*0.001;//(float)clock()*0.001f;

	++_fps;
	if( currentTime - lastTime > 1.0f ){
		
		lastTime = currentTime;
		g_fps = (int)_fps;
		_fps = 0;
	}
*/


	static float _fps = 0;        
	static float lastTime = 0.0f; 
	float currentTime = get_longTime();//(float)clock()*0.001f;

	++_fps;

	//printf("_fps = %.3f g_fps = %d\n",_fps,g_fps);
	if( currentTime - lastTime > _FPS_FACTOR_ ){
		lastTime = currentTime;
		g_fps = (int)(_fps*1000/_FPS_FACTOR_);//(int)(_fps/1000);
		_fps = 0;
	}

}
static int 
f_get_fps(){
	return g_fps;
}
/*
*	md2渲染回调
*/
static void 
md2_render(struct MD2_Object* _md2){

	struct FrameAnim* frameAnim = _md2->frameAnim;
	struct HeadInfo* base =(struct HeadInfo*)_md2->base;
	struct MD2_Frame* frame;
	struct VertexData* p;

	//计算关键帧
	frame_run(frameAnim);

	frame =	md2parse_getFrame(_md2->parseHandle,frameAnim->curFrame);//&(_md2->parseHandle->pframe[frameAnim->curFrame]);
	//frame = &(_md2->parseHandle->pframe[0]);//只读取第一帧

	//设置数据
	p=&(base->rData);
	p->vertex = frame->vertices;
	p->vertLen= frame->vertCount;
	//md2parse_getFrameVertex(frame,&p->vertex,&p->vertLen);

	//实体绘制
	base_renderFill(base);
}
//static void ex_render3dNode(int data);
//static void render_uiNode(int data);
//int ex_mouseIsLeftDown()
//{
//	return isLeftDown;
//}
//void* ex_find_node(const char* name){
//	return ex_findNodeByName(ex_getInstance(),name);
//}
/*
根据name寻找对象(name为一个字符串)
*/
void* ex_find_ptr(struct EX* ptr,const char* name){
	//struct LStackNode* s = (struct LStackNode* )ptr->renderList;
	//void* top,*p;
	//top = s;
	//p=top;
	//while((int)LStack_next(p)){
	//	int data;
	//	struct HeadInfo* base = NULL;
	//	void* n;

	//	p=(void*)LStack_next(p);
	//	data = LStack_data(p);

	//	n = (void*)data;
	//	//base_get(n,&base);
	//	base = base_get(n);
	//	if(base!= NULL && !strcmp((const char*)base->name,name)){
	//		return n;
	//	}
	//}
	//return NULL;
	return base_findByName(ptr->renderList,name);
}

void* ex_find(const char* name){
	return ex_find_ptr((void*)ex_getIns(),name);
}

/*
添加一个渲染节点
*/
static void 
f_addNode(struct EX* p, void* _node){
	//EngineX* p = this;
	if(_node!=NULL){
		//struct HeadInfo base;
		struct HeadInfo* b;
		//base_get(_node,&base);
		b = base_get(_node);
		if( ex_find_ptr(p,b->name) != NULL){
			log_color(0xff0000,"error! 重名的对象 :%s\n",b->name);
			assert(0);
		}else{
			if(b->curType == TYPE_SPRITE_FLIE){
				sprite_set_z(_node,ex_newPosZ());//设置sprite的z轴坐标
			}
			LStack_push(p->renderList,_node);
		}
	}
}
void ex_add(void* ptr){
	struct HeadInfo* b = base_get(ptr);
	log_color(0,"*** ex_add 0x%0x name=(%s)\n",ptr,b->name);
	f_addNode(ex_getIns(),ptr);
}

void ex_add_fbo(void* fbo){
	if(LStack_findNodeByData(ex_getIns()->fboList,(int)fbo)){
		printf("列表中已经有了这个fbo节点\n");
	}
	LStack_push(ex_getIns()->fboList,fbo);
}
void ex_remove_fbo(void* fbo){
	//ex_getIns()->lock = 1;
	
	//printf("######### 节点删除之前当前的fbo节点数:%d\n",LStack_length(ex_getIns()->fboList));

	if(!LStack_delNode(ex_getIns()->fboList,(int)fbo)){
		printf("删除fbo节点失败!");
		return;
	}
	//printf("######### 当前的fbo节点数:%d\n",LStack_length(ex_getIns()->fboList));
	//ex_getIns()->lock = 0;
}

/*
	根据shader类型获取一个
*/
GLuint ex_getProgrom3D(const char* glslType){
	
	if(!ex_getIns()->pro3d){
		ex_getIns()->pro3d = progrom3d_create();
	}
	return (GLuint)progrom3d_get(ex_getIns()->pro3d,glslType);
}

///*
//获取模型类型
//*/
//int getType(void* p){
//	struct HeadInfo base;
//	base_get(p,&base);	
//	return base.curType;
//}

///*
//*	与引擎绑定headInfo属性
//*/
//void linkBase(struct EX*ex,struct HeadInfo* base){
//
//	//包围盒数据(float*)的数组长度
//	base->boxVertLength = BOX_SIZE;
//}
///*
//	为材质创建贴图
//*/
//static void f_createMaterialTexture(struct GMaterial *p)
//{
//	int i;
//	for(i = 0;i< p->curTexIndex;i++){
//		tmat_bindTex(p,i,(const char*)p->texArray[i]);
//	}
//}
///*
//创建材质
//*/
//static void f_createShader(struct GMaterial *tmat,char* glslType)
//{
//	/*struct GMaterial *tmat=base->tmat;
//     
//	if(!base){
//		printf("创建材质失败!\n");
//		return;
//	}*/
//	//========================
//
//	tmat->glslType = glslType;//base->glslType;
//	
//	//创建材质
//	//加载贴图,贴图要垂直翻转一下,否则要调整uv值
//	
//	f_createMaterialTexture(tmat);
//}

//=================================================================
/*

	1.转化为编辑网格
	2.运行脚本md2Export.ms
	3.勾选Save Anumation (Frame Step设置成1),Generate Normal,Active Time Segment
	4.Export

*	加载并解析md2模型动画
*	字节流序列使用大端方式
*/
static void 
md2_load(struct MD2_Object* _md2,const char* str,int len,const char* name)
{
	struct HeadInfo* base=NULL;
	struct FrameAnim* frameAnim = 0;
	long usetime=get_longTime();

	int numFrames;

	//_md2->parseHandle = (struct MD2_ParseObj*)tl_malloc(sizeof(struct MD2_ParseObj));

	_md2->parseHandle = md2parse_load(str,len);
	numFrames = md2parse_totalFrames(_md2->parseHandle);// _md2->parseHandle->numFrames;



	if(!numFrames){
		printf("关键帧数为0!\n");
		assert(0);
	}

	printf("解析文件[%s]花费%ld毫秒,关键帧数=%d\n",name,get_longTime()-usetime,numFrames);

	_md2->base = base_create(TYPE_MD2_FILE,name,0.0,0.0,0.0);
	base = _md2->base;
	base->parent = _md2;
	//关键帧管理器
	_md2->frameAnim = (struct FrameAnim*)tl_malloc(sizeof(struct FrameAnim));
	frameAnim = _md2->frameAnim;
	memset(frameAnim,0,sizeof(struct FrameAnim));

	frameAnim->frameStart = 0;
	frameAnim->frameEnd = numFrames-1;

	if(numFrames <=0){
		printf("检测到没有关键帧数据\n");
		assert(0);
	}
}

/*
渲染界面
*/
void renderUI(GLenum mode){
	float m = 5.0f;
	char* ui01 = "ui01";
	if (mode == GL_SELECT){
		glPushName ((GLuint)ui01);
	}
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 0.0);
	//
	glOrtho(0.0, 10.0, 0.0, 10.0, -0.0, 1.0);
	//glBegin(GL_QUADS);
	//glColor3f(1.0, 1.0, 0.0);
	//glVertex3i(1, 0, 0);
	//glVertex3i(1, 3, 0);
	//glVertex3i(3, 3, 0);
	//glVertex3i(3, 0, 0);
	//glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f,0.0f,0.0f);  
	glVertex3f(m*1,0.0f,0.0f);  
	glVertex3f(m*1,m*1,0.0f); 

	glVertex3f(0.0f,m*1,0.0f); 
	glVertex3f(m*1,m*1,0.0f); 
	glVertex3f(0.0f,0.0f,0.0f);  



	glEnd();  
	glPopMatrix();
	if (mode == GL_SELECT){
		glPopName ();
	}
}

///*
//设置名字
//*/
//void setName(char* name,const char* targetName,int len){
//	memset(name,0,len);
//	memcpy(name,targetName,strlen(targetName));
//}


static char* f_getFileName(int type){
	switch(type){
		case TYPE_OBJ_FILE:
			return "obj";
		case TYPE_MD5_FILE:
			return "md5";
		case TYPE_MD2_FILE:
			return "md2";
		case TYPE_SPRITE_FLIE:
			return "sprite";
		case TYPE_TEXT_FILE:
			return "text";
		case TYPE_OBJ_VBO_FILE:
			return "obj_vbo";
		case TYPE_FONT_TXT:
			return "font_txt";		
	}
	return "errorType";
}

//Matrix44f ortho;
static void 
f_infoNode(int data){
	//struct EX*e = ex_getIns();	
	struct HeadInfo* base = base_get((void*)data);


	log_color(0xff00ff,"[cam=%0x,curType=%d(%s),(name=%s),visible:%d]\n",base->cam,base->curType,f_getFileName(base->curType),base->name,base_getv((void*)data,FLAGS_VISIBLE));
}
/*
打印引擎当前信息
*/
void 
ex_get_info(){
	struct EX* ex = ex_getIns();
	//EngineX* p = this;
	//struct ECamera cam = ex->cam;

	void* _cam = ex->_3dcam;
	int totleByte,nodeCnt,pg_total;
	int j=0;
	int fps = f_get_fps();
	memory_get_info(&totleByte,&nodeCnt,&pg_total);
	log_color(0,"**********************************************\n");
	//j+=sprintf_s(buffer+j,buffer_size, "FPS	: %d\n",fps);
	log_color(0xffff00,"fps:%ld,ui_z:%.2f\n",fps,ex->ui_pos_z);
	log_color(0x00ff00,"camera 坐标  :      %.3f %.3f %.3f\n",
		cam_getX(_cam),
		cam_getY(_cam),
		cam_getZ(_cam));
	log_color(0xff0000,"camera 角位移: PI * %.3f PI * %.3f PI * %.3f\n",
		cam_getRX(_cam)/PI,
		cam_getRY(_cam)/PI,
		cam_getRZ(_cam)/PI);
	
	log_color(0xff00ff,"draw call渲染节点个数:%d,sw=%.3f,sh=%.3f\n",LStack_length(ex->renderList),ex->_screenWidth,ex->_screenHeight);
	
	//mat4x4_printf("ex->_2dcam",cam_getPerctive(ex->_2dcam));
	{
		/*void* _cam = fbo_get2dCam(ex->fbo);
		mat4x4_printf("fbo->2dcam",cam_getPerctive(_cam));*/
	}
	ex_renderlistCall(f_infoNode);
	
	log_color(0,"屏幕尺寸:%.1f,%.1f ex->3dcam=%0x ex->2dcam=%0x\n",ex->_screenWidth,ex->_screenHeight,ex->_3dcam,ex->_2dcam);
	log_color(0,"程序已执行:%.3f 秒\n",get_longTime()*0.001);
	log_color(0,"内存池已使用 %d bytes(%.3f kb),闲置节点数 %d 总内存池使用 %.3f kb\n",totleByte,(float)(totleByte/1024),nodeCnt,(float)pg_total/1024);
	
	//printf( "%s\n","F4:静态多边形显示线框 \nF12:包围盒显示");
	
	log_color(0, "vbo使用:%d bytes\n",tlgl_getVboSize());
	log_color(0, "当前(射线检测)状态:%d\n",getv(&(ex_getIns()->flags),EX_FLAGS_RAY));

	//mat4x4_printf("perspectiveMatrix",ex->perspectiveMatrix);
	//mat4x4_printf("modelViewMatrix",ex->modelViewMatrix);

	//mat4x4_printf("ui_perspectiveMatrix",ex->ui_perspectiveMatrix);
	//mat4x4_printf("ui_modelViewMatrix",ex->ui_modelViewMatrix);
	//mat4x4_printf("ortho",ortho);
	
	log_color(0,"************ 当前的fbo节点数:%d\n",LStack_length(ex_getIns()->fboList));
	
	map_info(ex->mapptr);
}

/*
 *显示fps
 */
static void
f_drawFps(){
	char _str[G_BUFFER_64_SIZE];
	//sprintf_s(_str, G_BUFFER_64_SIZE,"%ld",/*ex_fps()*/ex_delay_time());
	sprintf_s(_str, G_BUFFER_64_SIZE,"%d %ld",f_get_fps(),g_delayTime);
	ex_showLog(_str);
}

static void
f_show_all_info(){
	struct EX* p = ex_getIns();
	//struct ECamera cam = p->cam;
	char buffer[G_BUFFER_256_SIZE];
	int vbo = tlgl_getVboSize();
	int j;
	int size=-1;
	GLint total_mem_kb,cur_avail_mem_kb;
	int fps = f_get_fps();
	memset(buffer,0,G_BUFFER_256_SIZE);

	tlgl_getGPU_memery(&total_mem_kb,&cur_avail_mem_kb);

	//if(tl_memGetStat())
	{
		//size = (int)tl_memByteSize();
		memory_get_info(&size,0,0);
	}

	j = sprintf_s(buffer, G_BUFFER_256_SIZE,"fps:%d total %d bytes ",fps,size);
	sprintf_s(buffer + j,G_BUFFER_256_SIZE,"vbo:%d bytes (%.3f kb)  cur_avail_mem_kb=%d total_mem_kb=%d,is running %.3f second",
		vbo,
		(float)vbo/1024.0
		/*,cam.x,cam.y,cam.z*/   
		/*,p->allVertexTotal*/
		/*,p->allVertexTotal/3*/
		,cur_avail_mem_kb,total_mem_kb,get_longTime()*0.001);

	ex_showLog(buffer);
}

/*
绘制文本
*/
static void 
drawText(){
	//f_drawFps();	return;
	//显示大量信息
	//f_show_all_info();
}

static void 
drawLine(float h){
	float gap = 100;
	int i;
	int count = 10;
	//int i=0,w;
	//绘制直线
	glLineWidth(1.0f);//直线的宽度。
	//glEnable(GL_LINE_STIPPLE);//绘制虚线
	//glLineStipple(2, 0x0F0F);
	//glUseProgram(0);
	//glColor3f(1,0,0);
	for(i = 0;i < count;i++){
		glBegin(GL_LINES); //绘制实线
		glVertex3f(-h/2, 0,gap*i);
		glVertex3f(h/2, 0.0f ,gap*i);
		glEnd();
	}
	for(i = 0;i < count;i++){
		glBegin(GL_LINES); //绘制实线
		glVertex3f(-gap*i,0,-h/2);
		glVertex3f(-gap*i,0,h/2 );
		glEnd();
	}
}
/*
	添加一个md2模型到引擎

	使用方法:
	gMd2=ex_md2Add(p,"horse",
	"E:\\source\\resource\\md2\\horse.md2",
	"E:\\source/resource/md2/horse.bmp",
	"stand,0,39|run,40,45|attack,46,53|pain,54,65|jump,66,71|filp,72,83|salute,84,94|taunt,95,111|dead,178,197|",1);

	const char* model:	模型路径
	const char* tex:	贴图路径
	char* animConf:		动作配置
	const char* defaultAnim:	默认第一个动作

	lineSize:		线框的尺寸 >0就有线框,否则无
	isCanSelect:	是否可以被选择到

	void (*render)(struct HeadInfo*)	:渲染回调

	int flags	:标示
*/
static struct MD2_Object* 
load_md2(const char* name,const char* model,float x,float y,float z,float scale)
{	
	struct EX* p = ex_getIns();

	int len;

	struct MD2_Frame* frame;

	struct MD2_Object* md2 = (struct MD2_Object*)tl_malloc(sizeof(struct MD2_Object));
	char* data = tl_loadfile(model,&len);
	struct HeadInfo* base = NULL;

	memset(md2,0,sizeof(struct MD2_Object));

	md2_load(md2,(const char*)data,len,name);

	base = md2->base;
	
	tl_free(data);

	//设置坐标
	//tl_set_vec(&base->x,x,y,z);
	base->x = x;
	base->y = y;
	base->z = z;

	base->scale = scale;

	frame =	md2parse_getFrame(md2->parseHandle,0);	// &(md2->parseHandle->pframe[0]);//取第1帧为射线拾取的索引
	tlgl_createRay(&base->rayVertexData,frame->vertices,frame->vertCount);

	printf("创建md2 [%s],每%ld毫秒切换一帧, 坐标x:%.3f,y:%.3f,z:%.3f 当前动作:%s\n",name,md2->frameAnim->fpsInterval,x,y,z,md2->frameAnim->curAnim);

	//默认设置为跑步状态
	//base_curAnim(base->frameAnim,defaultAnim);


	base_updateMat4x4(base);

	f_addNode(p,md2);

	return md2;
}
/*
*	销毁Ent3D对象
*/
static void 
f_ent_dispose(struct Ent3D* p){
	
	tl_free(p->vertex);

	base_dispose(p->base);
	p->base = NULL;

	LStack_delNode(ex_getIns()->renderList,(int)p);
	tl_free((void*)p);
}
/*
*	销毁md2对象
*/
static void 
md2_dispose(struct MD2_Object* _md2)
{
	//销毁动作管理器
	if(_md2->frameAnim){
		tl_free(_md2->frameAnim);
	}

	md2parse_dispose(_md2->parseHandle);

	base_dispose(_md2->base);

	LStack_delNode(ex_getIns()->renderList,(int)_md2);

	tl_free((void*)_md2);
}

void md5Model_dispose(struct MD5* _md5)
{
	md5_dispose(_md5);
	
	base_dispose(_md5->base);
	_md5->base = NULL;

	LStack_delNode(ex_getIns()->renderList,(int)_md5);

	tl_free((void*)_md5);
}

static void 
f_ent_render(struct Ent3D* ent){
	struct HeadInfo* base =(struct HeadInfo*)ent->base;

	struct VertexData* p=&(base->rData);
	//设置数据
	p->vertex = ent->vertex;
	p->vertLen= ent->vertexCount;
	//p->vbo = ent->vbo;
	
	//实体渲染
	base_renderFill(base);
}

//static void
//callMat4x4(struct HeadInfo* base)
//{
//	if(base->bMat4x4Update)
//	{
//		updateMat4x4(base);
//		base->bMat4x4Update = 0;
//	}
//}

/*
	获取顶点总数
*/
static int
getAllVertex(int data)
{
	/*struct HeadInfo* base = base_get2((void*)data);
	int objType = base->curType;
	if(objType == TYPE_OBJ_VBO_FILE)
	{
		struct Obj_vbo_model* p = (struct Obj_vbo_model*)data;
		struct ObjVBO* vbo = p->vboPtr;
		return vbo->vertCount;
	}*/
	return 0;
}

/*
渲染一个节点
*/
void 
ex_render3dNode(int data)
{
	struct EX*ex = ex_getIns();
	void* checkcam = ex->_3dCurCam;
	struct HeadInfo* base = base_get((void*)data);
	int objType = base->curType;
	void* targetCam = base->cam;

	if(objType == TYPE_SPRITE_FLIE){
		checkcam = ex->_2dCurCam;
	}

	//if(!targetCam && checkcam == ex-)
	
	//if(!targetCam && ex->_2dCurCam == ex->_2dcam){
	if(!targetCam){
		//没有指定任何cam
		
		//log_color(0,"************name = (%s) type=%d 没有指定任何cam\n",base->name,base->curType);
		
		
		//getchar();
		return;
	} 
	
	if(checkcam!=targetCam){
		//printf("======>name:%s\n",base->name);
		return;//不在同一个渲染矩阵空间,返回之
	}

	base_realUpdateMat4x4(base);
	if(objType== TYPE_OBJ_FILE)
	{
		f_ent_render((struct Ent3D*)data);
	}
	else if(objType == TYPE_MD5_FILE)
	{
		struct MD5* md5 = (struct MD5*)data;
		float f = md5->fpsInterval <= 0 ? 0.0f : (1000.0f/md5->fpsInterval);//f代表 f毫秒播放一个关键帧,f = 0的时候停止在当前关键帧
		md5_render(md5,f);//ex->fps	

		if(getv(&base->flags,FLAGS_RENDER_DRAWSKELETON)){
			md5_drawSkeleton((void*)data);//绘制骨骼
		}
		
	}
	else if(objType == TYPE_MD2_FILE)
	{
		md2_render((struct MD2_Object*)data);
	}
	else if(objType == TYPE_OBJ_VBO_FILE)
	{
		node_render((struct Node*)data);
		//e->allVertexTotal+=getAllVertex(data);
	}
	else if(objType == TYPE_SPRITE_FLIE)
	{
		sprite_drawRender(data);
	}
}
/*
	回调
*/
void 
ex_renderlistCall(void _callBack(int))
{
	struct EX* ex = ex_getIns();
	struct LStackNode* s = (struct LStackNode* )ex->renderList;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		_callBack(data);
	}
}

//===========================================================
struct AnimCheck
{
	int allFrameCount;
	int isCorrect;//检测标记
};
static void f_animSpliteCheck(int* inputParms,char* splitStr)
{
	struct AnimCheck* anim = (struct AnimCheck*)inputParms;
	if(tl_isNum(splitStr))
	{
		//printf("%s::%d\n",splitStr,atoi(splitStr));
		int animIndex = atoi(splitStr);
		int needFrame = animIndex + 1;
		if(needFrame>anim->allFrameCount){
			printf("关键帧帧配置溢出 总关键帧数:%d,需要读取第(%d)关键帧数据,溢出!!!\n",anim->allFrameCount,needFrame);
			anim->isCorrect = 0;
		}
	}
}

/*
	逐行解析字符
*/
static void f_animObjSplit(int* inputParms,char* splitStr)
{
	str_split((const char*)splitStr,',',f_animSpliteCheck,inputParms);
}
/*
		"stand,0,51@run,40,45@"
		检查md2动作文件配置是否正确,检查关键帧是否溢出,正确返回1,错误返回0
*/
static int check_md2AnimConf(int allFrameCount,const char* animConf)
{
	struct AnimCheck anim;
	anim.isCorrect = 1;
	anim.allFrameCount = allFrameCount;
	str_split((const char*)animConf,'@',f_animObjSplit,(int*)&anim);
	return anim.isCorrect;
}

//void mat4x4_zero_d(GLdouble m[16]){
//	int i;
//	for(i= 0; i <16; i++)
//		m[i] =0;
//}
//void mat4x4_identity_d(GLdouble m[16]){
//	mat4x4_zero_d(m);
//	m[0] =1.0;
//	m[5]  =1.0;
//	m[10] =1.0;
//	m[15] =1.0;
//}

//GLdouble outModel[16];
/*
static void
changeCam()
{

	struct EX* p = ex_getInstance();
	Matrix44f rx,ry,rz,temp1,temp2;
	Matrix44f pos;
	struct ECamera cam = p->cam;
	
	mat4x4_zero(temp1);
	mat4x4_zero(temp2);

	mat4x4_identity(pos);
	mat4x4_translate(pos,cam.x,cam.y,cam.z);

	mat4x4_zero(p->modelViewMatrix);

	//rx
	mat4x4_identity(rx);
	mat4x4_rotateX(rx,p->camRotateX);

	//ry
	mat4x4_identity(ry);
	mat4x4_rotateY(ry,p->camRotateY);

	//rz
	mat4x4_identity(rz);
	mat4x4_rotateZ(rz,p->camRotateZ);
	
	multi2(temp1,rx,ry);
	multi2(temp2,rz,temp1);
	//multi2(temp1,rx,rz);
	//multi2(temp2,ry,temp1);


	multi2(p->modelViewMatrix,pos,temp2);
}
*/

/*
//构造模型矩阵,将cam矩阵左乘到模型矩阵
static void
f_get_custom_modelMatrix(Matrix44f m,struct ECamera* pcam){
	//struct EX* p = ex_getIns();
	//struct ECamera cam = *pcam;//p->cam;
	void* _cam = ex_getIns()->_3dcam;


	Matrix44f xyz,rx,ry,rz;
	//x,y,z坐标
	mat4x4_identity(xyz);
	mat4x4_translate(
		xyz,
		cam_getX(_cam),
		cam_getY(_cam),
		cam_getY(_cam));

	//旋转===================================================
	//rx
	mat4x4_identity(rx);
	mat4x4_rotateX(rx,-cam_getRX(_cam));

	//ry
	mat4x4_identity(ry);
	mat4x4_rotateY(ry,-cam_getRY(_cam));

	//rz
	mat4x4_identity(rz);
	mat4x4_rotateZ(rz,-cam_getRZ(_cam));

	mat4x4_identity(m);
	
	mat4x4_mult(4,m,rx,ry,rz,xyz);

	//################################
	//tmp = m[7];
	//m[7] = m[14];
	//m[14] = tmp;

	//tmp = m[11];
	//m[11] = m[13];
	//m[13] = tmp;

	mat4x4_transpose(m);
}
*/
//static void
//f_getModelMat4x4(){
//	struct EX* p = ex_getIns();
//	//struct ECamera cam = p->cam;
///*
//	glMatrixMode (GL_MODELVIEW);
//	//glGetFloatv(GL_MODELVIEW_MATRIX,p->modelViewMatrix);
//	glLoadIdentity ();
//	
//	glRotatef(-180/PI*p->cam.rx,1,0,0);
//	glRotatef(-180/PI*p->cam.ry,0,1,0);
//	glRotatef(-180/PI*p->cam.rz,0,0,1);
//	glTranslatef(cam.x,cam.y,cam.z);
//	
//	mat4x4_zero(p->modelViewMatrix);
//	glGetFloatv(GL_MODELVIEW_MATRIX,p->modelViewMatrix);
//	mat4x4_transpose(p->modelViewMatrix);
//*/
//	//tlgl_get_matrix(p->modelViewMatrix,
//		//cam.x,cam.y,cam.z,
//		//cam.rx,cam.ry,cam.rz);
//
//	cam_refreshModdel(p->_3dcam);
//}

//用固定管线计算的透视矩阵和模型矩阵
//static void
//f_used_normal_perctive(GLdouble fov, GLdouble aspectRatio,
//					   GLdouble zNear, GLdouble zFar){
//	struct EX* p = ex_getIns();
//	Matrix44f _out;
//
//
//	//struct ECamera cam = p->cam;
//	//只是处理鼠标拾取操作用来获取坐标使用gluUnProject,3d物体 不使用该方式(使用用户自定义的透视矩阵,视图矩阵,模型矩阵)
//	glMatrixMode (GL_PROJECTION);
//	glLoadIdentity ();
//	gluPerspective (fov,aspectRatio,zNear,zFar);
//
//	mat4x4_identity(_out);
//	glGetFloatv(GL_PROJECTION_MATRIX,_out);
//	mat4x4_transpose(_out);	
//	
//	//mat4x4_copy(_out,p->perspectiveMatrix);
//
//	//f_getModelMat4x4();
//	ex_refresh3dModelView();
//}
//
//static void
//updatePerspectiveMatrix( 
//						GLdouble fov, GLdouble aspectRatio,
//						GLdouble zNear, GLdouble zFar){
//	struct EX* p = ex_getIns();
//
//	//struct ECamera cam = p->cam;
//	
//	//f_used_normal_perctive(fov,aspectRatio,zNear,zFar);return;//使用固定管线计算出透视矩阵和模型矩阵
//
//	//模型矩阵
//	//f_get_custom_modelMatrix(p->modelViewMatrix,&p->cam);//使用自定义计算出模型矩阵	
//	f_getModelMat4x4();
//
//	//透视矩阵
//	/*
//	mat4x4_identity(p->perspectiveMatrix);
//	mat4x4_perspective(p->perspectiveMatrix,fov,aspectRatio,zNear,zFar);
//	mat4x4_transpose(p->perspectiveMatrix);	
//	*/
//	//cam_setPerspect(p->_3dcam,fov,aspectRatio,zNear,zFar);
//}

///*
//	计算透视和矩阵索引
//*/
//void 
//ex_refresh3dModelView(){
//	cam_refreshModdel(ex_getIns()->_3dcam);
//}

/*
	绘制线段
*/
//static void 
//f_drawline(){

	/*
	glLineWidth(1.0f);//直线的宽度。

	//for(i = 0;i < count;i++){
		glBegin(GL_LINES); 
		//glVertex3f(-h/2, 0,gap*i);
		glVertex3f(0.0, 0.0f ,0.0f);
		glVertex3f(1.0, 0.0f ,0.0f);
		glEnd();
	//} 
	*/
	//if(ex_getInstance()->drawLine_callBack)
	//	ex_getInstance()->drawLinhe_callBack();
//}

///*
// *计算正交矩阵,只需要在渲染窗口发生尺寸变化的重新计算一次即可
// */
//void ex_calculat_ortho(){
//	struct EX* p = ex_getIns();
//	//mat4x4_identity(p->ui_perspectiveMatrix);
//	//mat4x4_orthoPerspect(p->ui_perspectiveMatrix,0,p->_screenWidth,0,p->_screenHeight,0,-p->allzBuffer);
//	void* _2dCam = p->_2dcam;
//	cam_setOrtho(_2dcam,0,p->_screenWidth,0,p->_screenHeight,0,-p->allzBuffer);
//
//
//	//mat4x4_identity(p->ui_modelViewMatrix);
//	//mat4x4_transpose(p->ui_modelViewMatrix);
//	//printf("ex_calculat_ortho===>\n");
//}

//固定管线计算正交矩阵
static void
f_static_calculat(){
	struct EX* p = ex_getIns();
			
	//计算正交矩阵

	//界面渲染,切换为正交视图
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, p->_screenWidth, 0, p->_screenHeight, 0, -p->allzBuffer);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*
	mat4x4_identity(p->ui_modelViewMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX,p->ui_modelViewMatrix);
	mat4x4_transpose(p->ui_modelViewMatrix);

	mat4x4_identity(p->ui_perspectiveMatrix);
	glGetFloatv(GL_PROJECTION_MATRIX,p->ui_perspectiveMatrix);
	mat4x4_transpose(p->ui_perspectiveMatrix);
	*/
}

//默认的帧缓冲区
static void
f_defaultRenderFrame(){
	struct EX* p = ex_getIns();
	
	//切换对应的cam的矩阵空间
	ex_switch3dCam(p->_3dcam);
	ex_switch2dCam(p->_2dcam);
	// Reset FBO. Draw world again from the real cameras perspective
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//切换到前台帧缓冲,默认的窗口帧缓冲
	glDrawBuffers(1, windowBuff);
	glViewport (0, 0, p->_screenWidth, p->_screenHeight);
	glClearColor(p->bg_r,p->bg_g,p->bg_b,1);				//清除背景并填充背景为白色
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清空颜色缓冲区和深度缓冲区

	//深度测试
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	//ex_updatePerspctiveModelView();
	//p->allVertexTotal = 0;
	ex_renderlistCall(ex_render3dNode);//渲染节点
	
	//drawLine(200);

	//drawLine(2000);
	//f_drawline();

	//ex_lua_global_evt_dispatch(EVENT_ENGINE_RENDER_3D);//这个一直在给lua模块发送数据,性能不好,需要优化

	//f_renderlistCall(sprite_drawRender);//渲染2d节点

	//废弃此字体的渲染,一律采用将文本数据copy到Sprite渲染出来
	ex_renderlistCall(tf_render);

	//渲染文本(非常耗费性能,待优化)
	// 2019.8.26 此处已经优化成Sprite模式的字体渲染
	//if(getv(&p->flags,EX_FLAGS_DRAW_DEBUG_TEXT)) {drawText(p);}
}
static void 
f_fboRenderList(int data,int parms){
	fbo_render((void*)data);
}

//渲染fbo列表
static void
f_renderFBOs(){	
	//if(!ex_getIns()->lock)
		LStack_ergodic_t(ex_getIns()->fboList,f_fboRenderList,0);
}

static void 
_new(){
	struct EX* p = ex_getIns();

	//long _time;
	if(p->_screenWidth <= 0 || p->_screenHeight<=0){
		//屏幕尺寸0的时候不进行渲染
		return;
	}
	//计算fps
	f_calculate_fps();
	g_delayTime = get_longTime() - _longTime;
	_longTime =  get_longTime();
	
	f_defaultRenderFrame();
	//fbo_render(p->fbo);	

	

	f_renderFBOs();

	//Do the buffer Swap
	glutSwapBuffers();
	// Do it again
	glutPostRedisplay();

	//渐变管理器回调
	//ramp_callBack();

	tween_run(_longTime,g_delayTime);
	
	runLastList();
	p->index++;
	
	evt_dispatch(ex_getIns(),EVENT_ENGINE_RENDER_3D,0);
	timelater_run();
}
void ex_render(void)
{
	_new();
}

struct HeadInfo* ex_find_headinfo(struct EX* p,const char* name){
	return base_get(ex_find_ptr(p,name));
}

void 
ex_update_uiPos()
{
	ex_renderlistCall(sprite_updatePos);	
}
//
///*
//	加载一个四边形用于sprite面片
//*/
//static void 
//loadSpriteVert(struct EX* p)
//{
//	char* _objStr;
//	int _dataCount,verts;
//	_objStr=tl_loadfile("\\resource\\obj\\quad.obj",0);
//	obj_parse(_objStr,&_dataCount,&verts,OBJ_UV_VERTEX);
//	p->spriteVert.vertLen = _dataCount;
//	p->spriteVert.vertex = (GLfloat*)verts;
//
//	tl_free(_objStr);
//}
//static float _g_pos_z;
float ex_newPosZ()
{
	struct EX* p = ex_getIns();
	//return p->zBuffer++;//这里存疑,UI正交zbuffer叠加的影响
	 p->ui_pos_z+=0.01;

	//printf("==>%f\n",p->ui_pos_z);
	return p->ui_pos_z;
}

static void
initPerspective(Matrix44f mat,float r,float t,float b,float n,float f,float l)
{
	mat[0] = 2/(r-l);
	mat[1] = 0;
	mat[2] = 0;
	mat[3] = -(r+l)/(r-l);
	mat[4] = 0;
	mat[5] = 2/(t-b);
	mat[6] = 0;
	mat[7] = -(t+b)/(t-b);
	mat[8] = 0;
	mat[9] = 0;
	mat[10] = 2/(n-f);
	mat[11] = (n+f)/(n-f);
	mat[12] = 0;
	mat[13] = 0;
	mat[14] = 0;
	mat[15] = 1;
}
static void
f_stage_click_callback(struct Sprite* self,int x,int y){
	//printf("f_stage_click_callback %d,%d\n",x,y);
}

////初始化一个设置坐标和cam角度的fbo
//static void
//f_init_fbo(void* fbo){
//	void* cam = fbo_get3dCam(fbo);
//	//void* spr = sprite_createEmptyTex(256,256);
//
//	//void* mat = sprite_get_material(spr);
//	//tmat_pushTex(mat,(GLuint)fbo_getTex(fbo));
//
//	//base_resetv(spr,FLAGS_VISIBLE);
//	//sprite_setpos((struct Sprite*)spr,100,100);
//	//ex_getIns()->fboTexSprite = spr;
//
//	//ex_getIns()->fboTexture = fbo_getTex(fbo);
//
//	cam_setZ(cam,-3);
//	cam_setRX(cam,PI*1.8);
//	cam_refreshModel(cam);
//	
//	{
//		/*void*	ptr = sprite_create("a",0,0,50,50,0);
//		sprite_
//		fbo_pushNode(fbo,ptr);*/
//	}
//}

////初始化场景,舞台和设备上下文设备都处理完成了
//static void
//f_initScene(){
//	struct EX* p = ex_getIns();
//	//**************************************************
//	//初始化一个fbo texture对象
//	//p->fbo = fbo_init(256,256);
//	//f_init_fbo(p->fbo);
//	
//	//ex_reshape(sw,sh);
//
//	//在这里可以初始化一些 引擎需要的配置等文件
//	evt_dispatch(ex_getIns(),EVENT_ENGING_INIT,0);
//}

//构造stage2d舞台
static int 
f_init_stage2d(struct EX* p,float w,float h){
	if(!p->stage2d){
		//初始化2d stage
		p->stage2d = sprite_create("stage2d",0,0,32,32,f_stage_click_callback,0);
		ex_add(p->stage2d);
		
		//base_resetv(p->stage2d,FLAGS_VISIBLE);
		//stage2d = (struct Sprite*)p->stage2d;
		//sprite_set_default_tex(p->stage2d);
		p->curFocus = p->stage2d;

		//stage2d舞台初始化完成,通知可以初始化了
		//f_initScene();
		//evt_dispatch(ex_getIns(),EVENT_ENGING_INIT,0);
		return 1;
	}
	return 0;
}
void 
ex_resize_stage2d(){
	struct EX* p = ex_getIns();
	if(!f_init_stage2d(p,p->_screenWidth,p->_screenHeight)){
		sprite_resize(p->stage2d,p->_screenWidth,p->_screenHeight);
	}
	sprite_set_hit_rect(p->stage2d,0,0,p->_screenWidth,p->_screenHeight);
}

void 
ex_reshape(int w,int h){
	struct EX* p = ex_getIns();
	void* _2dcam = p->_2dcam;
	if(w == 0 || h == 0){
		return;
	}

	p->_screenWidth = w;
	p->_screenHeight = h;

	ex_update_uiPos();

	glViewport (0, 0, (GLsizei)w, (GLsizei)h);

	//刷新3d齐次坐标矩阵,只会在渲染帧缓冲区尺寸发生变化的时候会调用该接口
	cam_setPerspect(p->_3dcam,45.0, (GLdouble)p->_screenWidth/(GLdouble)p->_screenHeight, 0.1, p->zFar);

	//计算正交透视矩阵,用于2d界面,计算正交矩阵,只需要在渲染窗口发生尺寸变化的重新计算一次即可
	cam_setOrtho(_2dcam,p->_screenWidth,p->_screenHeight,-p->allzBuffer);
	
	//fbo_resize(p->fbo);

	ex_resize_stage2d();
	
	//printf("%d,%d\n",sizeof(struct HeadInfo),sizeof(struct LStackNode));

	ex_lua_global_evt_dispatch(EVENT_ENGINE_RESIZE);
	//printf("============> %d,%d\n",w,h);

	evt_dispatch(ex_getIns(),EVENT_ENGING_INIT,0);
}

void 
ex_init(struct EX* p,GLdouble zfar,float sw,float sh){	
	//p->allzBuffer在-90000初始化的时候会被模型挡在后面
	//计算出所有的透视模型最小深度
	p->allzBuffer = -90000;	//初始化一个Z深度,此深度标识3d层的
	p->ui_pos_z =  -1000;	//此深度如果小于3d层,那么界面将在3d界面后面
	p->_screenWidth = sw;
	p->_screenHeight= sh;

	p->mapptr = map_create();
	p->_3dcam = cam_create();//初始化3d透视camera
	ex_switch3dCam(p->_3dcam);
	p->_2dcam = cam_create();//初始化2d正交camera
	ex_switch2dCam(p->_2dcam);
	//p->zBuffer = p->allzBuffer+1;
	p->clickInfo = tl_malloc(sizeof(struct ClickInfo));
	{
		struct ClickInfo* _click = (struct ClickInfo*)p->clickInfo;
		_click->sprite = 0;
	}
	p->zFar = zfar;
	p->renderList = LStack_create();

	p->fboList = LStack_create();
	p->lastList = LStack_create();
	/*p->hit=hit_create(p->renderList,p->viewport,p->hitModelView,p->hitPerspectProjection);*/
	
	//==========================================
	//loadSpriteVert(p);
	p->evtList = (void*)LStack_create();
	p->mouseState.action = GLUT_UP;
}

void ex_dispose(struct EX* p){
	printf("销毁引擎设备!\n");
	//getch();
	
	cam_dispose(p->_3dcam);
	p->_3dcam = 0;
	cam_dispose(p->_2dcam);
	p->_2dcam = 0;

	tl_free(p->clickInfo);
	p->clickInfo = 0;

	atals_dispose(p->atals);
	p->atals = 0;

	LStack_delete(p->renderList);
	p->renderList = 0;

	LStack_delete(p->fboList);
	p->fboList = 0;

	ft_free(p->ft);
	evt_dispose(p);
	memory_gc();
	
	/*if(p->spriteVert.vertex){
		tl_free(p->spriteVert.vertex);
	}*/
}

/*
	加载md5模型
*/
static void* 
load_md5(const char* _name,const char* url,float x,float y,float z,float scale)
{
	struct EX* engine = ex_getIns();
	struct MD5* md5 = NULL;
	struct HeadInfo* _base = NULL;
	
	//================================================
	md5 = (struct MD5*)tl_malloc(sizeof(struct MD5));
	memset(md5,0,sizeof(struct MD5));
	md5->custframe = -1;
	_base=base_create(TYPE_MD5_FILE,_name,x,y,z);
	_base->parent = md5;
	md5->base = _base;
	
	_base->rayVertexData.vertex = 0;
	_base->rayVertexData.vertLen = 0;

	_base->scale = scale;//缩放值

	////静态拾取盒子
	//_base->boundLength = 30.0f * scale;
	//_base->boundHeight = 40.0f * scale;
	//_base->boundWidth = 60.0f *  scale;

	/*base_boxStaticVert_init(_base);
	tl_createAbsBoundBox(_base->boundLength,_base->boundHeight,_base->boundWidth,_base->staticBoxVert);*/
	_base->rayVertexData.vertLen = BOX_SIZE;
	_base->rayVertexData.vertex = tl_malloc(sizeof(float)*_base->rayVertexData.vertLen);
	tlgl_setAbsBoundBox(1,1,1,_base->rayVertexData.vertex);//默认一个半径0.5的立方体
	
	//======================================================================================
	//加载md5模型
	md5_loadMesh((struct MD5* )md5,url);
	_base->curType = TYPE_MD5_FILE;
	base_updateMat4x4(_base);

	//添加到渲染列表
	f_addNode(engine,md5);

	return md5;
}
/*
	为obj解析mesh数据
*/
static void f_end3d_loadMesh(struct Ent3D* ent,const char* path)
{
	int _dataCount;
	int verts;
	char* _objStr = tl_loadfile(path,NULL);

	//obj模型解析
	obj_parse(_objStr,&_dataCount,&verts,OBJ_UV_VERTEX_NORMAL);

	ent->vertex = (GLfloat*)verts;
	ent->vertexCount =_dataCount/OBJ_SPAN_COUNT;

	tl_free(_objStr);
}

/*
	因为是静态模型,所以可以计算出检测边界值,所以这里采用优化处理的方式
	创建ent 解析OBJ静态模型对象 使用VBO
	flags:	标识
*/
static struct Ent3D* 
load_obj(const char* name,const char* mesh_s,
	float x,float y,float z,float scale)
{
	struct EX* engine = ex_getIns();

	//EngineX* engine = this;
	struct HeadInfo* base;

	struct Ent3D* ent = (struct Ent3D*)tl_malloc(sizeof(struct Ent3D));

	ent->base = base_create(TYPE_OBJ_FILE,name,x,y,z);
	
	base  = (struct HeadInfo*)ent->base;
	base->parent = ent;

	//设置缩放
	base->scale = scale;
	
	//绑定x,y,z引用
	//tl_set_vec(&base->x,x,y,z);
	base->x = x;
	base->y = y;
	base->z = z;

	//设置名字
	//setName(base->name,name,G_BUFFER_32_SIZE);

	f_end3d_loadMesh(ent,mesh_s);//加载解析数据

	tlgl_createRay(&base->rayVertexData,ent->vertex,ent->vertexCount);

	base_updateMat4x4(base);

	//添加到渲染列表
	f_addNode(engine,ent);

	return ent;
}

void*
ex_load_model(char* name,const char* url,int mode){

	switch(mode){
		case E_RenderModeNormal:
			{
				float x,y,z;
				float scale = 1.0;
				
				char suffix[G_BUFFER_16_SIZE];
				x = y = z = 0;
				tl_getSuffixByPath((char*)url,suffix,G_BUFFER_16_SIZE);
				if(!strcmp(suffix,"obj"))
				{
					return (void*)load_obj(name,url,x,y,z,scale);
				}
				else if(!strcmp(suffix,"md2"))
				{
					return (void*)load_md2(name,url,x,y,z,scale);
				}
				else if(!strcmp(suffix,"md5mesh"))
				{
					return (void*)load_md5(name,url,x,y,z,scale);
				}
				return 0;
			}
		case E_RenderModeVBO:
			{
				void* n = ex_loadVBO(name,url);
				f_addNode(ex_getIns(),n);
				return n;
			}
	}
	return 0;
}

/************************************************************************/
/* vbo模式加载md2模型                                                            */
/************************************************************************/
/*
	添加md2关键帧顶点数据
*/
static void
pushMd2Frame(struct Obj_vbo_model* ptr,void* _md2Parse,int gap)
{
	int i;
	struct MD2_Frame* frame;
	
	for(i = 0;i </* _md2Parse->numFrames*/md2parse_totalFrames(_md2Parse);i++)
	{
		frame = md2parse_getFrame(_md2Parse,i);//&(_md2Parse->pframe[i]);
		//md2parse_getFrameVertex(frame,&vertexData.vertex,&vertexData.vertLen);
		//struct VertexData vertexData;
		//objVBO_pushNode(ptr,vertexData.vertex,vertexData.vertLen * gap);//压入一个关键帧数据到VBO
		objVBO_pushNode(ptr,frame->vertices,frame->vertCount * gap);//压入一个关键帧数据到VBO
	}
}

int 
vbo_md2Load(const char* name,const char* url)
{
	char* _objStr;
	int fileSize;
	//struct Obj_vbo_model* ptr;
	//md2解析器,负责解析数据
	void* _parse;

	struct Node* node;
	//int gap; //= UV_GAP+NORMAL_GAP+VERTEX_GAP;
	
	int dataType = OBJ_UV_VERTEX_NORMAL;//uv,顶点,法线
	node = node_create(name);
	//base_set_suffix(node->base,url);

	node->ptrVBO = objVBO_create(name,dataType);
	_objStr=tl_loadfile((char*)url,&fileSize);
	//////////////////////////////////////////////////////////////////////////
	//_parse=(struct MD2_ParseObj*)tl_malloc(sizeof(struct MD2_ParseObj));
	_parse=md2parse_load(_objStr,fileSize);
	
	pushMd2Frame(node->ptrVBO,_parse,ex_get_gap(dataType));

	//构造动作管理器
	//ptr->anim = anim_create(ptr->ptrList);
	node_bindAnim(node);
	
	//设置动作播放帧频
	anim_set_fps(node->anim,7);

	//释放资源.
	md2parse_dispose(_parse);
	tl_free(_objStr);
	
	//f_addNode(ex_getIns(),node);

	//printf("[%s]动作总数:%d\n",url,node->anim->allLength);
	return (int)node;
}
/*
	vbo加载一个OBJ格式的模型
	int bCollideBox	是否有碰撞检测盒
*/
static int 
vbo_loadObj3d(char* name,const char* url)
{
	char* _objStr;
	int parseType;
	int verts,_bufferSize;
	struct Node* node;

	const int useIndexExport = 1;//使用索引方式导入

	//printf("****\n");

	node = node_create(name);
	_objStr=tl_loadfile(url,0);
	
	//设置后缀
	//base_set_suffix(node->base,url);

	//数据的组织方式
	parseType =	OBJ_UV_VERTEX_NORMAL;
	//parseType = OBJ_UV_VERTEX;
	//解析obj数据
	obj_parse((char*)_objStr,&_bufferSize,&verts,parseType);

	//////////////////////////////////////////////////////////////////////////
	node->ptrVBO = objVBO_create(name,parseType);

	if(useIndexExport)
	{
		//使用索引方式,内存占用比较小,但是UV和normal会重用
		objVBO_pushExportObj(node->ptrVBO,_objStr);
	}
	else
	{
		//使用默认的顶点组织方式,内存占用比较大
		objVBO_pushNode(node->ptrVBO ,(GLfloat*)verts,_bufferSize);
	}

	//////////////////////////////////////////////////////////////////////////
	
	/*if(bCollideBox)
		base_createRayVertex(&node->collideBox,(float*)verts,_bufferSize/(UV_GAP+VERTEX_GAP+NORMAL_GAP));*/
	
	//构造动作管理器
	//ptr->anim = anim_create(ptr->ptrList);
	node_bindAnim(node);

	tl_free((void*)verts);
	tl_free(_objStr);

	//node->base = node->ptrVBO->base;
	//f_addNode(ex_getIns(),node);
	return (int)node;
}
//加载VBO模式的模型
void* 
ex_loadVBO(char* name,const char* url){
	char suffix[G_BUFFER_16_SIZE];
//#ifdef DEBUG
//	log_color(0x00ff00,"VBO模式创建模型对象:(%s)[%s]\n",name,url);
//#endif
	tl_getSuffixByPath((char*)url,suffix,G_BUFFER_16_SIZE);
	if(!strcmp(suffix,"obj"))
	{
		return (void*)vbo_loadObj3d(name,url);
	}
	else if(!strcmp(suffix,"md2"))
	{
		//return (int)load_md2(name,url,x,y,z,scale);
		return (void*)vbo_md2Load(name,url);
	}
	else if(!strcmp(suffix,"md5mesh"))
	{
		//return (int)load_md5(name,url,x,y,z,scale);
	}
	printf("模型加载方式未实现...\n");
	assert(0);
	return 0;
}


//void 
//setv_ptr(void* ptr,int flags)
//{
//	struct HeadInfo* b = base_get(ptr);
//	//setv(&((struct HeadInfo*)base_get2(ptr)->flags),flags);
//	setv(&b->flags,flags);
//}
//为ptr设置材质material
//void 
//setMaterial(void* ptr,void* material)
//{
//	(struct HeadInfo*)base_get(ptr)->tmat = material;
//}

/*
	加载md2动作配置
*/
int 
ex_load_anim_config(void* ptr,char* animConf,long fps)
{
	//const char* defaultAnim,
	struct HeadInfo* base = base_get(ptr);
	int animConfLen = (int)strlen(animConf);
	if(animConfLen>G_BUFFER_128_SIZE){
		printf("参数animConf 动作配置太长 len:%d\n",animConfLen);
		assert(0);
		return 0;
	}

	if(base->curType == TYPE_MD2_FILE)
	{
		struct MD2_Object* md2 = (struct MD2_Object* )ptr;

		//检查关键帧配置是否异常
		if(!check_md2AnimConf(md2parse_totalFrames(md2->parseHandle)/*md2->parseHandle->numFrames*/,animConf))
		{
			assert(0);
			return 0;
		}
		memcpy(md2->frameAnim->animConfig,animConf,strlen(animConf));

		md2->frameAnim->fpsInterval = 1000 / fps;
		//base_curAnim(base->frameAnim,defaultAnim);
	}
	else if(base->curType == TYPE_MD5_FILE)
	{

	}
	return 1;
}

/*
	设置动作
*/
void 
ex_set_anim(void* ptr,const char* animKey)
{
	struct HeadInfo* base = base_get(ptr);
	if(base->curType == TYPE_MD2_FILE)
	{
		struct MD2_Object* md2 = (struct MD2_Object*)ptr;
		frame_set(md2->frameAnim,animKey);
	}
	else if(base->curType == TYPE_MD5_FILE)
	{
		md5_setAnim((void*)ptr,animKey);
	}
	else{
		printf("类型:%d未实现\n",base->curType);
		assert(0);
	}
}
int 
ex_animtor_ptr_setcur(void* ptr,const char* str,void (*playend)(void*)){
	struct Node* node = (struct Node*)ptr;
	struct Animtor* animtor=anim_get_animtor(node->anim);

	if(!animtor_setcur(animtor,(char* const)str))
	{
		struct HeadInfo* base = base_get(ptr);
		printf("为(%s)设置动作:%s失败\n",base->name,str);
		log_code(ERROR_SET_FRAME);
		return 0;
	}else{
		animtor->playend = playend;
	}
	return 1;
}
//static int 
//OnMouseDown(struct Sprite* pBtn,int mousex, int mousey)
//{
//	struct EX* ex = f_getEx();
//	//鼠标的位置：mousex，mousey坐标系是原点位于左上角
//	//必须将mousey变换到原点位于左下角的坐标系中
//	mousey = ex->_screenHeight-mousey;
//	if( mousex > pBtn->m_fPosX && mousex < pBtn->m_fPosX+pBtn->m_fWidth &&
//		mousey > pBtn->m_fPosY && mousey < pBtn->m_fPosY+pBtn->m_fHeight )
//	{
//		//printf("button is pressed .... 激活一些按钮特效或者处理一些业务逻辑\n");
//		pBtn->m_bPressed = 1;
//
//		return 1;
//	}
//	return 0;
//}

/*
	界面拾取获取的信息
*/
struct HitUiInfo
{
	//拾取的局部坐标
	int localX;
	int localY;

	//拾取的全局坐标
	int x;
	int y;
};
/*
	射线拾取ui测试
*/
static int
hitUiTest(struct Sprite* spr,float x,float y,struct HitUiInfo* outptInfo)
{
	struct Vec3 pos;
	struct Vec3 nearPoint;
	struct Vec3 farPoint;
	struct HitResultObject hit;

	memset(&hit,0,sizeof(struct HitResultObject));
	memset(&pos,0,sizeof(struct Vec3));

	nearPoint.x = x;
	nearPoint.y = y;
	nearPoint.z = 10;
	
	farPoint.x = x;
	farPoint.y = y;
	farPoint.z = -10;

	tl_pickTriangle(spr->hitTriangle,SPRITE_TRIANGLE_COUNT,&nearPoint,&farPoint,&pos,&hit);
	if(hit.isHit){
		outptInfo->localX = hit.x - spr->screenX;
		outptInfo->localY = hit.y - spr->screenY;
		outptInfo->x = hit.x;
		outptInfo->y = hit.y;
		//printf("拾取状态:%d,	 拾取的坐标%3f,%3f  原点坐标 %.3f,%.3f\n",hit.isHit,hit.x,hit.y,x,y);
		return 1;
	}
	return 0;
}

static void 
render_hitUiNode(int data){
	if(sprite_isEnable(data))
	{
		struct EX* p = ex_getIns();

		struct ClickInfo* _clickInfo = p->clickInfo;

		struct HeadInfo* base = base_get((void*)data);
		//renderSprite(data);
		struct Sprite* spr = (struct Sprite*)data;
		struct HitUiInfo info;
		if(sprite_isCanClick((void*)spr))
		{
			int ox,oy;
			void* cam = base_get_cam(spr);
			cam_get_2dxy(cam,&ox,&oy);


			if(hitUiTest(spr,p->mouseState.xMouse-ox,p->mouseState.yMouse-oy,&info)){

				//p->isHitRaySprite = 1;//设置拾取状态1*

				setv(&(p->flags),EX_FLAGS_RAY_TO_UI);
				
				spr->m_bPressed = 1;//设置鼠标的按下状态
				
				if(getv(&base->flags,FLAGS_DRAG))
				{
					//记录左键点击下的鼠标的相对坐标
					spr->mouseDownX = info.localX;
					spr->mouseDownY = info.localY;
				}
	/*
	#ifdef DEBUG_PRINT_HIT
				//是否射线拾取到了
				printf("**********************************************\n点击了%s界面 局部坐标 %d,%d z=%.2f\n%0x(默认的2dcam)\n%0x(当前的sprite 2dcam)\n**********************************************\n",base->name,info.localX,info.localY,spr->pos_z,
					p->_2dcam,
					base_get_cam((void*)data)
					);
	#endif
	*/

				if(spr->clickCallBack!=0){
					if(_clickInfo->sprite == 0){
						_clickInfo->sprite = spr;
						_clickInfo->local_click_x = info.localX;
						_clickInfo->local_click_y = info.localY;
						
					}else{
						if(_clickInfo->sprite->pos_z < spr->pos_z){//按照z轴值,选择一个更加前面的,作为拾取触发的sprite
							_clickInfo->sprite = spr;
							_clickInfo->local_click_x = info.localX;
							_clickInfo->local_click_y = info.localY;
						}
					}

					//调用点击回调
					//spr->clickCallBack(spr,info.localX,info.localY);
				}
			}
		}
	}
}

/*
	记录鼠标当前移动的屏幕位置坐标
*/
void mouseMove(int x,int y)
{
	struct EX* e=ex_getIns();
	e->mouseState.moveX = x;
	e->mouseState.moveY = y;
	
	//printf("x = %d y = %d\n",x,y);

	if(f_is_mouse_leftDown()  && !getv(&(e->flags),EX_FLAGS_RAY_TO_UI)){
		//鼠标常按了点击了非UI元素
		//printf("1");
		
		base_hit_mouse(x,y,e->_screenWidth,e->_screenHeight,e->renderList,cam_getPerctive(e->_3dcam)/*e->perspectiveMatrix*/,cam_getModel(e->_3dcam)/*/*e->modelViewMatrix*/,f_rayPick);
		//f_3d_pick();
	}


	/*{
		char buffer[G_BUFFER_512_SIZE];	
		sprintf_s(buffer,G_BUFFER_512_SIZE,"%d,%d",e->mouseState.moveX,e->mouseState.moveY);
		ex_showLog(buffer);
	}*/
	
	//只有当鼠标移动的时候才会更新Sprite
	ex_renderlistCall(sprite_mouseMove);	
}

/************************************************************************/
/* 点击回调                                                             */
/************************************************************************/
static void
f_raySome(struct HitResultObject* hit){
	struct EX* ex = ex_getIns();
	struct HitResultObject* last = hit;
	struct HeadInfo* _node = ex_find_headinfo(ex,last->name);

	if(_node){		
		if(_node->luaPickCallBack){
			int n = (int)ex_find_ptr(ex,(const char*)_node->name);
			ex_callIntLuaFun(_node->luaPickCallBack,n);//lua拾取回调	
		}
		//printf("-->%s,%0x\n",_node->name,_node->luaPickCallBack);
		//printf("(%s)射线拾取到的3D坐标:\t%.3f\t%.3f\t%.3f\t%s\tcurType=%d\n",last->name,last->x,last->y,last->z,_node->suffix,_node->curType);
	}else{
		printf("can`t find f_rayPickCallBack\n");
	}
}

static void
f_rayPick(struct HitResultObject* hit){
	//引擎点击回调	
	//evt_dispatch(ex_getInstance(),EVENT_RAY_PICK,(void*)hit);
	f_raySome(hit);

	//################HeadInfo拾取点击回调
	{
		void * ptr = ex_find(hit->name);
		evt_dispatch((void*)base_get(ptr),EVENT_RAY_PICK,(void*)hit);
		
		{
			char bufferXml[G_BUFFER_128_SIZE];
			memset(bufferXml,0,G_BUFFER_128_SIZE);
			//构造xml数据
			//sprintf_s(bufferXml,G_BUFFER_128_SIZE,"<luadata x=\"%.3f\" y=\"%.3f\" z=\"%.3f\"/>",hit->x,hit->y,hit->z);
			sprintf_s(bufferXml,G_BUFFER_128_SIZE,"%.3f,%.3f,%.3f",hit->x,hit->y,hit->z);
			//printf("bufferXML %0x\n",bufferXml);
			ex_lua_evt_dispatch(ptr,EVENT_RAY_PICK,bufferXml);
		}
	}
}

void mousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse){
	struct EX* ex = ex_getIns();
	struct ClickInfo* _clickInfo = (struct ClickInfo*)ex->clickInfo;

	ex->mouseState.button = button;
	ex->mouseState.action = action;
	ex->mouseState.xMouse = xMouse;
	ex->mouseState.yMouse = yMouse;
	ex->mouseState.midDirection = 0;
	
	//printf("%d %d\n",button,action);

	//ex->isHitRaySprite = 0;
	resetv(&(ex->flags),EX_FLAGS_RAY_TO_UI);
	
	//ex->_oldSprite = _clickInfo->sprite;
	_clickInfo->sprite = 0;

	//左键+鼠标按下
	//if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	if(f_is_mouse_leftDown())
	{

		//鼠标点击了屏幕交互操作(开启了射线拾取状态)

		
		//界面射线拾取检测,执行一个可以处理的点击回调
		ex_renderlistCall(render_hitUiNode);
		if(_clickInfo->sprite){
			struct HeadInfo* base = base_get((void*)_clickInfo->sprite);

			if(ex->curFocus!=_clickInfo->sprite){
				void* _old = ex->curFocus;
				//ex_lua_evt_dispatch(ex->curFocus,EVENT_ENGINE_SPRITE_FOCUS_OUT,0);
				ex->curFocus = _clickInfo->sprite;
				//ex_lua_evt_dispatch(ex->curFocus,EVENT_ENGINE_SPRITE_FOCUS_IN,0);
				//printf("focus change\t %0x\tname=[%s]\n",ex->curFocus,base_get(ex->curFocus)->name);
				evt_dispatch(ex_getIns(),EVENT_ENGINE_SPRITE_FOCUS_CHANGE,_old);
			}

			_clickInfo->sprite->clickCallBack(
											_clickInfo->sprite,
											_clickInfo->local_click_x,
											_clickInfo->local_click_y);

			
			//最终拾取到的sprite对象
#ifdef DEBUG_PRINT_HIT
			log_color(0xff0000,"最上层对象************\t[%s]\t,%d,%d\n",base->name,_clickInfo->local_click_x,_clickInfo->local_click_y);
#endif
		}


		setv(&(ex->flags),EX_FLAGS_LEFT_DOWN);//isLeftDown = 1;

		if(_clickInfo->sprite!=ex->stage2d && getv(&(ex->flags),EX_FLAGS_RAY_TO_UI)){
			//点击到了不是舞台,并且是界面层

			//printf("点击了界面就忽略掉3d场景中的模型\n");
		}else{
			//3D世界拾取
			if(getv(&(ex->flags),EX_FLAGS_RAY)){
				base_hit_mouse(xMouse,yMouse,ex->_screenWidth,ex->_screenHeight,ex->renderList,
					
					//ex->perspectiveMatrix
					cam_getPerctive(ex->_3dcam)
					,cam_getModel(ex->_3dcam)/*ex->modelViewMatrix*/,f_rayPick);
				//f_3d_pick();
			}
			
		}
	}

	if(action == GLUT_UP)
	{	
		resetv(&(ex->flags),EX_FLAGS_LEFT_DOWN);//isLeftDown = 0;
	}
	
	if(button == GLUT_MIDDLE_BUTTON)
	{

	}
	//printf("button %d,%d\n",button,action);

	//滚轮事件
	if(button == GLUT_WHEEL_UP)
	{
		ex->mouseState.midDirection = -1;
	}
	else if(button == GLUT_WHEEL_DOWN)
	{
		ex->mouseState.midDirection = 1;
	}

	//printf("中键滚动状态 = %d\n",ex->mouseState.midDirection);

	//ui鼠标事件
	//uimouse(button,action,xMouse,yMouse);
	
	ex_renderlistCall(sprite_action);
	glutPostRedisplay();

}
//按键控制
void onKeyboardCallBack(unsigned char key, int x, int y){
	struct E_KeyBoard ekey;
	ekey.key = key;
	ekey.x = x;
	ekey.y = y;
	//printf("key=\t%d %d %d\n",key,x,y);
	
	evt_dispatch(ex_getIns(),EVENT_ENGINE_KEYBOARD,(void*)&ekey);

	//{
		/*char _str[G_BUFFER_16_SIZE];
		memset(_str,0,G_BUFFER_16_SIZE);
		sprintf_s(_str,G_BUFFER_16_SIZE,"%d",key);
		printf("键盘键值key=(%s)\n",_str);
		ex_lua_evt_dispatch(0,EVENT_ENGINE_KEYBOARD,_str);*/
	//}
	ex_lua_evt_dispatch_f(0,EVENT_ENGINE_KEYBOARD,key);
}

//static void 
//update3DNode(int data){
//	base_updateMat4x4(base_get((void*)data));
//}

void 
ex_cam_set_pos(float x,float y,float z){
	struct EX* ex = ex_getIns();	

	void* _c = ex->_3dcam;
	
	cam_setX(_c,x);
	cam_setY(_c,y);
	cam_setZ(_c,z);

	//更新渲染列表中的矩阵
	//f_renderlistCall(update3DNode);
	cam_refreshModel(ex_getIns()->_3dcam);
}

void setBgColor(float r,float g,float b){
	struct EX* ex = ex_getIns();
	ex->bg_r = r;
	ex->bg_g = g;
	ex->bg_b = b;
#ifdef DEBUG
	printf("设置窗口背景填充色:%f,%f,%f\n",r,g,b);
#endif
}

//int getState(int t){
//	struct EX* e = f_getEx();
//	return (e->_st & t)>>tl_bintree(t);
//}

//void set(int t){
//	struct EX* e = f_getEx();
//	e->_st = e->_st | t;
//}

//void reset(int t ){
//	struct EX* e = f_getEx();
//	e->_st = (~t)&e->_st; 
//}

/*
	切换标识
*/
/*
void ex_switch_baseFlags(struct HeadInfo* _node,int flag){
	//选择切换模式//int flag =  FLAGS_GLSL_OUTLINE;
	int *v = &_node->flags;
	if(getv(v,flag))
	{
		resetv(v,flag);
	}
	else
	{
		setv(v,flag);
	}
}
*/
/*
 *初始化日志tf句柄
 */
static void
getLog(){
	struct EX* e = ex_getIns();
	/*if(!e->logTf){
		e->logTf = tf_create(G_BUFFER_1024_SIZE,0,0,1.0,0.0,0);
	}*/
}

void 
ex_showLog(const char* buffer){
	struct EX* e = ex_getIns();
	getLog();
	//tf_setText(e->logTf,buffer);
}

//void
//ex_showLogFloat(float v)
//{
//	char buffer[G_BUFFER_1024_SIZE];
//	struct EX* e = ex_getInstance();
//	getLog();
//	sprintf_s(buffer,G_BUFFER_1024_SIZE,"%f",v);
//	tf_setText(e->logTf,buffer);
//}

void ex_callParmLuaFun(const char* luaFunName,const char* parm)
{
	if((size_t)strlen(luaFunName))
	{
		lua_State* L =ex_getIns()->mylua;
		lua_getglobal(L,luaFunName);
		lua_pushstring(L,parm);
		//lua_pushinteger(L,parm);

		//lua_pushinteger(L,sprite->ptr_luaCallBack);
		//lua_pushlstring();
		if(lua_pcall(L,1,0,0)!=0){
			printf("error %s\n", lua_tostring(L,-1));
		}
	}
}

#define __BUFFER_SIZE_ 1024	//缓冲区大小

void ex_alert(const char* format,...){	
	char str_tmp[__BUFFER_SIZE_];
	int i;
	char s[__BUFFER_SIZE_];
	va_list vArgList;                          
	va_start (vArgList, format);	
	i = _vsnprintf_s(str_tmp, __BUFFER_SIZE_,__BUFFER_SIZE_, format, vArgList);
	sprintf_s(s,__BUFFER_SIZE_,"%s",str_tmp);
	ex_callParmLuaFun("alert",s);
	va_end(vArgList);
}

	

void ex_callIntLuaFun(const char* luaFunName,int value)
{
	if((size_t)strlen(luaFunName))
	{
		lua_State* L =ex_getIns()->mylua;
		lua_getglobal(L,luaFunName);
		//lua_pushstring(L,parm);
		lua_pushinteger(L,value);

		//lua_pushinteger(L,sprite->ptr_luaCallBack);
		//lua_pushlstring();
		if(lua_pcall(L,1,0,0)!=0){
			printf("error %s\n", lua_tostring(L,-1));
		}
	}
}
void 
ex_lua_global_evt_dispatch(int evtid){
	ex_lua_evt_dispatch(0,evtid,0);
}

void 
ex_lua_evt_dispatch_f(void* obj,int evtid,float data){
	lua_State* L =ex_getIns()->mylua;
	if(L){
		lua_getglobal(L,"evt_dispatch");//调用lua的eve_dispath方法
		lua_pushinteger(L,(int)obj);//事件派发的对象
		lua_pushinteger(L,evtid);//事件id号
		lua_pushnumber(L,data);//事件数据
		lua_pcall(L,3,0,0);
	}
}

void 
ex_lua_evt_dispatch(void* obj,int evtid,const char* data){
	
	lua_State* L =ex_getIns()->mylua;
	if(L){
		lua_getglobal(L,"evt_dispatch");//调用lua的eve_dispath方法
		lua_pushinteger(L,(int)obj);//事件派发的对象
		lua_pushinteger(L,evtid);//事件id号
		lua_pushstring(L,data);//事件数据
		lua_pcall(L,3,0,0);
	}
	else{
#ifdef DEBUG
	//	printf("evtId : %d,lua script is not init!\n",evtid);
#endif
	}
}
//
//void 
//setLookTarget(void* ptr,float x,float y,float z)
//{
//	struct HeadInfo* b = base_get(ptr);
//	b->lookat = 1;
//	b->target.x = x;
//	b->target.y = y;
//	b->target.z = z;
//	vec3Normalize(&b->target);
//	//printf("%f.%f,%f\n",x,y,z);
//
//	if(vec3IsZero(&b->target)){
//		printf("不能是0向量\n");
//		assert(0);
//	}
//	else
//		base_updateMat4x4(b);
//}
void
ex_set_material(void* ptr,void* material){
	struct HeadInfo*  base = base_get(ptr);
	base->tmat = (struct GMaterial*)material;
}
int 
ex_rename(void* p,const char* name){
	if(!ex_find(name)){
		memset(base_get(p)->name,0,G_BUFFER_32_SIZE);
		memcpy(base_get(p)->name,name,strlen(name));
		return 1;
	}
	log_code(ERROR_RENAME);
	assert(0);
	return 0;
}
void ex_ptr_remove(void* ptr){
	struct HeadInfo* b = base_get((void*)ptr);
	if(!b)
	{
		log_code(ERROR_PTR_REMOVE_FAIL);
		printf("ex_ptr_remove移除失败,找不到对象0x%0x\n",ptr);
		assert(0);
		return;
	}
	log_color(0,"ex_ptr_remove curType = %d(%s) name = %s\n",b->curType,f_getFileName(b->curType),b->name);
	switch(b->curType){
		case TYPE_SPRITE_FLIE:
			sprite_dipose((struct Sprite*)ptr);
			break;
		case TYPE_TEXT_FILE:
			tf_dispose((struct TextField*)ptr);
			break;
		case TYPE_OBJ_VBO_FILE:
			//md2,obj的vbo模式
			//objVBO_dispose((struct Obj_vbo_model*)ptr);
			node_dispose((struct Node*)ptr);
			break;
		case TYPE_MD2_FILE:
			md2_dispose((struct MD2_Object*)ptr);
			break;
		case TYPE_OBJ_FILE:
			f_ent_dispose((struct Ent3D*)ptr);
			break;
		case TYPE_MD5_FILE:
			md5Model_dispose((struct MD5*)ptr);
			break;
	}
}

//void 
//ex_setv(void* ptr,int flag){
//	struct HeadInfo* base=base_get((void*)ptr);
//	setv(&base->flags,flag);
//}

//void
//ex_resetv(void* ptr,int flag){
//	struct HeadInfo* base=base_get((void*)ptr);
//	resetv(&base->flags,flag);
//}

//int 
//ex_getv(void* ptr,int flag){
//	struct HeadInfo* base=base_get((void*)ptr);
//	return getv(&base->flags,flag);
//}


int ex_get_gap(int type)
{
	if(type == OBJ_UV_VERTEX)
	{
		return UV_GAP + VERTEX_GAP;
	}
	return UV_GAP + VERTEX_GAP + NORMAL_GAP;
}
void* ex_get_ui_atals(){
	if(!ex_getIns()->atals){
		struct Atals* ptr = atals_load("//resource//texture//","1");
		ex_getIns()->atals = (void*)ptr;
	}
	return ex_getIns()->atals;
}

void ex_switch3dCam(void* cam){
	ex_getIns()->_3dCurCam=cam;
}

void ex_switch2dCam(void* cam){
	ex_getIns()->_2dCurCam = cam;
}

//########################################

//void
//fbo_render(void* ptr){
//	struct FBOTex* fbo = (struct FBOTex*)ptr;
//
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->fboName);
//	glDrawBuffers(1, fboBuffs);
//
//	glViewport(0, 0, fbo->texw, fbo->texh);
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除当前的缓冲区
//
//	f_renderlistCall(render_3dNode);//渲染3d节点
//
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//}
