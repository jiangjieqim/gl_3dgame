//MD5接口
#ifndef _MD5_H_
#define _MD5_H_
#include "tools.h"

//==========================================================================

struct MD5{
	void* base;
	//========================================
	/*单个对象的关键帧帧率*/
	int fpsInterval;
	int frameTicket;				/*关键帧计数器*/
	void* model;					/*md5结构体数据*/
	Vec3_t *vertexArray;			/*顶点数据*/
	Vec8_t *vertexOut;
	Vec8_t *trianglesArray;
	GLuint *vertexIndices;			/*顶点索引数据*/
	
	void* curAnim;
	struct LStackNode* pAnimList;	//动作列表
	void* animInfo;					//anim_info_t
	void *skeleton;					//md5_joint_t
};
/*
	加载网格模型
*/
void md5_loadMesh(struct MD5* p,const char* path);

/*
	加载动画文件
 */
void md5_loadAnim(struct MD5* p, const char* animFile,const char* animName);
/*
	设置动画
	#define ANIM_STR "stand,walk,att,skill01,skill02"
	
	anim=0 stand
	anim=1 walk
*/
void md5_setAnim(void* _md5,const char* anim);

/*
	渲染
*/
void md5_render(struct MD5* p,int fps);

/*
	销毁md5
*/
void md5_dispose(struct MD5* out);
/*
	获取md5动画的关键帧数
*/
int md5_getFrameNum(void* _md5,char* animType);

/*
	获取当前动画的所有总关键帧数
*/
int md5_getTotalFrame(void* p);

/*
	获取当前的md5关键帧索引
*/
int md5_getFrameIndex(void* _md5);
/*
	绘制骨骼节点
*/
void md5_drawSkeleton(void* ptr);

#endif