#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "tools.h"
#include "tl_malloc.h"
#include "gettime.h"
#include "anim.h"
#include "animtor.h"

/************************************************************************
*	动作管理器
*	无关乎底层是用VBO还是直接模式,只是管理动画配置,fps
/************************************************************************/
typedef struct VBOAnim{
	int allLength;//动作总长度
	int _tempLength;
	int curIndex;//当前动作索引

	int* nodeList;//int 数组储存struct ObjVBO*

	/*
	 * 只有>0的时候才是有动作的
	 * 每一帧的间隔时间
	 * fps = 60;
	 * fpsInterval = 1000 / fps = 16.6666;
	 */
	long fpsInterval;
	
	/*
	*	插值
	*/
	long interpolation;

	/*
	*	动作管理器
	*/
	struct Animtor* pAnimtor;
};
static void
addListCallBack(int data,int parms)
{
	struct VBOAnim* anim=(struct VBOAnim*)parms;
	int i = anim->allLength-anim->_tempLength-1;
	anim->nodeList[i] = data;
	//anim->allLength++;
	anim->_tempLength++;
}

struct Animtor* 
anim_get_animtor(void* ptr){
	struct VBOAnim* p = (struct VBOAnim*)ptr;
	if(!p->pAnimtor)
		p->pAnimtor=animtor_create();
	
	return p->pAnimtor;
}
int
anim_total(void* p){
	struct VBOAnim* ptr=(struct VBOAnim*)(p);
	return ptr->allLength;
}
/*
	销毁
*/
void
anim_dispose(void* p){
	struct VBOAnim* anim = (struct VBOAnim*)p;
	if(anim->pAnimtor)
	{
		animtor_dispose(anim->pAnimtor);
	}

	tl_free(anim->nodeList);
	tl_free(anim);
}

/*
	将VBO转化为动作列表
*/
void*
anim_create(struct LStackNode* list)
{
	int length;
	struct VBOAnim* anim = tl_malloc(sizeof(struct VBOAnim));
	memset(anim,0,sizeof(struct VBOAnim));
	
	length = LStack_length(list);
	
	anim->nodeList = tl_malloc(sizeof(int)*length);
	anim->allLength = length;
	LStack_ergodic_t(list,addListCallBack,(int)anim);
	
	return anim;
}

/*
	播放指定索引号的动作
*/
void
anim_play(void* p,int index,void (*callBack)(int,int),int param){
	struct VBOAnim* anim = (struct VBOAnim*)p;
	int data=anim->nodeList[index];
	callBack(data,param);
}

/*
	跳转到一帧
*/
static void 
nextFrame(struct VBOAnim* p)
{
	if(p->pAnimtor)
	{
		animtor_calculateFrame(p->pAnimtor,&p->curIndex);
	}
	else
	{
		p->curIndex++;
		//没有切割过的动作播放所有关键帧
		if(p->curIndex>=p->allLength)
		{
			p->curIndex=0;
		}
	}
	//printf("%d,",p->curIndex);
}
/*
	计算关键帧
*/
static void
calculateFrame(struct VBOAnim* p)
{
	long cur =  get_longTime();

	if(p->fpsInterval >0 &&	cur - p->interpolation > p->fpsInterval)
	{
		nextFrame(p);
		p->interpolation = cur;
	}
}
/*
	播放所有动作

	struct VBOAnim* p,主要设置p->curIndex索引号
*/
void
anim_playByFPS(void* ptr,void (*callBack)(int,int),int parm){
	struct VBOAnim* p = (struct VBOAnim*)ptr;
	if(p->fpsInterval!=0)
	{
		calculateFrame(p);
	}
	anim_play(p,p->curIndex,callBack,parm);
}

void
anim_set_fps(void* ptr,int fps){
	struct VBOAnim* p = (struct VBOAnim*)ptr;
	if(fps==0)
	{
		//printf("fps = %d ,设置了错误的帧率\n",fps);
		//assert(0);
		//动作到当前关键帧,静止不动
		p->fpsInterval = 0;
	}
	else
	{
		p->fpsInterval = 1000 / fps;
	}
}