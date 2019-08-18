#ifndef _INPUT_H_
#define _INPUT_H_
/*
 *初始化输入组件,设置一个宽度为width的输入组件
 */
void* input_create(int width);
/*
 *销毁input组件
 */
void input_dispose(void* p);
/*
 *设置坐标
 **/
void intput_set_pos(void *p,int x,int y);

/*设置其图集,未设置的时候,会设置一个默认的贴图作为输入框的背景*/
//void input_set_bg(void* p,const char* url);

/*获取输入框中的当前文本数据,当前的字符缓存区存储在堆内存上*/
char* input_get_heap_str(void* p);
#endif