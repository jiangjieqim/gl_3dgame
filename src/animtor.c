#include <string.h>
#include <assert.h>

#include "tools.h"
#include "tl_malloc.h"
#include "animtor.h"

/*
	动作片段
*/
struct AnimatorFrame
{
	char* name;
	int s;
	int e;
};

struct Animtor* 
animtor_create()
{
	struct Animtor* p = tl_malloc(sizeof(struct Animtor));
	memset(p,0,sizeof(struct Animtor));
	p->list = LStack_create();
	return p;
}
static void 
disposeAnimatorFrame(struct AnimatorFrame* animatorFrame)
{
	tl_free(animatorFrame->name);
	tl_free(animatorFrame);
}

static void
disposeList(int data,int parms)
{
	disposeAnimatorFrame((struct AnimatorFrame*)data);
}
void 
animtor_dispose(struct Animtor* p)
{
	LStack_ergodic_t(p->list,disposeList,(int)p);
	LStack_delete(p->list);	
	tl_free(p);
}

void 
animtor_push(struct Animtor* p,const char* animKey,const int start,const int end)
{
	int len;
	struct AnimatorFrame* clip = tl_malloc(sizeof(struct AnimatorFrame));
	memset(clip,0,sizeof(struct AnimatorFrame));
	
	len = strlen(animKey);
	clip->name=tl_malloc(len+1);
	memset(clip->name,0,len+1);
	memcpy(clip->name,animKey,len);

	clip->s = start;
	clip->e = end;
	LStack_push(p->list,(int)clip);
}

struct FindResult
{
	int r;
	char* name;
	int s;
	int e;
};

static void
find(int data,int parms)
{
	struct FindResult* pfind = (struct FindResult*)parms;
	struct AnimatorFrame* p=(struct AnimatorFrame*)data;
	if(!strcmp(p->name,pfind->name))
	{
		pfind->r = 1;
		pfind->s = p->s;
		pfind->e = p->e;
	}
}
/*
	获取一个动作片段的配置
	return 0未找到 1获取成功
*/
static int 
f_animtor_get(struct Animtor* p,char* const animKey,int* const start,int* const end)
{
	struct FindResult pFind;
	memset(&pFind,0,sizeof(struct FindResult));
	pFind.name = animKey;
	LStack_ergodic_t(p->list,find,(int)&pFind);
	*start = pFind.s;
	*end = pFind.e;
	return pFind.r;
}

int 
animtor_setcur(struct Animtor* p,char* const animKey){
	int s,e;
	if(!strcmp(p->cur_anim,animKey)){
		return 1;//assert(0);/*动作相同*/
	}
	if(f_animtor_get(p,animKey,&s,&e)){
		
		memset(p->cur_anim,0,G_BUFFER_64_SIZE);
		memcpy(p->cur_anim,animKey,strlen(animKey));
		p->cur_start = s;
		p->cur_end = e;
		return 1;
	}
	return 0;
}

void animtor_play_start_end(struct Animtor* p,const int s,int e)
{
	p->cur_start = s;
	p->cur_end = e;
	memset(p->cur_anim,0,G_BUFFER_64_SIZE);
}

int animtor_isPlaying(struct Animtor* p)
{
	return p->isPlaying;
}
void 
animtor_play(struct Animtor* p)
{
	p->isPlaying = 1;
}

void 
animtor_pause(struct Animtor* p)
{
	p->isPlaying = 0;
}
void 
animtor_calculateFrame(struct Animtor* p,int* const pCurIndex)
{
	if(*pCurIndex < p->cur_start)
	{
		*pCurIndex = p->cur_start;
	}	
	if(*pCurIndex >= p->cur_end){
		*pCurIndex=p->cur_start-1;
	}
	if(p->isPlaying)
	{
		(*pCurIndex)++;
	}
}