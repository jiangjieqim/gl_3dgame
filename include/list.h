#ifndef _LIST_H_
#define _LIST_H_
/**
*	列表结构体
*/
struct List
{
	float* list;
	int length;
};
//****************************************************************************//
/*	案例
int main(int argc, char* argv[])
{
	struct List* a = (struct List*)tl_malloc(sizeof(struct List));
	list_create(a);
	
	list_add(a,1.2);
	list_add(a,0.5);
	
	printf("列表长度 %d\n",a->length);
	
	for(int i = 0;i < a->length;i++)
	{
		printf("%f\n",a->list[i]);
	}
	list_dispose(a);
	tl_free(a);
	return 0;
}
*/

/*
*	初始化一个列表,起始长度为0
*/
void list_create(struct List* _l);
/*
*	为列表添加一个float数据,索引从0位开始增加
*/
void list_add(struct List* _l,float value);
/*
*	销毁列表	
*/
void list_dispose(struct List* _l);
#endif
//=======================================================================