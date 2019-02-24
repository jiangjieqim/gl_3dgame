#ifndef __TLGL_H__
#define __TLGL_H__
#include "tools.h"

#include <gl/glew.h>
#include <gl/glut.h>

#include "tmat.h"
#include "vmath.h"

/******************************************************************************************/
/*
	单个显示对象的	FLAGS
	2的指数次
*/
#define FLAGS_RENDER_BOUND_BOX		0x01	//渲染显示碰撞盒子,是否渲染场景中3d对象的包围盒,包围盒用来处理射线,碰撞等等
#define FLAGS_DRAW_RAY_COLLISION	0x02	//绘制射线盒子	(是否绘制射线静态包围盒,用于测试 查看射线包围盒),即使没有设置FLAGS_RAY,那么也是可以绘制射线盒子的,这样可以用来查看
#define FLAGS_LINE_RENDER			0x04	//是否显示渲染线框
#define FLAGS_OUTLINE				0x08	//是否显示边缘轮廓(固定管线实现)
#define FLAGS_RAY					0x10	//16是否具有射线拾取功能(此只用于3d拾取,2d拾取不用这个标记)
#define FLAGS_VISIBLE				0x20	//32显示或者隐藏模型,(是否加入渲染队列中)
#define FLAGS_RENDER_DRAWSKELETON	64		//渲染骨骼节点(md5模型才会显示)
#define FLAGS_ANIM_ADAPTIVE			128		//是否根据关键帧的帧率适配关键帧的动画(是否适配fps 1,适配  0,不适配)
#define FLAGS_GLSL_OUTLINE			256		//用GLSL实现的轮廓线
#define FLAGS_DRAW_PLOYGON_LINE		512		//在第一个基本材质上用线框渲染(固定管线模式设置GL_LINE),会取消掉双面显示
#define FLAGS_BUTTON_EFFECT			1024	//设置按钮特效(在有点击事件的前提下才会有该特效)
#define FLAGS_DRAW_NORMAL			2048	//绘制法线
#define FLAGS_DRAG					4096	//Sprite是否可拖拽
#define FLAGS_DRAW_PLOYGON_POINT	8192	//点模式
#define FLAGS_DISABLE_CULL_FACE		16384	//设置显示双面
#define FLAGS_REVERSE_FACE			32768	//反转渲染面

#define SHADER_FLAG_FLAT_COLOR			0x01	//设置平滑着色器


/************************************************************************/
/* headInfo事件定义                                                     */
/************************************************************************/
#define EVENT_RAY_PICK 1				//拾取点击回调

/******************************************************************************************/
#define OBJ_SPAN_COUNT				8		//obj模型文件 数据结构跨距间隔
#define BOX_SIZE 108		//6面体(12个三角形)包围盒尺寸(float个数)

//顶点数据间隔数
enum
{
	UV_GAP		=	2,		//uv索引间隔
	NORMAL_GAP	=	3,		//normal索引间隔
	VERTEX_GAP	=	3		//顶点索引间隔
};

//=================================================================================================
//顶点分布方式
//#define OBJ_UV_VERTEX			0
//#define OBJ_UV_VERTEX_NORMAL	1
enum {
	//OBJ_NULL = -1,
	/*
	 * uv vertex
	 */
	OBJ_UV_VERTEX=0,
	/*
	 * uv vertex normal
	 */
	OBJ_UV_VERTEX_NORMAL=1,
};
struct VertexData
{
	//GLfloat数据的长度
	int vertLen;

	//顶点数据引用,这是一个引用
	GLfloat* vertex;

	//VBO数据
	//struct VBO_Obj* vbo;
};

struct TLGLinfo{
	/*景色深*/
	GLdouble zfar;
	int* pScreenWidth;
	int* pScreenHeight;
};

/*屏幕尺寸*/
//void tlgl_screenSize(float* w,float* h);

void tlgl_getVersion();/*用来检查OpenGL版本，需要GLSL 2.0支持*/

/*重绘,窗口宽高发生变化*/
//void tlgl_reshape(int w,int h);

/*显示版本*/
void tlgl_showGLVersion();
/*为着色器绑定纹理对象*/
//void tlgl_bindTex(GLuint pro,GLsizei imageWidth,GLsizei imageHeight,GLubyte* pImage,GLuint* pTextureID);



