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
#endif