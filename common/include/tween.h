#ifndef _TWEEN_H_
#define _TWEEN_H_
#include "def.h"
/*
	渐变
*/
DLLIMPORT void* 
tween_to(void* obj,int time,
		 void (*endCallBack)(void*),
		 void (*updateCallBack)(void*),
		 int cnt,...);

/*
	是否在播放(1:在播放,0:没有播放)
*/
DLLIMPORT int 
tween_is_play(void* ptr);
/*
	停止tween
*/
DLLIMPORT void
tween_stop(void* ptr);
//将此方法放在渲染循环里
DLLIMPORT void
tween_run(long _longTime,long delayTime);
#endif