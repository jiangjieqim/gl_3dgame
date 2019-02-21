#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "gettime.h"
#include "frame.h"
#include "str.h"
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
static int 
f_anim_chars_splice(char* str,void* parms){
	//char* str = p->str;
	struct Md2Info* _pInfo = (struct Md2Info*)parms;//p->parms;

	char* res = (char*)strstr(str,",");

	if(res!=0){
		char animName[G_BUFFER_32_SIZE];
		memset(animName,0,G_BUFFER_32_SIZE);
		memcpy(animName,str,(int)strlen(str)-(int)strlen(res));

		if(!strcmp((const char*)animName,(const char*)_pInfo->animName)){
			//找到动作名,对动作起始关键帧和结束关键帧进行赋值
			_pInfo->findState = 1;
			sscanf_s((const char*)res,",%d,%d",&(_pInfo->s),&(_pInfo->e));
		}
	}
	return 1;
}

//int base_isCanAnimation(struct HeadInfo* ptr){
//	return ptr->curType == TYPE_MD2_FILE ? 1 : 0;
//}

void frame_set(struct FrameAnim* frameAnim,const char* anim){
	//struct FrameAnim* frameAnim = _md2->base->frameAnim;
	struct Md2Info _info;
	struct Md2Info* _pInfo = &_info;

	memset(_pInfo,0,sizeof(struct Md2Info));
	_pInfo->animName = (char*)anim;

	str_split_cut(frameAnim->animConfig,'@',f_anim_chars_splice,(void*)_pInfo);
	if(_pInfo->findState){
		memset(frameAnim->curAnim,0,G_BUFFER_16_SIZE);
		memcpy(frameAnim->curAnim,anim,strlen(anim));

		frameAnim->frameStart = _pInfo->s;
		frameAnim->frameEnd = _pInfo->e;
		frameAnim->curFrame = frameAnim->frameStart;

		//printf("=======> 设置动作%s\n",anim);

	}else{
		printf("未找到动作片段配置 [%s]原动作配置:[%s]\n",anim,frameAnim->animConfig);
		assert(0);
	}
}
static 
void f_base_playOnce(struct FrameAnim* frameAnim){
	frameAnim->playOnce = NULL;
	frame_set(frameAnim,frameAnim->oldAnim);
	memset(frameAnim->oldAnim,0,G_BUFFER_16_SIZE);
}

void frame_play(struct FrameAnim* frameAnim,const char* anim){

	//struct FrameAnim* frameAnim = base->frameAnim;
	if(strcmp(frameAnim->curAnim,anim)==0){
		//printf("%s 动作相同不做处理\n",anim);
		return;
	}
	memset(frameAnim->oldAnim,0,G_BUFFER_16_SIZE);
	memcpy(frameAnim->oldAnim,frameAnim->curAnim,strlen(frameAnim->curAnim));
	frame_set(frameAnim,anim);
	frameAnim->playOnce = f_base_playOnce;
}
void frame_run(struct FrameAnim* frameAnim){
	long cur = 0;
	if(frameAnim->frameStart == frameAnim->frameEnd){
		printf("md2 未设置初始动作!\n");
		assert(0);
		return;
	}

	//fps自适应处理
	cur =  get_longTime();

	if(cur - frameAnim->_subTick >= frameAnim->fpsInterval){

		if(frameAnim->curFrame>=frameAnim->frameEnd){

			//printf("%s 一个循环播放结束\n",_md2->curAnim);
			if(frameAnim->playOnce)
				frameAnim->playOnce(frameAnim);

			frameAnim->curFrame = frameAnim->frameStart - 1;
		}

		frameAnim->curFrame++;
		//printf("当前进程已经运行 : %ld 毫秒 %ld\n",get_longTime(),(cur - _md2->_subTick));
		frameAnim->_subTick = cur;
	}
}