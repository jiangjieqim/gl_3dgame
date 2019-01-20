#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"
#include "array.h"

//=================================================================================
struct Array* array_create(int length,int mode){
	struct Array* p = (struct Array*)tl_malloc(sizeof(struct Array));
	memset(p,0,sizeof(struct Array));
	p->mode = mode;
	p->ptr = (int*)tl_malloc(sizeof(int)*length);
	p->length = length;
	memset(p->ptr,0,sizeof(int)*length);
	return p;
}

void array_free(struct Array* p){
	int i = 0;
	for(i = 0;i < p->length;i++){
		int node = p->ptr[i];
		if(node && p->mode == ARRAY_HEAP){
			tl_free((void*)node);
			p->ptr[i] = 0;
		}
	}
	tl_free(p->ptr);
	p->ptr = NULL;
	tl_free(p);
	p = NULL;
}

void array_log(struct Array* p){
	int i = 0;
	printf("数组长度:%d\n",array_len(p));
	for(i = 0;i < p->length;i++){
		int node = p->ptr[i];
		printf("0x%0x array[%d]:%d\n",&(p->ptr[i]),i,node);
	}
}

int array_len(struct Array* ptr){
	return ptr->length;
}

void array_put(struct Array* ptr,int d){
	if(ptr->cur+1 > ptr->length){
		printf("数组越界!\n");
		assert(0);
	}
	ptr->ptr[ptr->cur] = d;
	ptr->cur++;
}

int array_get(struct Array*p,int index,int* outData){
	if(index+1>p->length){
		return 0;
	}
	*outData = p->ptr[index];
	return 1;
}

int array_set(struct Array* p,int index,int d){
	if(index+1>p->length){
		return 0;
	}
	p->ptr[index]=d;
	return 1;
}

static void f_arradd(struct Array* ptr,const char* source,
					 int s,int e){
						 if(s == e){
							 array_put(ptr,0);
							 //printf("()\n");
						 }else{
							 int n = e - s + 1;
							 char* _str = (char*)tl_malloc(n);
							 memset(_str,0,n);
							 memcpy(_str,source+s,e - s);
							 array_put(ptr,(int)_str);
							 //printf("(%s)\n",_str);
						 }
}

struct Array* array_split(const char* str,char key){

	//_splen用来确定静态数组的创建长度
	int _spLen = tl_split_getlen(str,key);

	//int length = tl_strlen(str);
	int offset = 1;//key占用的字符数
	struct Array* list = array_create(_spLen,ARRAY_HEAP);
	int _k = 0;
	int start = 0;
	int end;
	int index = tl_charIndex((char*)str,key);
	//	int index =	tl_strpos_s(str,key);
	while (index!=-1){

		end = index+start;

		f_arradd(list,str,start,end);

		start=end+offset;
		index = tl_charIndex((char*)(str + start),key);
		//		index = tl_strpos_s((char*)(str + start),key);
		_k++;
	}
	f_arradd(list,str,start,tl_strlen(str));
	return list;
}