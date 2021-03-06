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
	int id;
	/*
	*	默认的着色器类型
	*/
	char* glslType;//[G_BUFFER_32_SIZE];

	/*
	*	当前的着色器类型
	*/
	//char curGlslType[G_BUFFER_32_SIZE];

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
	//int _DiscardAlpha;//丢弃alpha是0的片段

	int flags;//材质标示符
	
	//着色器传递变量回调
	void (*updateVarCallback)(void* material,Matrix44f m,void* param,void* cam);
	//存储一些着色器中需要使用的变量
	void* shaderVars;
	
	//shader的参数引用(九宫格引用)
	void* params;

	//渲染模式		GL_TRIANGLES GL_LINE_STRIP
	//				GL_POINTS GL_LINES
	GLbyte rendermode;

}GMaterial;

/*
	创建一个材质,隐藏返回引用
	texCount:贴图数量
*/
void*  
tmat_create(const char* glslName,int texCount,...);

//设置id
void tmat_setID(void* m,int id);

//获取材质id
int tmat_getID(void* m);

//添加一个贴图句柄到材质句柄
void 
tmat_pushTex(void* mat,GLuint tex);

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

	void* cam当前材质所使用的camera引用,如果是sprite就是正交cam,
	如果是3d对象就是透视cam,它们又可以在各自的FBO空间内
*/
void tmat_render(void* mat,const char* shader,Matrix44f M,void* cam);  

/*
	获取当前材质着色器名(这是一个材质默认的一个shader)
*/
//void tmat_getShader(void* mat,char* _out,int _outBufferSize);

/*
	 绘制一个Sprite(基于正交视图的多边形),非VBO模式
*/
//void tmat_renderSprite(struct GMaterial *_material,const char* shader,Matrix44f mat4x4,float* vertexs,int vertLen,int format,int mode,void* cam);
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

void* 
tmat_create_rgba(const char* glslType,GLint width,GLint height,GLenum rgbaType);

/*
*	存储在到GPU显存中一张贴图,指定texIndex索引位置存储一张贴图
*	int texIndex		:		纹理索引
*	GLuint tex			:		纹理句柄引用
*/
void 
tmat_setTexFromGPU(struct GMaterial* mat,int texIndex,GLuint tex);

//创建一个空材质对象,此材质对象贴图属性
void* 
tmat_create_empty(const char* glslType);




/*
 *
 *	上传模型变换矩阵到顶点着色器
 ****
	变换
	自缩放,旋转
	缩放==> 平移到(0,0,0) ==>缩放 ==>恢复到原来坐标
	旋转==> 平移到(0,0,0) ==>旋转 ==>恢复到原来坐标

*/
void 
tmat_uploadMat4x4(GLint location_mat4x4,Matrix44f _out_mat4x4);

/*
	着色器更新贴图数据
	检查有无纹理引用
*/
void 
tmat_updateTexture(GLuint program3D,struct GMaterial* mat);

//打印材质对象中的所有贴图句柄
void
tmat_print_tex(void* mat);
//创建一个九宫格材质
void* tmat_create_9grid(struct Atals* atals,const char* icon);

#endif