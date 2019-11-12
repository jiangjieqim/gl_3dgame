#ifndef _TIME_LATER_H_
#define _TIME_LATER_H_

void* timelater_new(int ms,void (*callBack)(void*),void* param);
//ms毫秒后一个回调
//void  timelatar_push((*callBack)(void* timelater),void* param,int ms);

/*
 *销毁计时器
 */
void timelater_remove(void* timer);

/************************************************************************/
/* 运行计时器                                                                     */
/************************************************************************/
void timelater_run();

#endif