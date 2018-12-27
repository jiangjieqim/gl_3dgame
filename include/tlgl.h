#ifndef __TLGL_H__
#define __TLGL_H__
#include "tools.h"

#include <gl/glew.h>
#include <gl/glut.h>

#include "tmat.h"



/*shader类型*/
//#define SHADER_DIFFUSE 0x01

//#define SHADER_BUMP 0x03
//
//#define SHADER_NORMAL_MAP	0x04	//法线材质
//#define SHADER_MULT_TEXTURE 0x05	//多重纹理
//#define SHADER_SHADOW_MAP	0x06	//阴影
/*

EX->flags(32位,存储32个状态)

EX_FLAGS_RAY:
射线拾取开关枚举,这是一个总开关
设置静态碰撞盒子,设置成TRUE就可以射线检测,不检测射线拾取,这是一个整体开关,单个模型可以单独设置
*/
#define EX_FLAGS_RAY 0x01

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




#define SHADER_FLAG_FLAT_COLOR			0x01	//设置平滑着色器


/******************************************************************************************/
#define OBJ_SPAN_COUNT				8		//obj模型文件 数据结构跨距间隔

struct TLGLinfo{
	/*景色深*/
	GLdouble zfar;
	int* pScreenWidth;
	int* pScreenHeight;
};

/*
*	动作管理结构体
*	用于切换关键帧,播放动作
*/
typedef struct FrameAnim
{
	/*
	 *	当前动作的起始关键帧和结束关键帧索引号(从0开始)
	 */
	int frameStart;
	/*
	 *	结束帧索引,该索引从0开始
	 */
	int frameEnd;
	
	/*
	 * 每一帧的间隔时间
	 * fps = 60;
	 * fpsInterval = 1000 / fps = 16.6666;
	 */
	long fpsInterval;

	/*
	 * 当前的插值
	 */
	long _subTick;

	/*
	*	当前关键帧索引号(从0开始)
	*/
	int curFrame;

	
	/*
	*当前动作
	*/
	char curAnim[G_BUFFER_16_SIZE];
	/*
	*前一次动作
	*/
	char oldAnim[G_BUFFER_16_SIZE];
	
	/*
	*	动作片段配置
	*	"stand,0,39|run,40,45|"
	*/
	char animConfig[G_BUFFER_128_SIZE];

	/*
	*	播放一次动作回调
	*/
	//CallBackFun playOnceCallBack;
	void (*playOnce)(struct FrameAnim*);
	

}FrameAnim;

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
void tlgl_AABB(float* trianglesArray,int vertexCount,float* triData,CallBackFun callBack,int parms,float r,float g,float b,int isDraw);


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
	渲染一个带材质的模型
*/
void base_renderByMaterial(struct HeadInfo* base);

//=========================================================================================================
struct MaterialList
{
	struct LStackNode* texList;		/*贴图指针*/

	struct LStackNode* vboList;		/*vbo列表*/
};

/*
	贴图结构体
*/
struct Tex
{
	int w,h;
	char texName[G_BUFFER_64_SIZE];			/*贴图名*/
	//GLubyte* image;							/*贴图数据*/
	int use;							/*使用的个数*/
	int mTextureID;
};
//
///*
//	材质结构体
//*/
//struct Material{
//	GLuint programObject;					/*着色器对象*/
//	GLuint mTextureID;						/*Texture指针*/
//};

struct VertexData
{
	//GLfloat数据的长度
	int vertLen;
	
	//顶点数据引用,这是一个引用
	GLfloat* vertex;

	//VBO数据
	//struct VBO_Obj* vbo;
};

/*
	Base 包含模型对象的基本操作,位移,旋转,缩放
	这里和模型类型是否是md2,md5,obj无关,类似于它们的父类
*/

typedef struct Md2Info
{
	//开始帧
	int s;
	
	//结束关键帧
	int e;

	//动作片段名
	char* animName;

	//记录是否找到
	int findState;
}Md2Info;

///*
//	顶点数据
//*/
//struct VertexData
//{
//	float* vert;	//顶点引用
//	int count;		//float 数据个数
//};

/*
	基础结构体,用于ent3d和md2继承
*/
typedef struct HeadInfo{

	/*类型*/
	int curType;
	
	/*名字,可用于查询,作为唯一实例*/
	char name[G_BUFFER_32_SIZE];
	/*文件后缀*/
	char suffix[G_BUFFER_16_SIZE];

	/*坐标*/
	float x,y,z;

	float rx,ry,rz;

	/*缩放值*/
	float scale;
	
	/*
		材质引用	===>struct GMaterial* tmat;
	*/
	struct GMaterial* tmat;

	//char curShader[G_BUFFER_32_SIZE];//当前使用的shader

	///*
	//	vbo引用
	//*/
	//void* vboPtr;

	/*
	*	动态包围盒顶点数组引用
	*/
	float* boxVertPtr;

	///*
	//*	静态包围盒顶点数组,用来做射线拾取的
	//*/
	//float* staticBoxVert;

	/*
		静态顶点数据
	*/
	struct VertexData rayVertexData;

	///*
	//*	静态包围盒的长宽高
	//*/
	//float boundWidth,boundHeight,boundLength;

	/*
		标识位,标识各种状态
	*/
	int flags;
	
	/*
	*	动作管理器
	*/
	struct FrameAnim* frameAnim;

	///*
	//*	渲染回调
	//*/
	//void (*render)(struct HeadInfo*);

	/*
	*	被渲染数据(顶点数据)
	*/
	struct VertexData rData;

	/*
		渲染节点回调接口,此接口做向外扩展用
	*/
	void (*renderCallBack)(void* ptr);

	/*
		输入的变换矩阵
	*/
	Matrix44f m;
	
	/************************************************************************/
	/* 四元数矩阵															*/
	/************************************************************************/
	Matrix44f quat_m;

	struct Vec3 target;
	int lookat;
	
	//做计时器存储的变量
	long time;
	
	/*单个对象的关键帧帧率*/
	int fpsInterval;
	

	/*
	自定义设置当前的关键帧索引,0开始										
	-1标示没有使用的状态,0代表使用指定的关键帧,初始化的时候设置成-1
	*/
	int custframe;
	
	/*
		lua拾取回调
	*/
	char* luaPickCallBack;
	/*
		拾取框颜色
	*/
	float boxR,boxG,boxB;

}HeadInfo;

