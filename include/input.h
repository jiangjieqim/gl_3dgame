#ifndef _INPUT_H_
#define _INPUT_H_
/*
 *input组件是基于ftext组件做的扩展,渲染节点就一个Sprite用于渲染,点击等交互.
 *如果要设置背景,可以基于input组件进行扩展,input组件本身不负责这种背景的渲染
 *input本身只渲染文本和处理键盘交互
 */

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
 */
void intput_set_pos(void *p,int x,int y);

/*设置其图集,未设置的时候,会设置一个默认的贴图作为输入框的背景*/
//void input_set_bg(void* p,const char* url);

/*获取输入框中的当前文本数据,当前的字符缓存区存储在堆内存上*/
char* input_get_heap_str(void* p);

//获取输入框的容器
void* input_get_container(void* p);
//获取ftext句柄
void* input_get_ftext(void* p);
//清空数据
void input_clear(void* p);
#endif