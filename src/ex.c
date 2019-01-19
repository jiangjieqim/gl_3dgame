#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#include "tools.h"
#include "tlgl.h"
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
};
/*
	鼠标左键是否常按着
*/
static int isLeftDown = 0;

static struct EX g_ex;
struct EX* ex_getInstance(){
	return &g_ex;
}//引擎实例引用

/*
"//resource//texture//1.tga"
*/
void*
ex_get_defaultMaterial(){
	if(!ex_getInstance()->defaultMaterial){
		ex_getInstance()->defaultMaterial =	tmat_createMaterial("spritevbo",0,0);
	}

	return ex_getInstance()->defaultMaterial;
}

/*
*	md2渲染回调
*/
static void 
md2_render(struct MD2_Object* _md2){

	struct FrameAnim* frameAnim = _md2->base->frameAnim;
	struct HeadInfo* base =(struct HeadInfo*)_md2->base;
	struct MD2_Frame* frame;
	struct VertexData* p;

	//计算关键帧
	base_calculateFrame(frameAnim);

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
void render_3dNode(int data);
void render_uiNode(int data);
int ex_mouseIsLeftDown()
{
	return isLeftDown;
}
void* ex_find_node(const char* name){
	return ex_findNodeByName(ex_getInstance(),name);
}
/*
根据name寻找对象(name为一个字符串)
*/
void* ex_findNodeByName(struct EX* ptr,const char* name){
	struct LStackNode* s = (struct LStackNode* )ptr->renderList;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		struct HeadInfo* base = NULL;
		void* n;

		p=(void*)LStack_next(p);
		data = LStack_data(p);

		n = (void*)data;
		//base_get(n,&base);
		base = base_get(n);
		if(base!= NULL && !strcmp((const char*)base->name,name)){
			return n;
		}
	}
	return NULL;
}