//接口定义部分

/*
*	curType:			模型类型
*	name:				模型名,用于搜索
*	x,y,z:				初始化坐标
*	const char* tex:	贴图路径
*/
struct HeadInfo* base_create(int curType,const char* name,float x,float y,float z);

/*
	设置坐标
*/
void base_setPos(struct HeadInfo* base,float x,float y,float z);
void base_set_position(struct HeadInfo* base,struct Vec3* pos);
/*
	更新矩阵
*/
void updateMat4x4(struct HeadInfo* base);
/*
*	绘制包围盒
*	void (*translateCallBack)(struct HeadInfo*)	:矩阵变形回调
*/
//void base_drawBoundBox(struct HeadInfo* base,float* vertices,int vertCount,void (*translateCallBack)(struct HeadInfo*));

/*
 *	静态碰撞盒子 只是获取碰撞盒子数据不进行绘制
 *	初始化静态碰撞盒
 */
void base_md2_staticBox(struct HeadInfo* base);

/*
	创建射线拾取盒子
	float* inputVertex	三角形顶点序列
	inoutVertexCount	float个数
*/
void base_createRayVertex(struct VertexData* rayVertexData,float* inputVertex,int inputVertexCount);

/*
*	设置动作
*/
void base_curAnim(struct FrameAnim* frameAnim,const char* anim);

/*
*	该接口只播放一次动作,播放完一次循环后,恢复上一个动作
*/
void base_playOnce(struct FrameAnim* frameAnim,const char* anim);

/*
*	计算关键帧
*/
void base_calculateFrame(struct FrameAnim* frameAnim);

/*
	获取渲染模式是否是线框还是实体模式
*/
int base_get_ploygonLineMode(struct HeadInfo* base);

/*
*	在渲染列表射线拾取
*	struct Vec3* offset	这是一个偏移向量,可以作为视图矩阵的偏移值
*/
void base_seachPick(struct LStackNode* list,struct Vec3* nearPoint,struct Vec3* farPoint,struct HitResultObject* last);

/*
*	销毁
*/
void base_dispose(struct HeadInfo* base);

struct HeadInfo* base_get2(void* p);

/*
	填充 HeadInfo 结构体,根据head信息获取相关信息,需要尾部传递一个HeadInfo*引用
*/
//void base_get(void* point,struct HeadInfo* base);

//实心渲染
void base_renderFill(struct HeadInfo* node);

/*
	使用ms毫秒移动到x,y,z坐标
	在单位时间内移动到一个目标向量,这是一个渐变的过程
*/
void base_trace(struct HeadInfo* base,float x,float y,float z,float ms);

///*
//	初始化静态包围盒
//*/
//void base_boxStaticVert_init(struct HeadInfo* base);

/*
	是否可以播放动画
*/
int base_isCanAnimation(struct HeadInfo* ptr);
/*
	设置一个点击拾取的lua函数回调
*/
void base_setLuaPick(struct HeadInfo* base,const char* luaFunctionName);

/************************************************************************/
/* 设置后缀                                                    */
/************************************************************************/
void base_set_suffix(struct HeadInfo* base,const char* str);
/*
	双面绘制
*/
GLboolean base_cullface(struct HeadInfo* base);
//==========================================================================



/*
	开始检测
	mRayPickCallBack 回调
*/
void hit_mouse(GLint xMouse, GLint yMouse,float screenWidth,float screenHeight,
			   struct LStackNode *renderList,
			   void (*mRayPickCallBack)(struct HitResultObject*));


/*
	根据shader类型获取一个
*/
GLuint tlgl_getShader(const char* glslType);


//struct VBO_Obj* mat_loadModel(const char* path,int arrayLength);
/*
	绘制静态包围盒,该包围盒是自定义长宽高的
*/
void tlgl_staticBox(struct HeadInfo* base);


/*
	获取显存使用和总显存大小(kb)
*/
void tlgl_getGPU_memery(GLint* total_mem_kb,GLint* cur_avail_mem_kb);

/*
 *	绘制线框
 */
void drawLineByColor(struct HeadInfo* base,GLfloat* vertex,int vertLen,float r,float g,float b);

/*
	设置vbo尺寸
*/
void tlgl_setVboSize(int size,int isAdd);

/*
	获取vbo尺寸
*/
int tlgl_getVboSize();
#endif