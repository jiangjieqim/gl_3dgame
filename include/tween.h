#ifndef _TWEEN_H_
#define _TWEEN_H_

/*
	渐变
*/
void* 
tween_to(int time,void (*callBack)(),int cnt,...);


//将此方法放在渲染循环里
void
tween_run(long _longTime);
#endif