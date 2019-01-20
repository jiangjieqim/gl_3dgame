#ifndef _ARR_
#define _ARR_

/*
	数组创建
*/
struct Array* array_create(int length,int mode);

/*
	释放数组
*/
void array_free(struct Array* ptr);
/*
	存储数据局
*/
void array_put(struct Array* ptr,int d);
/*
	数组长度
*/
int array_len(struct Array* ptr);
/*
	获取数据找到数据返回1,没有数据就0
*/
int array_get(struct Array* ptr,int index,int* outData);
/*
	设置数据
*/
int array_set(struct Array* p,int index,int data);
/*
	打印数组数据
*/
void array_log(struct Array* p);

/*
	根据key切割成多个字符文件,保存在数组中,字符串切割成字符串数组
*/
struct Array* array_split(const char* str,char key);

#endif