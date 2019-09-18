#include <gl/glew.h>
#include <gl/glut.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "tl_malloc.h"
#include "jgl.h"

#include "fbotex.h"
#include "tmat.h"
#include "base.h"
#include "sprite.h"
#include "camera.h"
#include "ex.h"

//#define _2D_Z_POS -90000

struct FBOTexNode
{
	void* _3dcam;			//3d透视camera
	
	void* _2dcam;			//2d透视camera

	GLuint              fboName;			//FBO命名对象	
	GLuint				tex;		//贴图
	GLuint              depthBufferName;	//深度缓冲区
	int					texw,texh;			//贴图的宽高
	
	//GLuint				tex;

	// 渲染节点的回调
	//void (*callBack)();

	//渲染的节点列表
	void* nodelist;
};
static void
f_renderList(int data,int parms){
	//printf("%d\n",data);
	ex_render3dNode(data);
	//sprite_drawRender(data);
	//sprite_updatePos(data);
}
void
fbo_render(void* ptr){
	struct FBOTexNode* fbo = (struct FBOTexNode*)ptr;
	const GLenum fboBuffs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	ex_switch3dCam(fbo->_3dcam);

	{
		//This is Test code for debug to equal 2 mat4x4
		/*struct EX* p = ex_getIns();
		cam_setOrtho(p->_2dcam,256,256,-p->allzBuffer);*/
	
		//void* a= cam_getPerctive(fbo->_2dcam);
		//void* b= cam_getPerctive(ex_getIns()->_2dCurCam);
		
		//mat4x4_copy(b,a);//将_2dCurCam矩阵的值赋值给fbo->_2dcam

		//printf("比较:%d\n",mat4x4_equal(a,b));
		//if(!mat4x4_equal(a,b,1)){
		//	mat4x4_printf("_2dCurCam",b);
		//	mat4x4_printf("fbocam",a);
		//	getchar();
		//}
	}
	ex_switch2dCam(fbo->_2dcam);


	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->fboName);
	glDrawBuffers(1, fboBuffs);

	{//检查fbo的状态
		GLenum state =	glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if(state != GL_FRAMEBUFFER_COMPLETE){
			printf("fbo state = %0x\n",state);
			return;
		}
	}

	glViewport(0, 0, fbo->texw, fbo->texh);
	glClearColor(1,0,1,1);//绘制成紫红色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除当前的缓冲区
	

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	LStack_ergodic_t(fbo->nodelist,f_renderList,0);
	//f_renderlistCall(sprite_drawRender);//渲染2d节点

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void fbo_pushNode(void* p,void* _node){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	if(_node){
		struct HeadInfo* b;
		b = base_get(_node);
		if( base_findByName(fbo->nodelist,b->name)){
			printf("fbo_pushNode,error! 重名的对象_engine :%s\n",b->name);
			assert(0);
		}else{
			if(b->curType == TYPE_SPRITE_FLIE){
				sprite_set2dCam(p,fbo->_2dcam);//设置sprite当前的cam为fbo的cam
			}

			LStack_push(fbo->nodelist,_node);
		}
	}
}

void* 
fbo_init(int texW,int texH){
	struct FBOTexNode* fbo = (struct FBOTexNode*)tl_malloc(sizeof(struct FBOTexNode));
	
	GLint mirrorTexWidth  = texW;
	GLint mirrorTexHeight = texH;

	GLuint              fboName;
	GLuint				mirrorTexture;//镜像贴图
	GLuint              depthBufferName; //深度缓冲区
	
	memset(fbo,0,sizeof(struct FBOTexNode));

	fbo->nodelist = LStack_create();

	fbo->texw = mirrorTexWidth;
	fbo->texh = mirrorTexHeight;

	// Create and bind an FBO(创建,绑定帧缓存对象fbo)
	glGenFramebuffers(1,&fboName);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboName);

	// Create depth renderbuffer(创建深度缓存)
	glGenRenderbuffers(1, &depthBufferName);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferName);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, mirrorTexWidth, mirrorTexHeight);

	// Create the reflection texture(创建映射贴图)
	glGenTextures(1, &mirrorTexture);
	glBindTexture(GL_TEXTURE_2D, mirrorTexture);
	//作者的bug
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mirrorTexWidth, mirrorTexHeight, 0, GL_RGBA, GL_FLOAT, 0);
  
	// Attach texture to first color attachment and the depth RBO(将texture绑定到RBO)
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture, 0);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferName);//深度缓冲区

	fbo->depthBufferName = depthBufferName;
	fbo->fboName = fboName;
	fbo->tex = mirrorTexture;

	// Make sure all went well
	//gltCheckErrors();
	
	//fbo->_2dspr=f_createSprite(mirrorTexture,texW,texH);
	//fbo->tex = mirrorTexture;
	// Reset framebuffer binding
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//###################################
	fbo->_3dcam = cam_create();
	cam_setPerspect(fbo->_3dcam,45.0,fbo->texw / fbo->texh , 0.1, 10000);

	fbo->_2dcam = cam_create();
	fbo_resize(fbo);
	return fbo;
}

void 
fbo_resize(void*p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	cam_setOrtho(fbo->_2dcam,fbo->texw,fbo->texh,-ex_getIns()->allzBuffer);
}

//销毁fbo对象
void
fbo_dispose(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;

	//释放之前要销毁那些push进去的3d节点
	LStack_delete(fbo->nodelist);
	fbo->nodelist = 0;
	if(fbo->tex){
		glDeleteTextures(1, &fbo->tex);
		fbo->tex = 0;
	}
	//sprite_dipose(fbo->_2dspr);
	//glDeleteTextures(1, textures);
	cam_dispose(fbo->_3dcam);

	// Cleanup RBOs
	glDeleteRenderbuffers(1, &fbo->depthBufferName);

	// Cleanup FBOs
	glDeleteFramebuffers(1, &fbo->fboName);
}

//void* fbo_get_spr(void* p){
//	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
//	return fbo->_2dspr;
//}
void* fbo_getCam(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	return fbo->_3dcam;
}
void* fbo_get2dCam(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	return fbo->_2dcam;
}
void* fbo_getTex(void* p){
	struct FBOTexNode* fbo = (struct FBOTexNode*)p;
	return (void*)fbo->tex;
}