/*
添加一个渲染节点
*/
void ex_addNode(struct EX* p, void* _node){
	//EngineX* p = this;
	if(_node!=NULL){
		//struct HeadInfo base;
		struct HeadInfo* b;
		//base_get(_node,&base);
		b = base_get(_node);
		if( ex_findNodeByName(p,b->name) != NULL){
			printf("error! 重名的对象_engine :%s\n",b->name);
			assert(0);
		}else{
			LStack_push(p->renderList,(int)_node);
		}
	}
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
	struct FrameAnim* frameAnim = NULL;
	long usetime=get_longTime();

	int numFrames = md2parse_totalFrames(_md2->parseHandle);// _md2->parseHandle->numFrames;

	//_md2->parseHandle = (struct MD2_ParseObj*)tl_malloc(sizeof(struct MD2_ParseObj));

	_md2->parseHandle = md2parse_load(str,len);
	
	if(!numFrames){
		printf("关键帧数为0!\n");
		assert(0);
	}

	printf("解析文件[%s]花费%ld毫秒,关键帧数=%d\n",name,get_longTime()-usetime,numFrames);

	_md2->base = base_create(TYPE_MD2_FILE,name,0.0,0.0,0.0);
	base = _md2->base;

	//关键帧管理器
	base->frameAnim = (struct FrameAnim*)tl_malloc(sizeof(struct FrameAnim));
	frameAnim = base->frameAnim;
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

/*
打印引擎当前信息
*/
void ex_info(){
	struct EX* ex = ex_getInstance();
	//EngineX* p = this;
	char buffer[G_BUFFER_1024_SIZE];
	int j=0;
	int buffer_size = G_BUFFER_1024_SIZE;
	memset(buffer,0,G_BUFFER_1024_SIZE);
	j+=sprintf_s(buffer+j,buffer_size, "**********************************************\n");
	j+=sprintf_s(buffer+j,buffer_size, "FPS	: %d\n",ex->fps);
	j+=sprintf_s(buffer+j,buffer_size, "屏幕尺寸:%.1f,%.1f\n",ex->_screenWidth,ex->_screenHeight);
	j+=sprintf_s(buffer+j,buffer_size, "程序已执行:%.3f 秒\n",gettime_cur());
	j+=sprintf_s(buffer+j,buffer_size, "内存池已使用 %f mb\n",(float)tl_memByteSize()/1024/1024);
	j+=sprintf_s(buffer+j,buffer_size, "渲染节点个数:%d 摄影机坐标:%f %f %f\n%s\n",LStack_length(ex->renderList),ex->camx,ex->camy,ex->camz,"F4:静态多边形显示线框 \nF12:包围盒显示");
	j+=sprintf_s(buffer+j,buffer_size, "vbo使用:%d bytes\n",tlgl_getVboSize());
	j+=sprintf_s(buffer+j,buffer_size, "当前(射线检测)状态:%d\n",getv(&(ex->flags),EX_FLAGS_RAY));
	//printf("%s\n",buffer);
	log_color(0x00ff00,"%s\n",buffer);
	//printf("当前(射线检测)状态:%d\n",getv(&(ex->flags),EX_FLAGS_RAY));

	memory_get_info();
}
static char buffer[G_BUFFER_256_SIZE];
/*
绘制文本
*/
void drawText(struct EX* p){
	//EngineX* p = this;
	
	int vbo = tlgl_getVboSize();
	int j;
	int size=-1;
	GLint total_mem_kb,cur_avail_mem_kb;
	memset(buffer,0,G_BUFFER_256_SIZE);
	
	tlgl_getGPU_memery(&total_mem_kb,&cur_avail_mem_kb);

	if(tl_memGetStat())
	{
		size = (int)tl_memByteSize();
	}

	j = sprintf_s(buffer, G_BUFFER_256_SIZE,"fps:%d, tl_memByteSize=%d bytes ", p->fps,size);
	sprintf_s(buffer + j,G_BUFFER_256_SIZE,"(vbo:%d bytes %.3f kb)cam: %f,%f,%f VertexCount=%d TriangleCount=%d  cur_avail_mem_kb=%d total_mem_kb=%d,is running %.3f second",vbo,(float)vbo/1024.0,p->camx,p->camy,p->camz,p->allVertexTotal,p->allVertexTotal/3,cur_avail_mem_kb,total_mem_kb,gettime_cur());
	
	ex_showLog(buffer);
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
	struct EX* p = ex_getInstance();

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
	tl_set_vec(&base->x,x,y,z);

	base->scale = scale;

	frame =	md2parse_getFrame(md2->parseHandle,0);	// &(md2->parseHandle->pframe[0]);//取第1帧为射线拾取的索引
	base_createRayVertex(&base->rayVertexData,frame->vertices,frame->vertCount);

	printf("创建md2 [%s],每%ld毫秒切换一帧, 坐标x:%.3f,y:%.3f,z:%.3f 当前动作:%s\n",name,base->frameAnim->fpsInterval,x,y,z,base->frameAnim->curAnim);

	//默认设置为跑步状态
	//base_curAnim(base->frameAnim,defaultAnim);


	updateMat4x4(base);

	ex_addNode(p,md2);

	return md2;
}
/*
*	销毁Ent3D对象
*/
static void 
ent_dispose(struct Ent3D* p){
	
	tl_free(p->vertex);

	base_dispose(p->base);
	p->base = NULL;

	LStack_delNode(ex_getInstance()->renderList,(int)p);
	tl_free((void*)p);
}
/*
*	销毁md2对象
*/
static void 
md2_dispose(struct MD2_Object* _md2)
{
	md2parse_dispose(_md2->parseHandle);
	_md2->parseHandle = NULL;

	base_dispose(_md2->base);
	_md2->base = NULL;

	LStack_delNode(ex_getInstance()->renderList,(int)_md2);

	tl_free((void*)_md2);
}

void md5Model_dispose(struct MD5* _md5)
{
	md5_dispose(_md5);
	
	base_dispose(_md5->base);
	_md5->base = NULL;

	LStack_delNode(ex_getInstance()->renderList,(int)_md5);

	tl_free((void*)_md5);
}

static void ent_render(struct Ent3D* ent){
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
void render_3dNode(int data)
{
	struct EX*e = ex_getInstance();	
	struct HeadInfo* base = base_get((void*)data);
	int objType = base->curType;

	if(objType== TYPE_OBJ_FILE)
	{
		ent_render((struct Ent3D*)data);
	}
	else if(objType == TYPE_MD5_FILE)
	{
		struct EX*ex = ex_getInstance();
		float f = base->fpsInterval <= 0 ? 0.0f : (1000.0f/base->fpsInterval);//f代表 f毫秒播放一个关键帧,f = 0的时候停止在当前关键帧
		md5_render((struct MD5*)data,f);//ex->fps	

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
		e->allVertexTotal+=getAllVertex(data);
	}
}
/*
	回调
*/
void ex_renderlistCall(void _callBack(int))
{
	struct EX* ex = ex_getInstance();
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
	tl_splitByStr((const char*)splitStr,',',f_animSpliteCheck,inputParms);
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
	tl_splitByStr((const char*)animConf,'@',f_animObjSplit,(int*)&anim);
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

static void
changeCam()
{

	struct EX* p = ex_getInstance();
	Matrix44f rx,ry,rz,temp1,temp2;
	Matrix44f pos;
	
	mat4x4_zero(temp1);
	mat4x4_zero(temp2);

	mat4x4_identity(pos);
	mat4x4_translate(pos,p->camx,p->camy,p->camz);

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

static void 
updatePerspectiveMatrix( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar){
	struct EX* p = ex_getInstance();
	//只是处理鼠标拾取操作用来获取坐标使用gluUnProject,3d物体 不使用该方式(使用用户自定义的透视矩阵,视图矩阵,模型矩阵)
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (fov,aspectRatio,zNear,zFar);
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glRotatef(-180/PI*p->camRotateX,1,0,0);
	glRotatef(-180/PI*p->camRotateY,0,1,0);
	glRotatef(-180/PI*p->camRotateZ,0,0,1);
	glTranslatef(p->camx,p->camy,p->camz);

	mat4x4_zero(p->modelViewMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX,p->modelViewMatrix);
	mat4x4_transpose(p->modelViewMatrix);

	mat4x4_zero(p->perspectiveMatrix);
	glGetFloatv(GL_PROJECTION_MATRIX,p->perspectiveMatrix);
	mat4x4_transpose(p->perspectiveMatrix);
	//{
	//	GLdouble rFov = fov * PI / 180.0;
	//	mat4x4_zero(p->perspectiveMatrix);
	//	perspectiveFrustum(p->perspectiveMatrix, -zNear * tan( rFov / 2.0 ) * aspectRatio,zNear * tan( rFov / 2.0 ) * aspectRatio,-zNear * tan( rFov / 2.0 ),zNear * tan( rFov / 2.0 ),zNear, zFar);
	//}	
}

/*
	计算透视和矩阵索引
*/
void ex_updatePerspctiveModelView()
{
	struct EX* p = ex_getInstance();
	updatePerspectiveMatrix(45.0, (GLdouble)p->_screenWidth/(GLdouble)p->_screenHeight, 0.1, p->zFar);
}

//static struct Obj_vbo_model* tmodel;
//static struct GMaterial* tmaterial;
//static Matrix44f myMatirx;
///*
// *构造VBO
// */
//static struct Obj_vbo_model* 
//load_vbo()
//{
//	struct Obj_vbo_model* vbo;
//	char* _objStr;
//
//	int verts,_bufferSize;
//	char buffer[G_BUFFER_64_SIZE];
//
//	_objStr=tl_loadfile("\\resource\\obj\\quad.obj",0);//quad,teapot
//	obj_parse((char*)_objStr,&_bufferSize,&verts,OBJ_UV_VERTEX_NORMAL);
//	tl_address2str((int)_objStr,buffer,G_BUFFER_64_SIZE);
//	
//	vbo = objVBO_create(buffer,OBJ_UV_VERTEX);//OBJ_UV_VERTEX,OBJ_UV_VERTEX_NORMAL
//	objVBO_pushNode(vbo ,(GLfloat*)verts,_bufferSize);
//	tl_free((void*)verts);
//	tl_free(_objStr);
//
//	return vbo;
//}
void ex_drawline(){

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
	if(ex_getInstance()->drawLine_callBack)
		ex_getInstance()->drawLine_callBack();
}
void _new()
{
	struct EX* p = ex_getInstance();

	if(p->_screenWidth <= 0 || p->_screenHeight<=0)
	{
		//屏幕尺寸0的时候不进行渲染
		return;
	}

	//if(tmodel==NULL)
	//{
	//	Matrix44f m_scale,xyz;
	//	
	//	float s = 100;

	//	tmodel = load_vbo();
	//	//tmat_createMaterial("sprite",tgaURL,0);
	//	//ex_getInstance()->myAtals->material
	//	mat4x4_zero(myMatirx);
	//	mat4x4_identity(m_scale);
	//	mat4x4_scale(m_scale,s,s,s);

	//	mat4x4_identity(xyz);
	//	mat4x4_translate(xyz,0,0,0);

	//	multi2(myMatirx,xyz,m_scale);//位移矩阵

	//	tmaterial = tmat_createMaterial("spritevbo","//resource//texture//1.tga",0);//vboDiffuse
	//}

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//填充背景为白色
	glClearColor(p->bg_r,p->bg_g,p->bg_b,1);

	//深度测试
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	
	//ex_updatePerspctiveModelView();
	p->allVertexTotal = 0;
	ex_renderlistCall(render_3dNode);

	//drawLine(2000);
	ex_drawline();
	
	//ex_renderlistCall(sprite_drawRender);//test
	if(TRUE)//FALSE
	{

		//界面渲染,切换为正交视图
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, p->_screenWidth, 0, p->_screenHeight, 0, -p->allzBuffer);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		mat4x4_zero(p->ui_modelViewMatrix);
		glGetFloatv(GL_MODELVIEW_MATRIX,p->ui_modelViewMatrix);
		mat4x4_transpose(p->ui_modelViewMatrix);

		mat4x4_zero(p->ui_perspectiveMatrix);
		glGetFloatv(GL_PROJECTION_MATRIX,p->ui_perspectiveMatrix);
		mat4x4_transpose(p->ui_perspectiveMatrix);


		//if(tmaterial)
		//{
		//	struct GMaterial* material = tmaterial;//(struct GMaterial*)(p->myAtals)->material;
		//	struct ObjVBO* vbo=(struct ObjVBO*)LStatk_getAddressByIndex(tmodel->ptrList,0);
		//	objVBO_renderNode(vbo,material,myMatirx,GL_FILL);
		//}
		
		ex_renderlistCall(sprite_drawRender);
		ex_renderlistCall(tf_render);

		//渲染文本(非常耗费性能,待优化)
		//drawText(p);
	}

	glutSwapBuffers ();
	glutPostRedisplay ();
	//计算fps
	tl_calculate_fps();
	p->fps = tl_get_fps();

	//渐变管理器回调
	ramp_callBack();

	//外部循环回调
	if(p->loopCallBack!=NULL)
	{
		p->loopCallBack();
	}
	//printf("**** %d\n",p->fps);
}
void render(void)
{
	_new();
}

struct HeadInfo* ex_find(struct EX* p,const char* name){
	return base_get(ex_findNodeByName(p,name));
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

int ex_zBuffer()
{
	struct EX* p = ex_getInstance();
	return p->zBuffer++;
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

void ex_init(struct EX* p,GLdouble zfar)
{	
	p->allzBuffer = -90000;//初始化一个Z深度
	p->zBuffer = p->allzBuffer+1;

	p->zFar = zfar;
	p->renderList = LStack_create();
	/*p->hit=hit_create(p->renderList,p->viewport,p->hitModelView,p->hitPerspectProjection);*/
	
	//==========================================
	//loadSpriteVert(p);
	
}

void ex_dispose(struct EX* p){
	LStack_delete((struct LStackNode*)p->renderList);

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
	struct EX* engine = ex_getInstance();
	struct MD5* md5 = NULL;
	struct HeadInfo* _base = NULL;
	
	//================================================
	md5 = (struct MD5*)tl_malloc(sizeof(struct MD5));
	memset(md5,0,sizeof(struct MD5));
	
	_base=base_create(TYPE_MD5_FILE,_name,x,y,z);

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
	
	updateMat4x4(_base);

	//添加到渲染列表
	ex_addNode(engine,md5);

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
	struct EX* engine = ex_getInstance();

	//EngineX* engine = this;
	struct HeadInfo* base;

	struct Ent3D* ent = (struct Ent3D*)tl_malloc(sizeof(struct Ent3D));

	ent->base = base_create(TYPE_OBJ_FILE,name,x,y,z);
	base  = (struct HeadInfo*)ent->base;
	
	//设置缩放
	base->scale = scale;
	
	//绑定x,y,z引用
	tl_set_vec(&base->x,x,y,z);
	
	//设置名字
	//setName(base->name,name,G_BUFFER_32_SIZE);

	f_end3d_loadMesh(ent,mesh_s);//加载解析数据

	base_createRayVertex(&base->rayVertexData,ent->vertex,ent->vertexCount);

	updateMat4x4(base);

	//添加到渲染列表
	ex_addNode(engine,ent);

	return ent;
}
//固定管线
int
load_model(char* name,const char* url,float x,float y,float z,float scale)
{
	char suffix[G_BUFFER_16_SIZE];
	tl_getSuffixByPath((char*)url,suffix,G_BUFFER_16_SIZE);
	if(!strcmp(suffix,"obj"))
	{
		return (int)load_obj(name,url,x,y,z,scale);
	}
	else if(!strcmp(suffix,"md2"))
	{
		return (int)load_md2(name,url,x,y,z,scale);
	}
	else if(!strcmp(suffix,"md5mesh"))
	{
		return (int)load_md5(name,url,x,y,z,scale);
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
	base_set_suffix(node->base,url);
	
	//log_put("加载文件",url);

	node->ptrVBO = objVBO_create(name,dataType);
	_objStr=tl_loadfile((char*)url,&fileSize);
	//////////////////////////////////////////////////////////////////////////
	//_parse=(struct MD2_ParseObj*)tl_malloc(sizeof(struct MD2_ParseObj));
	_parse=md2parse_load(_objStr,fileSize);
	
	pushMd2Frame(node->ptrVBO,_parse,tl_getGap(dataType));

	//构造动作管理器
	//ptr->anim = anim_create(ptr->ptrList);
	node_bindAnim(node);
	
	//设置动作播放帧频
	anim_set_fps(node->anim,7);

	//释放资源.
	md2parse_dispose(_parse);
	tl_free(_objStr);
	
	ex_addNode(ex_getInstance(),node);

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

	node = node_create(name);
	_objStr=tl_loadfile(url,0);
	
	//设置后缀
	base_set_suffix(node->base,url);

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
	ex_addNode(ex_getInstance(),node);
	return (int)node;
}

int load_VBO_model(char* name,const char* url)
{
	char suffix[G_BUFFER_16_SIZE];
	
	log_color(0x00ff00,"load_VBO_model创建模型对象:(%s)\n",name);

	tl_getSuffixByPath((char*)url,suffix,G_BUFFER_16_SIZE);
	if(!strcmp(suffix,"obj"))
	{
		return vbo_loadObj3d(name,url);
	}
	else if(!strcmp(suffix,"md2"))
	{
		//return (int)load_md2(name,url,x,y,z,scale);
		return vbo_md2Load(name,url);
	}
	else if(!strcmp(suffix,"md5mesh"))
	{
		//return (int)load_md5(name,url,x,y,z,scale);
	}
	assert(0);
	printf("模型加载方式未实现...\n");
	return 0;
}


void 
setv_ptr(void* ptr,int flags)
{
	struct HeadInfo* b = base_get(ptr);
	//setv(&((struct HeadInfo*)base_get2(ptr)->flags),flags);
	setv(&b->flags,flags);
}

void 
setMaterial(void* ptr,void* material)
{
	(struct HeadInfo*)base_get(ptr)->tmat = material;
}

/*
	加载md2动作配置
*/
int 
load_animConfig(void* ptr,char* animConf,long fps)
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
		memcpy(base->frameAnim->animConfig,animConf,strlen(animConf));


		base->frameAnim->fpsInterval = 1000 / fps;
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
setanim(void* ptr,const char* animKey)
{
	struct HeadInfo* base = base_get(ptr);
	if(base->curType == TYPE_MD2_FILE)
	{
		base_curAnim(base->frameAnim,animKey);
	}
	else if(base->curType == TYPE_MD5_FILE)
	{
		md5_setAnim((void*)ptr,animKey);
	}
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
/*
	是否有鼠标事件
*/
static int hasMouseClick(struct Sprite* n)
{
	return (int)n->clickCallBack;
}

static void 
render_hitUiNode(int data)
{
	if(sprite_isEnable(data))
	{
		struct EX* p = ex_getInstance();

		struct HeadInfo* base = base_get((void*)data);
		//renderSprite(data);
		struct Sprite* spr = (struct Sprite*)data;
		struct HitUiInfo info;
		if(hasMouseClick(spr) && hitUiTest(spr,p->mouseState.xMouse,p->mouseState.yMouse,&info)){

			p->isHitRaySprite = 1;//设置拾取状态

			spr->m_bPressed = 1;//设置鼠标的按下状态
			
			if(getv(&base->flags,FLAGS_DRAG))
			{
				//记录左键点击下的鼠标的相对坐标
				spr->mouseDownX = info.localX;
				spr->mouseDownY = info.localY;
			}

			//是否射线拾取到了
			//printf("点击了%s界面 局部坐标 %d,%d\n",base->name,info.localX,info.localY);
			if(spr->clickCallBack!=NULL){
				spr->clickCallBack(spr,info.localX,info.localY);
			}
		}
	}
}

/*
	记录鼠标当前移动的屏幕位置坐标
*/
void mouseMove(int x,int y)
{
	struct EX* e=ex_getInstance();
	e->mouseState.moveX = x;
	e->mouseState.moveY = y;
	


	/*{
		char buffer[G_BUFFER_512_SIZE];	
		sprintf_s(buffer,G_BUFFER_512_SIZE,"%d,%d",e->mouseState.moveX,e->mouseState.moveY);
		ex_showLog(buffer);
	}*/
	
	//只有当鼠标移动的时候才会更新Sprite
	ex_renderlistCall(sprite_mouseMove);	
}

void mousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse){
	struct EX* ex = ex_getInstance();

	ex->mouseState.button = button;
	ex->mouseState.action = action;
	ex->mouseState.xMouse = xMouse;
	ex->mouseState.yMouse = yMouse;
	
	//printf("%d\n",button);

	ex->isHitRaySprite = 0;

	//鼠标点击了屏幕交互操作(开启了射线拾取状态,左键+鼠标按下)
	if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{
		//界面射线拾取检测
		ex_renderlistCall(render_hitUiNode);
		isLeftDown = 1;
		if(ex->isHitRaySprite){
			//点击了界面就忽略掉3d场景中的模型
		}else{
			//3D世界拾取
			if(getv(&(ex->flags),EX_FLAGS_RAY)){
				hit_mouse(xMouse,yMouse,ex->_screenWidth,ex->_screenHeight,ex->renderList,ex->mRayPickCallBack);
			}
		}
	}

	if(action == GLUT_UP)
	{
		isLeftDown = 0;
	}
	
	if(button == GLUT_MIDDLE_BUTTON)
	{

	}
	//printf("button %d,%d\n",button,action);

	//滚轮事件
	if(button == GLUT_WHEEL_UP)
	{

	}
	else if(button == GLUT_WHEEL_DOWN)
	{
		
	}

	//ui鼠标事件
	//uimouse(button,action,xMouse,yMouse);
	
	ex_renderlistCall(sprite_action);
	glutPostRedisplay();

}
//按键控制
void onKeyboardCallBack(unsigned char key, int x, int y){
	//struct EX* ex = ex_getInstance();
	////摄影机的移动速度
	//float _camSpeed = 10.0f;
	////struct EngineXOBJ* ep = engineObj;

	//printf("======>key=%d\n",key);
	//switch(key){
	//	case KEY_ESC:
	//		exit(0);
	//		break;
	//	case KEY_1:
	//		ex->camy+=_camSpeed;
	//		break;
	//	case KEY_2:
	//		ex->camy-=_camSpeed;
	//		break;
	//	case KEY_A:
	//		
	//		break;
	//	case KEY_D:

	//		break;
	//}
}


void update3DNode(int data)
{
	updateMat4x4(base_get((void*)data));
}

void setCamPos(float x,float y,float z){
	struct EX* ex = ex_getInstance();	
	ex->camx = x;	ex->camy = y;	ex->camz = z;
	//更新渲染列表中的矩阵
	ex_renderlistCall(update3DNode);
	//printf("setCamPos	%f,%f,%f\n",x,y,z);
	ex_updatePerspctiveModelView();
}

struct EX* ex_create()
{
	return ex_getInstance();
}

void setBgColor(float r,float g,float b){
	struct EX* ex = ex_getInstance();
	ex->bg_r = r;
	ex->bg_g = g;
	ex->bg_b = b;
	printf("设置窗口背景填充色:%f,%f,%f\n",r,g,b);
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

void ex_bind(int argc,char** argv){
	
}

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
/*
 *初始化日志tf句柄
 */
static void
getLog()
{
	struct EX* e = ex_getInstance();
	if(!e->logTf)
	{
		e->logTf = tf_create(G_BUFFER_1024_SIZE,140,0,0,1.0,0);
	}
}

void 
ex_showLog(const char* buffer)
{
	struct EX* e = ex_getInstance();
	getLog();
	tf_setText(e->logTf,buffer);
}

void
ex_showLogFloat(float v)
{
	char buffer[G_BUFFER_1024_SIZE];
	struct EX* e = ex_getInstance();
	getLog();
	sprintf_s(buffer,G_BUFFER_1024_SIZE,"%f",v);
	tf_setText(e->logTf,buffer);
}

void ex_callParmLuaFun(const char* luaFunName,const char* parm)
{
	if((size_t)strlen(luaFunName))
	{
		lua_State* L =ex_getInstance()->mylua;
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

void ex_callIntLuaFun(const char* luaFunName,int value)
{
	if((size_t)strlen(luaFunName))
	{
		lua_State* L =ex_getInstance()->mylua;
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
setLookTarget(void* ptr,float x,float y,float z)
{
	struct HeadInfo* b = base_get(ptr);
	b->lookat = 1;
	b->target.x = x;
	b->target.y = y;
	b->target.z = z;
	vec3Normalize(&b->target);
	//printf("%f.%f,%f\n",x,y,z);

	if(vec3IsZero(&b->target)){
		printf("不能是0向量\n");
		assert(0);
	}
	else
		updateMat4x4(b);
}

void ex_ptrRemove(void* ptr){
	struct HeadInfo* b = base_get((void*)ptr);
	if(!b)
	{
		log_code(ERROR_PTR_REMOVE_FAIL);
		printf("ex_ptr_remove移除失败,找不到对象0x%0x\n",ptr);
		assert(0);
		return;
	}
	//int curType = b->curType;
/*
	if(curType==TYPE_SPRITE_FLIE)
	{
		sprite_dipose((struct Sprite*)ptr);
	}
	else if(curType==TYPE_TEXT_FILE)
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
*/
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
			ent_dispose((struct Ent3D*)ptr);
			break;
		case TYPE_MD5_FILE:
			md5Model_dispose((struct MD5*)ptr);
			break;
	}
}