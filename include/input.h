#ifndef _INPUT_H_
#define _INPUT_H_
/*
 *��ʼ���������,����һ������Ϊwidth���������
 */
void* input_create(int width);
/*
 *����input���
 */
void input_dispose(void* p);
/*
 *��������
 **/
void intput_set_pos(void *p,int x,int y);

/*������ͼ��,δ���õ�ʱ��,������һ��Ĭ�ϵ���ͼ��Ϊ�����ı���*/
//void input_set_bg(void* p,const char* url);

/*��ȡ������еĵ�ǰ�ı�����,��ǰ���ַ��������洢�ڶ��ڴ���*/
char* input_get_heap_str(void* p);
#endif