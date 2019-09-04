#ifndef _FBO_TEX_
#define _FBO_TEX_

struct FBOTex
{
	GLuint              fboName;
	//GLuint				textures[1];	//大理石贴图
	GLuint				mirrorTexture;		//镜像贴图
	GLuint              depthBufferName;	//深度缓冲区
	int					texw,texh;			//贴图的宽高
	//struct Sprite*		_2dspr;				//2dSprite,绑定fbo的2dSprite
	void*				mat;//材质对象
	
	// 渲染节点
	void (*callBack)();
};
void* fbo_init();

void fbo_render(void* ptr);

//绑定渲染回调
void fbo_bind(void* ptr,void (*callBack)());

//销毁fbo对象
void fbo_dispose(void* p);
#endif