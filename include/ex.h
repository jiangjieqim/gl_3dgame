#ifndef _EX_H_
#define _EX_H_
#include "tlgl.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


#define  SPRITE_TRIANGLE_COUNT	18	//一个sprite 2d面片需要的三角形(2 * 3 * 3)数据长度

struct TextField
{
	void* base;

	//文本内容
	char* textPtr;

	//文本屏幕坐标
	int x,y;

	//文本颜色
	float r,g,b;

	//文本的高度
	int height;
	
	//缓冲区大小
	int bufferSize;
	
	//文本总长度
	int textWidth;
};

struct TextField* tf_create(int bufferSize,int x,int y,float r,float g,float b);
void tf_dispose(struct TextField* tf);
void tf_setPos(struct TextField* tf,int x,int y);
void tf_setText(struct TextField* tf,const char* str);
/*
 *	获取当前文本的宽度
 */
int tf_getTextWidth(struct TextField* tf);
void tf_copy(struct TextField *tf_source,struct TextField *tf_dest);
/*
	获取文本
*/
//void tf_getStr(struct TextField* tf,char* const buffer,const int bufferSize);

typedef struct Ent3D{
	struct HeadInfo* base;

	//struct VBO_Obj* vbo;

	/*顶点个数*/
	int vertexCount;

	/*顶点数组*/
	GLfloat* vertex;

}Ent3D;


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

//鼠标状态
struct MouseState
{
	GLint button;
	GLint action;
	
	GLint xMouse,yMouse;//点击下的鼠标状态
	
	int moveX,moveY;//当前移动的位置
};

//=============================================================================================================
struct EX
{
	//public
	/*
	*	摄影机坐标
	*/
	float camx,camy,camz;

	/*
		各种枚举定义,二进制位存储
	*/
	int flags;
	
	/*
	frame per second 每一关键帧需要的毫秒
	*/
	int fps;

	/*
	渲染节点对象列表
	*/
	struct LStackNode *renderList;

	/*
	*	背景填充色
	*/
	float bg_r,bg_g,bg_b;

	//public
	/*
	*	景深
	*/
	GLdouble zFar;

	/*
	*	点击拾取模型
	*/
	struct Ent3D* pick3DObject;

	/*
		屏幕尺寸
	*/
	float _screenWidth,_screenHeight;

	/*
		渲染循环回调
	*/
	void (*loopCallBack)(void);

	void (*drawLine_callBack)(void);

	/*
		当前鼠标状态
	*/
	struct MouseState mouseState;

	/*
		点击回调方法引用
	*/
	void (*mRayPickCallBack)(struct HitResultObject* hit);


	///*
	//	sprite顶点数据源,引擎释放后要清理该资源
	//*/
	//struct VertexData spriteVert;

	/*
		是否已经点击到了界面
	*/
	int isHitRaySprite;

	/*
		日志文本
	*/
	struct TextField* logTf;

	lua_State* mylua;

	/*
		UI 视角的z深度
	*/
	int zBuffer;

	/*
		总共的Z深度
	*/
	int allzBuffer;

	/*
		顶点总数
	*/
	int allVertexTotal;

	/*
		透视矩阵
	*/
	Matrix44f perspectiveMatrix;
	/*
		模型视图矩阵
	*/
	Matrix44f modelViewMatrix;

	Matrix44f ui_perspectiveMatrix;
	Matrix44f ui_modelViewMatrix;

	float camRotateX,camRotateY,camRotateZ;

	//struct Atals* myAtals;//废弃

	/*
		默认材质
	*/
	void* defaultMaterial;
};

//接口定义
/*

	1.转化为编辑网格
	2.运行脚本md2Export.ms
	3.勾选Save Anumation (Frame Step设置成1),Generate Normal,Active Time Segment
	4.Export

*	加载并解析md2模型动画
*	字节流序列使用大端方式
*/
void md2_load(struct MD2_Object* _md2,const char* str,int len,const char* name);
/*
*	渲染回调
*/
void md2_render(struct MD2_Object* _md2);
/*
*	销毁
*/
void md2_dispose(struct MD2_Object* _md2);


void ent_dispose(struct Ent3D* p);
//=====================================================
/*
创建引擎设备上下文
*/
struct EX* ex_create();

/*
渲染循环
*/
void render(void);

/*
根据name获取HeadInfo;
*/
struct HeadInfo* ex_find(struct EX* p,const char* name);
void* ex_find_node(const char* name);
/*
初始化
*/
void ex_init(struct EX* p,GLdouble zfar);

/*
销毁引擎
*/
void ex_dispose(struct EX* p);

