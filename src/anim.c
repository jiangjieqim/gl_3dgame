#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "tools.h"
#include "gettime.h"
#include "anim.h"
#include "animtor.h"

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
anim_get_animtor(struct VBOAnim* p)
{
	if(!p->pAnimtor)
		p->pAnimtor=animtor_create();
	
	return p->pAnimtor;
}

/*
	销毁
*/
void
anim_dispose(struct VBOAnim* anim)
{
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
struct VBOAnim*
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
anim_play(struct VBOAnim* anim,int index,void (*callBack)(int,int),int param)
{
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
anim_playByFPS(struct VBOAnim* p,void (*callBack)(int,int),int parm)
{
	if(p->fpsInterval!=0)
	{
		calculateFrame(p);
	}
	anim_play(p,p->curIndex,callBack,parm);
}

void
anim_set_fps(struct VBOAnim* p,int fps)
{
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