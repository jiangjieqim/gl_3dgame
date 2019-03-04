#ifndef _BASE_H_
#define _BASE_H_

/*
	基础结构体,用于ent3d和md2继承
*/
typedef struct HeadInfo{
	/*事件引用*/
	void* evtList;
	/*类型*/
	int curType;
	
	/*名字,可用于查询,作为唯一实例*/
	char name[G_BUFFER_32_SIZE];
	/*文件后缀*/
	char suffix[G_BUFFER_16_SIZE];

	/*坐标*/
	double x,y,z;

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
	//void (*renderCallBack)(void* ptr);
	
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
	/*是否是一个Node*结构*/
	int isNode;
	
	/*
		所属的父对象
	*/
	void* parent;
	void* _move_tp;
	void* _ry_tp;
}HeadInfo;

/*
	渲染一个带材质的模型
*/
void base_renderByMaterial(struct HeadInfo* base);



/*
	Base 包含模型对象的基本操作,位移,旋转,缩放
	这里和模型类型是否是md2,md5,obj无关,类似于它们的父类
*/


///*
//	顶点数据
//*/
//struct VertexData
//{
//	float* vert;	//顶点引用
//	int count;		//float 数据个数
//};

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
/************************************************************************/
/* 设置对象的缩放值                                                                  
/************************************************************************/
void 
base_set_scale(void* p,float scale);
/*
	更新矩阵
*/
void base_updateMat4x4(struct HeadInfo* base);
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

struct HeadInfo* base_get(void* p);

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
//int base_isCanAnimation(struct HeadInfo* ptr);
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
 *ms毫秒y轴旋转至ry
 **/
void 
base_rotate_to(HeadInfo* bp,float ms,float ry);

/*
	开始检测
	GLint xMouse, GLint yMouse:鼠标拾取的2d屏幕坐标
	mRayPickCallBack 回调
*/
void base_hit_mouse(GLint xMouse, GLint yMouse,float screenWidth,float screenHeight,
			   struct LStackNode *renderList,Matrix44f perspectiveMatrix,Matrix44f modelViewMatrix,
			   void (*mRayPickCallBack)(struct HitResultObject*));

/*
	让角色朝向坐标x y z
*/
void base_look_at(HeadInfo* p,float x,float y,float z);
/*
	位移到x y z坐标
*/
void 
base_move(HeadInfo* ptr,int ms,
		  float x,float y,float z,
		  void (*endCallBack)(void*),
		  void (*updateCallBack)(void*)
		  );
#endif