/*
创建md5模型
*/
void* createMD5(struct EX* engine,struct XMLSList* xml,const char* id,const char* targetName,float x,float y,float z,float _scale);


/*
创建ent 解析OBJ静态模型对象 使用VBO
flags:	标识
*/
struct Ent3D* load_obj(const char* targetName,const char* mesh_s,
	float x,float y,float z,
	float scale);

/*
	加载md5模型
*/
void* load_md5(const char* name,const char* url,float x,float y,float z,float scale);

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
struct MD2_Object* load_md2(
	const char* name,
	const char* model,
	float x,float y,float z,
	float scale
	);
/*
	加载动作配置,设置
*/
int load_animConfig(void* ptr,char* animConf,long fps);

/*
gluUnProject -- map window coordinates to object coordinates 
将屏幕坐标映射到世界坐标

gluUnProject maps the specified window coordinates into object coordinates using modelMatrix, 
projMatrix, and viewport. The result is stored in objx, objy, and objz. 
A return value of GL_TRUE indicates success, and GL_FALSE indicates failure


二维三维坐标转换函数： gluUnProject()
此函数的具体用途是将一个OpenGL视区内的二维点转换为与其对应的场景中的三维坐标。
转换过程如下图所示（由点P在窗口中的XY坐标得到其在三维空间中的世界坐标）：
*/
void mousePlot(GLint button, GLint action, GLint xMouse, GLint yMouse);

/*
	鼠标移动
*/
void mouseMove(int x,int y);

/*
普通按键控制a,1,2,3
*/
void onKeyboardCallBack(unsigned char key, int x, int y);

/*
设置摄影机的坐标
*/
void setCamPos(float x,float y,float z);

/*
设置渲染的背景颜色
*/
void setBgColor(float r,float g,float b);

/*
	开启引擎某个状态
*/
void set(int type);

/*
	初始化绑定相关接口
*/
void ex_bind(int argc,char** argv);

/*
	关闭某个状态(重置该状态位为0)

	0001
&	0000
=	0000

	0001
|	0000
=	0001

*/
void reset(int t);
/*
	获取引擎当前的状态
*/
//int getState(int type);

/*
	打印引擎当前信息
*/
void ex_info();

/*
	切换标识
*/
void ex_switch_baseFlags(struct HeadInfo* _node,int flag);

/*
	鼠标是否常按着
*/
int ex_mouseIsLeftDown();
/*
	更新场景中的界面坐标
*/
void ex_update_uiPos();

/*
	添加一个渲染节点
*/
void ex_addNode(struct EX* p, void* _node);

/*
	获取引擎句柄
*/
struct EX* ex_getInstance();
/*
	加载一个obj,或者md2等
	float rayRadius	:射线拾取半径
*/
int load_model(char* name,const char* url,float x,float y,float z,float scale);
/*
	为ptr设置材质material
*/
void setMaterial(void* ptr,void* material);
/*
	设置ptr的状态
*/
void setv_ptr(void* ptr,int flags);

/*
	设置动作
*/
void setanim(void* ptr,const char* animKey);
/*
	在屏幕左上角显示日志文本
*/
void ex_showLog(const char* buffer);
/*
 *	显示一个数字日志
 */
void ex_showLogFloat(float v);

/*
	回调lua函数

	调用一个参数的lua函数
	const char* luaFunName	:lua中的函数名
	int parm				:参数引用


	lua_pcall(lua_State *L, int nargs, int nresults, int errfunc)
	nargs:参数个数，nresults：返回值个数，errFunc：错误处理函数，0表示无，表示错误处理函数在栈中的索引


	lua:
	===================================
	function btnCallBackEvt(spriteName)
	end
*/
void ex_callParmLuaFun(const char* luaFunName,const char* parm);
/*
	调用lua方法luaFunName(lua函数必须是lua全局函数),传递一个int型的参数
*/
void ex_callIntLuaFun(const char* luaFunName,int value);
/*
	获取zBuffer
*/
int ex_zBuffer();

/*
	加载VBO模式的模型
*/
int load_VBO_model(char* name,const char* url);

//void updatePerspectiveMatrix( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar);
void ex_updatePerspctiveModelView();
/*
 *设置对象的朝向目标
 */
void setLookTarget(void* ptr,float x,float y,float z);
/*
	寻找节点
*/
void* ex_findNodeByName(struct EX* ptr,const char* name);
/*
	获取引擎的一个默认材质,共享使用,在处理没有材质的时候后调用
*/
void* ex_get_defaultMaterial();
/*
	绘制线段
*/
void ex_drawline();
#endif