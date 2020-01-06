#ifndef _BASE_H_
#define _BASE_H_

#define BOX_R 1.0 //碰撞盒的r值

/*
	基础结构体,用于ent3d和md2继承
*/
typedef struct HeadInfo{
	/*事件引用*/
	void* evtList;
	/*类型*/
	int curType;
	
	/*名字,可用于查询,作为唯一实例*/
	char* name;

	/*坐标*/
	double x,y,z;

	float rx,ry,rz;
	
	//旋转轴和角度
	float ax,ay,az,angle;

	/*缩放值*/
	float scale;
	
	/*
		材质引用	===>struct GMaterial* tmat;
	*/
	struct GMaterial* tmat;

	/*
		标识位,标识各种状态
	*/
	int flags;
	
	/*
		输入的变换矩阵
	*/
	Matrix44f* m;

	//矩阵发生变化的时候设置为1
	GLbyte changed;
	/*
		当前是2dcam的时候就是2dcam(sprite)
		用接口sprite_set2dCam来设置
		当前的2d camera引用,该引用来确定该sprite在哪个2d camera矩阵空间,
		 
		当前是3d物体的时候就是3dCam(md2,obj...)
	*/
	void* cam;
}HeadInfo;

/*
	渲染一个带材质的模型
*/
void base_renderByMaterial(struct HeadInfo* base,GLfloat* vertex,int vertLen);



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
int base_get_ploygonLineMode(int flag);

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
void base_renderFill(struct HeadInfo* node,GLfloat* vertex,int vertLen);

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
//void base_setLuaPick(struct HeadInfo* base,const char* luaFunctionName);

/************************************************************************/
/* 设置后缀                                                    */
/************************************************************************/
//void base_set_suffix(struct HeadInfo* base,const char* str);
/*
	双面绘制
*/
GLboolean base_cullface(int flag);
//==========================================================================

/*
 *ms毫秒y轴旋转至ry
 **/
//void base_rotate_to(HeadInfo* bp,float ms,float ry);

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


void base_setv(void* ptr,int flag);
void base_resetv(void* ptr,int flag);
int base_getv(void* ptr,int flag);

//在链表中搜索渲染节点
void* base_findByName(void* list,const char* name);


//真正的更新矩阵的接口,优化性能,保证每一个frame只计算一次矩阵
void
base_realUpdateMat4x4(void* p);

//设置cam句柄
void
base_set_cam(void* p,void*cam);

//获取cam句柄
void*
base_get_cam(void* p);
/*
	v:1 显示 0 隐藏
*/
void base_set_visible(struct HeadInfo* base,int v);

//绕自定义轴旋转
void base_rotate_vec(struct HeadInfo* base,float ax,float ay,float az,float angle);

#endif