/*
*	创建着色器对象
*/
int  tlgl_createShader(GLchar* vertex,GLchar* fragment,GLuint* outProgram);
/*
*	绘制AABB包围盒
*	float r,float g,float b 线框的颜色
*/
void tlgl_AABB(float* trianglesArray,int vertexCount,float* triData,int (*CallBackFun)(char*),int parms,float r,float g,float b,int isDraw);


/*
 *	绘制组合三角形
 *	float* _array			：	顶点数组
 *	int _arrayCount			:	顶点数组元素个数
 *	float r,float g,float b :	RGB颜色(0.0f~1.0f)
 */
void tlgl_drawTrianglesLine(float* _array,int _arrayCount,float r,float g,float b);

//void tlgl_renderBox(float* triData,CallBackFun callBack,/*char* parms*/int parms,float scale);

/*
 * 在屏幕坐标x,y处绘制字体
 */
void tlgl_drawText(const char* s,float x,float y,GLfloat r,GLfloat g,GLfloat b);
void tlgl_drawSign(float* pos,float size);/*绘制一个标记物*/
void tlgl_drawLine(float* a,float* b);/*绘制a到b的连接线*/
//int  tlgl_getCurTime();/*获取当前运行的时间*/
/*创建一个着色器对象*/
void tlgl_glslProgrom(GLuint* outProgra,const char* glslType);
void tlgl_bindVar(GLuint program3D,float* lightDir);
void tlgl_vao(GLuint* vao,int* pLen,GLfloat* verts);

/*
	创建vbo
	GLuint* m_nVBOVertices,GLuint* m_nVBOTexCoords,GLuint* m_nVBONormals,
*/
//void tlgl_createVBO(GLfloat* verts,int len);

/*删除vbo*/
void tlgl_deleteVBO(GLuint* m_nVBOVertices,GLuint* m_nVBOTexCoords);

void tlgl_vbo_render(GLuint* m_nVBOVertices,GLuint* m_nVBOTexCoords,GLuint* m_nVBONormals,int len);/*渲染vbo*/
//float tlgl_vbo_use();/*vbo使用的内存大小*/
/*
	
*/
GLuint tlgl_bindTex(GLuint program3D,GLsizei w,GLsizei h,GLubyte* pImage,const char* tex,int texIndex);
/*
*	线框模式渲染
*/
void tlgl_drawPloyLine();


///*
//	通用渲染接口,用vbo或者非vbo模式渲染
//	vbo==NULL的时候使用非VBO模式
//*/
//void tlgl_renderMode(struct VBO_Obj* vbo,GLfloat* vertex,int vertexCount);
//

/*
	渲染三角形
*/
void tlgl_render_triangles(GLfloat* vertex,int vertexCount);





/*
	根据shader类型获取一个
*/
GLuint tlgl_getShader(const char* glslType);





/*
	获取显存使用和总显存大小(kb)
*/
void tlgl_getGPU_memery(GLint* total_mem_kb,GLint* cur_avail_mem_kb);


/*
	设置vbo尺寸
*/
void tlgl_setVboSize(int size,int isAdd);

/*
	获取vbo尺寸
*/
int tlgl_getVboSize();

/*
 *	设置固定长宽高的包围盒
 *	x,y,z 为长宽高
 */
void tlgl_setAbsBoundBox(float x,float y,float z,float* outTriData);
/*
*	vertexs:	顶点首地址
*	count:		顶点的个数
*	取绝对值最大的做碰撞盒(8个float跨度)
*/
void tlgl_absMax(float* vertexs,int count,float* out);

int 
tlgl_aabb(float* trianglesArray,int vertexCount,float* outData);

void 
tlgl_triangles(float* _array,int _arrayCount,GLenum mode);

GLint
tlgl_UnProject(GLfloat winx, GLfloat winy, GLfloat winz,
			   const GLfloat modelMatrix[16], 
			   const GLfloat projMatrix[16],
			   const GLint viewport[4],
			   GLfloat *objx, GLfloat *objy, GLfloat *objz);

/*
	绘制多边形法线(固定管线)
	float normalSize  法线的长度
*/
void 
tlgl_drawNormal(struct VertexData* rData,float normalSize);

/*
	创建射线拾取盒子
	float* inputVertex	三角形顶点序列
	inoutVertexCount	float个数
*/
void tlgl_createRay(struct VertexData* rayVertexData,float* inputVertex,int inputVertexCount);

/*
 *	绘制一个设置线框的mesh
 */
void tlgl_drawColorLine(
				   Matrix44f matrix44f,struct GMaterial* m,
				   GLfloat* vertex,int vertLen,
				   float r,float g,float b
				   );
#endif