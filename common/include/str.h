#ifndef _STR_H_
#define _STR_H_
#include "def.h"
/*
创建一个字符串
*/
DLLIMPORT void* str_create(const char* s);

/*
销毁
*/
DLLIMPORT void str_dispose(void* _str);
/*
在字符串尾部添加新字符串
*/
DLLIMPORT void str_appand(void* _str,const char * s);
/*
在字符串尾部添加新字符串,指定截取字符串的长度为len
*/
DLLIMPORT void str_appand_s(void* _str,const char * s,int len);

/*
字符串长度
*/
DLLIMPORT int str_length(void* str);
/*
字符串替换
pSrc			-- 要被替换的子字符串, 比如 <br>
pDst			-- 要替换成的字符串, 比如   /n
注意:以上的字符串均要以'\0'结尾.
*/
DLLIMPORT void str_replace(void* str,char *pSrc, char *pDst);

/*
	将dest中的字符数据copy到target中
*/
DLLIMPORT void str_copy(void* target,struct Str* dest);
/*
	获取当前的字符串
*/
DLLIMPORT char* str_get(void* ptr);

/*删除str中的ch字符*/
DLLIMPORT 
void str_delchar(char *str,char ch);


/*
*	str.h
*	字符串处理接口
*/
/*查找字符串首次出现的位置，没有找到返回 -1，两个字符串相等返回 0*/
DLLIMPORT 
int str_pos(const char *haystack,const char *needle,int ignorecase);

 //int (*pCallBack)(struct StrCutSplit*)
/*
	用例:

	
	//	int* inputParms	输入的参数引用,inputParms可以作为输入字符串进行业务逻辑处理
	//	char* splitStr	被切割的字符串片段
	
	static void f_obj_animsSplit2(int* inputParms,char* splitStr){
		parseOneLine(inputParms,splitStr);
	}

	T t;
	tl_splitByStr((const char*)data,OBJ_SIGN_CHAR_BR,f_obj_animsSplit2,&t);

*/
DLLIMPORT 
void str_split(const char* dest,const char sign,void (*pCallBack)(int*,char*),int* parms);


/*
	字符串切割,比tl_split_s性能更好,临时字符串使用的栈空间中的数据
	使用方法:


	int animsSplitExample(char* point)
	{
		struct StrCutSplit* p = (struct StrCutSplit*)point;
		char* str = p->str;
		if(!strcmp(str,(char*)p->parms))
		{
			//找到字符串
			printf("找到字符串{%s}\n",str);
		}
		return 1;
	}
	tl_split_cut("run,walk,dead,",',',animsSplitExample,(int*)"walk");//寻找切割字符串中是否有"walk"字段

	
	void (*pCallBack)(char*,void*),
	char* 分割的子字符串
	void* 传递的参数引用
*/
DLLIMPORT 
void str_split_cut(
				  const char* dest,const char sign,
				  void (*pCallBack)(char*,void*),
				  void* parms);

#endif