#ifndef _TWEEN_H_
#define _TWEEN_H_

/*
	渐变
*/
void* 
tween_to(void* obj,int time,
		 void (*endCallBack)(void*),
		 void (*updateCallBack)(void*),
		 int cnt,...);

/*
	是否在播放(1:在播放,0:没有播放)
*/
int 
tween_is_play(void* ptr);
/*
	停止tween
*/
void
tween_stop(void* ptr);
//将此方法放在渲染循环里
void
tween_run(long _longTime);
#endif