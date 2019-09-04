#include <gl/glew.h>
#include <gl/glut.h>

#include "tl_malloc.h"
#include "jgl.h"

#include "fbotex.h"
#include "tmat.h"
#include "base.h"
#include "sprite.h"

static const GLenum fboBuffs[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

void
fbo_render(void* ptr){
	struct FBOTex* fbo = (struct FBOTex*)ptr;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->fboName);
	glDrawBuffers(1, fboBuffs);

	glViewport(0, 0, fbo->texw, fbo->texh);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除当前的缓冲区

	
	if(fbo->callBack){
		fbo->callBack();
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
void fbo_bind(void* ptr,void (*callBack)()){
	struct FBOTex* fbo = (struct FBOTex*)ptr;
	fbo->callBack = callBack;
}
//#define _TestTex_
void* 
fbo_init(){
	struct FBOTex* fbo = (struct FBOTex*)tl_malloc(sizeof(struct FBOTex));

	GLint mirrorTexWidth  = 256;
	GLint mirrorTexHeight = 256;

	GLuint              fboName;
	//GLuint				textures[1];//大理石贴图
	GLuint				mirrorTexture;//镜像贴图
	GLuint              depthBufferName; //深度缓冲区

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
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferName);

	fbo->depthBufferName = depthBufferName;
	fbo->fboName = fboName;
	fbo->mirrorTexture = mirrorTexture;

	// Make sure all went well
	//gltCheckErrors();

	
	//创建2dSprite
	/*fbo->_2dspr = sprite_create("fbo1",0,0,256,256,0);
	*/
	{
		struct Sprite* spr = 0;
#ifdef _TestTex_
		GLuint tex =jgl_loadTex("//resource//texture//1.tga");
#endif
		void* mat= tmat_create_empty("_spritevbo");
		//tmat_setTexFromGPU(mat,0,mirrorTexture);
		tmat_setID(mat,1);
		
#ifdef _TestTex_
		tmat_pushTex(mat,tex);//mirrorTexture
#else
		tmat_pushTex(mat,mirrorTexture);//mirrorTexture
#endif
		//printf("mirrorTexture:%d\n",mirrorTexture);
		fbo->mat = mat;
		
		spr = sprite_create("sprite0",100,100,256,256,0);
		sprite_rotateZ(spr,-PI/2);//sprite旋转90度
		sprite_rotateX(spr,PI);
		base_setv(spr,FLAGS_REVERSE_FACE);

		spr->material = mat;
	}
	
	// Reset framebuffer binding
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return fbo;
}

//销毁fbo对象
void
fbo_dispose(void* p){
	struct FBOTex* fbo = (struct FBOTex*)p;
	glDeleteTextures(1, &fbo->mirrorTexture);
	//glDeleteTextures(1, textures);

	// Cleanup RBOs
	glDeleteRenderbuffers(1, &fbo->depthBufferName);

	// Cleanup FBOs
	glDeleteFramebuffers(1, &fbo->fboName);
}