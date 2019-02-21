#ifndef _TMAT_H_
#define _TMAT_H_
#include "tools.h"
#include "tlgl.h"
#include "vmath.h"

/*
	预留的贴图数量,最多可以用8张贴图
*/
#define MATERIAL_TEXTURE_COUNT 8

/*
	材质结构体
	和着色器相关,配合各种参数,调用GLSL
*/
typedef struct GMaterial{

	/*
	*	默认的着色器类型
	*/
	char glslType[G_BUFFER_32_SIZE];

	/*
	*	当前的着色器类型
	*/
	char curGlslType[G_BUFFER_32_SIZE];

	/*
	*	纹理数组,预留个动态空间,这里存储着GPU纹理缓冲区的数据,使用完成后要释放
	*/
	unsigned int texs[MATERIAL_TEXTURE_COUNT];

	//当前贴图路径索引,从0开始
	int curTexIndex;
	//贴图数组
	int texArray[MATERIAL_TEXTURE_COUNT];
	
	struct Vec4 _lineColor;
	float lineWidth;
	float uvScale;//uv缩放值
	struct Vec4 _outlineColor;

	//int diffuseOutLine;
	struct Vec4 _diffuseOutLineColor;
	
	//设置alpha
	float _Alpha;

	int flags;//材质标示符
};

/*
	创建一个材质,隐藏返回引用
	texCount:贴图数量
*/
void*  
tmat_create(const char* glslName,int texCount,...);

/*
	创建一个空贴图的材质
*/
void* 
tmat_createTex(const char* glslType,GLint width,GLint height);
/*
	销毁材质
*/
void tmat_dispose(void* mat);


//float x,float y,float z,float scale
/*
	切换为当前的材质
*/
void tmat_render(void* mat,const char* shader,Matrix44f M);  

/*
	获取当前材质着色器名(这是一个材质默认的一个shader)
*/
void tmat_getShader(void* mat,char* _out,int _outBufferSize);

/*
	 绘制一个Sprite(基于正交视图的多边形),非VBO模式
*/
void tmat_renderSprite(struct GMaterial *_material,const char* shader,Matrix44f mat4x4,float* vertexs,int vertLen,int format,int mode);
/*
 *设置纯色渲染所使用的颜色,不使用的时候默认颜色为0,0,0
 */
void tmat_setFlatColor(struct GMaterial* ptr,float r,float g,float b);

/*
 *	删除指定索引上的贴图
 */
void tmat_disposeTextureByIndex(struct GMaterial* ptr,int index);

/*
	根据索引获取纹理引用
*/
GLuint tmat_getTextureByIndex(struct GMaterial* pmat,int index);
#endif