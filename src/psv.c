#include <memory.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tools.h"
#include "psv.h"
#include "gettime.h"

struct PCsvNodeVO
{
	/*
	行数据start
	*/
	int s;

	/*
	行数据end
	*/
	int e;

	/*
		主键值引用,不用释放
	*/
	char* keyValue;

	/*
	切割数组
	*/
	struct Array* list;
};
/*
根据node创建一个堆字符串
*/
static char* createStr(const char* str,int _s, int _e){
	int len = _e - _s+1;
	char* s = (char*)tl_malloc(len);
	memset(s,0,len);
	memcpy(s,str+_s,len-1);
	return s;
}

static void mcpyStr(const char* str,int _s, int _e,char* s){
	int len = _e - _s+1;
	//char* s = (char*)tl_malloc(len);
	memset(s,0,G_BUFFER_512_SIZE);
	memcpy(s,str+_s,len-1);
}

/*
输出日志
*/
static void showstr(const char* str,int s,int e){
	int len = tl_strlen(str)+1;
	char* _newStr = (char*)tl_malloc(len);
	int n = e - s;
	memset(_newStr,0,len);
	if(n > len){
		printf("n=%d,len=%d\n",n,len);
		assert(0);
	}
	memcpy(_newStr,str+s,e-s);

	//printf("s : %d - e : %d",s,e);
	printf("{s:%d-e:%d[%s]}\n",s,e,_newStr);
	if(_newStr!=0){
		tl_free(_newStr);
		_newStr = 0;
	}
}
//只要是定义了的static 的function ,那么只能在本文件域内使用 外部文件无法使用，即使在.h中定义了,也无法使用!
//static void setNodeStr(const char* str,int s,int e,char* out,int outlen){
//	if(tl_strlen(str)> outlen){
//		assert(0);
//	}
//	memset(out,0,outlen);
//	memcpy(out,str+s,e-s);
//}

/*
	获取数组中匹配的索引
*/
static int getIndex(struct Array* list,const char* mainkey){
	int len = array_len(list);
	int i;
	for(i = 0;i < len;i++){
		int d;
		array_get(list,i,&d);
		if(d){
			if(!strcmp((char*)d,mainkey)){
				return i;
			}
		}
	}
	return -1;
}
struct PCsvVO* pcsv_load(const char* path,const char* mainkey,int checkIndex){
	long time = get_longTime();
	//printf("%s\n",str);
	struct PCsvVO* pcsv = (struct PCsvVO*)tl_malloc(sizeof(struct PCsvVO));
	char* str = tl_loadfile(path,0);
	
	int len = tl_strlen(str);
	int offset = 1;
	
	int _k = 0;
	int start = 0;
	
	int keyIndex=-1;

	int index = tl_charIndex(str,'\n');
	memset(pcsv,0,sizeof(struct PCsvVO));

	//pcsv->str = str;
	//创建一个链表
	pcsv->list = LStack_create();
	
	
	while (index!=-1){
		
		int end = index+start;
	
		if(_k>=checkIndex){

			struct PCsvNodeVO* _csvVO = (struct PCsvNodeVO*)tl_malloc(sizeof(struct PCsvNodeVO));
			memset(_csvVO,0,sizeof(struct PCsvNodeVO));

			

			_csvVO->s = start;
			_csvVO->e = end-offset;//去掉一个回车字符长度
			_csvVO->list = 0;

			if(start<end-offset){
				
				char* tStr=createStr((const char*)str,start,end-offset);
				//char tStr[G_BUFFER_512_SIZE];
				//mcpyStr((const char*)str,start,end-offset,tStr);

				//创建数组
				
				_csvVO->list=array_split((const char*)tStr,',');//error

				tl_free(tStr);
				tStr = 0;

				if(_k==checkIndex){
					//key row
					keyIndex=getIndex(_csvVO->list,mainkey);
					pcsv->keylist = _csvVO->list;
				}
				else
				{
					if(keyIndex!=-1){
						int d;
						if(array_get(_csvVO->list,keyIndex,&d)){
							//设置keyVaule
							_csvVO->keyValue = (char*)d;
						}
					}
				}
			}

			LStack_push(pcsv->list,(int)_csvVO);
		}
		
		start=end+offset;
		index = tl_charIndex(str + start,'\n');
		_k++;
	}
	pcsv->keyIndex = keyIndex;
	/*int keyIndex = getIndexByKey(pcsv,mainkey);
	printf("keyIndex:%d",keyIndex);*/

	if(keyIndex==-1){
		printf("未找到主key:%s\n",mainkey);
	}

	if(str){
		tl_free(str);
		str = 0;
	}

	//printf("getIndexByKey:%d\n",getIndexByKey(pcsv,"type"));
	//showList(pcsv);

	printf("解析配置(%s)耗时%.3f毫秒\n",path,get_longTime()-time);

	return pcsv;
}
static void csvVoDel(struct PCsvNodeVO* vo){
	if(vo->list){
		array_free(vo->list);
		vo->list = 0;
	}
	tl_free((void*)vo);
}

void pcsv_dispose(struct PCsvVO* ptr){
	//释放配置列表数据
	struct LStackNode* s = (struct LStackNode* )ptr->list;

	int length = LStack_length(ptr->list);

	void* top,*p;
	top = s;
	p=top;

	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);
		csvVoDel((struct PCsvNodeVO*)data);
	}
	LStack_delete(ptr->list);
	//===============================================================
	
	tl_free(ptr);
}

struct Array* pcsv_getValue(struct PCsvVO* ptr,
	const char* compKeyValue)
{	
	struct LStackNode* s;
	void* top,*p;
	if(ptr->keyIndex==-1){
		return 0;
	}
	s = (struct LStackNode* )ptr->list;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		struct PCsvNodeVO* _csvVO;
		int d;

		p=(void*)LStack_next(p);
		data = LStack_data(p);

		//====================================================
		_csvVO = (struct PCsvNodeVO*)data;
		
		if(_csvVO->list && array_get(_csvVO->list,ptr->keyIndex,&d)){
			if(d && !strcmp((char*)d,compKeyValue)){
				return _csvVO->list;
			}
		}

	}
	return 0;
}