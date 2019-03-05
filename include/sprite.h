#ifndef _SPRITE_H_
#define _SPRITE_H_

//#define USE_VBO_SPRITE	//是否使用VBO模式的Sprite

//2d触发界面
struct Sprite
{
	void* base;
	float pos_x;		//表示在正交投影坐标系(左下角为坐标原点)的坐标，
	float pos_y;
	float pos_z;
	
	//z轴的缩放
	float zScale;

	int mouseLocalX,mouseLocalY;

	//float scaleWH;

	//屏幕坐标,从左上角开始计算0,0
	float screenX;
	float screenY;

	float mWidth;		//sprite所屏幕像素单位
	float mHeight;
	int m_bPressed;

	//int isLock;

	int mouseDownX,mouseDownY;//鼠标左键点击下的坐标

	GLfloat* vertexs;//顶点缓存数据,使用完之后要删除tl_free
	int	vertLen;

	//void* material;//按钮材质

	struct Atals* atals;//图集
	
	/************************************************************************/
	/* 材质引用                                                    */
	/************************************************************************/
	void* material;
	
	Matrix44f mat4x4;
	
	/*
		做射线拾取的时候使用
	*/
	float *hitTriangle;
	//float *triangle2;
	
	/*
		Sprite点击回调
	*/
	void (*clickCallBack)(struct Sprite* ptrSprite,int localX,int localY);
	
	/*
		lua中回调的函数名
	*/
	//int callLuaFunName[G_BUFFER_32_SIZE];

	/*
		lua中的sprite移动事件更新
	*/
	//char* callLuaDragMove;
	
	/*
	 *鼠标常按事件
	 */
	//char* callLuaMouseDown;

	/*
		以Sprire相对的0,0坐标设置的拖拽范围,如果设置的是0,没有范围限制
	*/
	int	dragX,dragY,dragWidth,dragHeight,oldx,oldy;

	/*
		--0:横向拖动滑块,1纵向拖动滑块
	*/
	int	dragDirection;

	//char* luaTablePtr;
	int parseType;
	int useVBO;//是否使用VBO模式
	struct Obj_vbo_model* vbo;


};
/*
	初始化按钮
*/
struct Sprite* sprite_create(
	char* _spriteName,
	int x,int y,
	int width,int height,
	void (*clickCallBack)(struct Sprite* ,int ,int ));

/*
	设置sprite的UV
*/
void sprite_setUV(struct Sprite* sprite,float x,float y,float w,float h);

/*
	绘制一个sprite 
*/
void sprite_drawRender(int data);

/*
	按钮事件特效
*/
void sprite_action(const int data);

/*
	渲染更新ui节点
*/
void sprite_updatePos(int data);
/*
	删除sprite节点
*/
void sprite_dipose(struct Sprite* spr);

/*
	根据图集中的key设置位图,atlas其实是一组配置
*/
void sprite_texName(struct Sprite* spr,const char* texName,struct AtlasNode* ptr);
/*
	更新坐标
*/
void sprite_setpos(struct Sprite* spr,int x,int y);

/*
	鼠标左键点击后拖拽只用该接口更新sprite坐标
*/
void sprite_mouseMove(int data);

/*
	是否可以渲染
*/
int sprite_isEnable(int data);

/*
	设置sprite可拖动范围
	x,y:可拖拽的坐标
	w:可拖拽的宽度
	h:可拖拽的高度
*/
void sprite_setDragScope(struct Sprite* pSpr,int x,int y,int w,int h);

/*
 *旋转rz轴 PI的n倍数,只变换渲染轴向
 */
void sprite_rotateZ(struct Sprite* ptr,float rz);
/*
	重置sprite的尺寸
*/
void sprite_resize(struct Sprite* spr,int w,int h);

#endif