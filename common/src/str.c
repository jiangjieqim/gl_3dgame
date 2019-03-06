#include <memory.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define BUILDING_DLL

#include "tools.h"
#include "tl_malloc.h"
#include "str.h"

typedef struct Str{
	char* s;
}Str;

//=================================================================================
void* str_create( const char* s ){
	struct Str* _str = tl_malloc(sizeof(struct Str));
	int len;
	memset(_str,0,sizeof(struct Str));
	len = tl_strlen(s)+1;
	_str->s = tl_malloc(len);
	memset(_str->s,0,len);
	memcpy(_str->s,s,len-1);
	return (void*)_str;
}
char* 
str_get(void* ptr){
	Str* str = (Str*)ptr;
	return str->s;
}
void str_dispose(void* ptr){
	struct Str* _str=(struct Str*)ptr;
	if(_str->s){
		tl_free(_str->s);
		_str->s = 0;
	}
	tl_free(_str);
	_str = 0;
}

void str_appand( void* ptr,const char * s ){
	struct Str* _str=(struct Str*)ptr;
	char* _tmps = _str->s;
	int len,ts;
	ts = tl_strlen(_tmps);
	len = tl_strlen(s) + ts;
	_str->s = tl_malloc(len+1);
	memset(_str->s,0,len+1);
	memcpy(_str->s,_tmps,ts);
	memcpy(_str->s+ts,s,tl_strlen(s));
	tl_free(_tmps);
	_tmps = 0;
}

int str_length(void* ptr){
	struct Str* str=(struct Str*)ptr;
	return tl_strlen(str->s);
}
void str_appand_s(void* ptr,const char * s,int length )
{
	struct Str* _str=(struct Str*)ptr;
	char* _tmps = _str->s;
	int len,ts;
	ts = tl_strlen(_tmps);
	len = length + ts;

	_str->s = tl_malloc(len+1);
	memset(_str->s,0,len+1);
	memcpy(_str->s,_tmps,ts);

	memcpy(_str->s+ts,s,length);
	tl_free(_tmps);
	_tmps = 0;
}
void str_replace(void* ptr, char *oldstr, char *pDst){

	struct Str* pstr=(struct Str*)ptr;
	Str* _ss;
	char* ts;
	int i,_l,mode=0;

	_l = tl_strlen(oldstr);//需要替换的字符串结点长度

	_ss = str_create("");

	ts = pstr->s;
	i=(int)strcspn(ts,oldstr);

	if(i){
		mode = 1;
	}

	while(i<tl_strlen(ts)){
		char* _new;
		int n;

		if(mode){
			str_appand_s(_ss,ts,i);
			ts += i;
			str_appand(_ss,pDst);
			mode = 0;
		}
		else{
			str_appand(_ss,pDst);
		}

		_new = ts +_l;
		n=(int)strcspn(_new,oldstr);
		str_appand_s(_ss,_new,n);
		ts = _new+n;
		i = n;

		if(!tl_strlen(ts)){
			break;
		}
	}
	tl_free(pstr->s);
	pstr->s = _ss->s;
	tl_free(_ss);
}

void str_copy(void* ptr,struct Str* dest){
	struct Str* target=(struct Str*)ptr;
	int len = str_length(dest)+1;
	char* s = tl_malloc(len);
	memset(s,0,len);
	memcpy(s,dest->s,str_length(dest));
	tl_free(target->s);
	target->s = s;
}

void str_delchar(char *str,char ch){
	//char ts[G_BUFFER_256_SIZE];
	int len,i,j=0;
	char* ts = NULL;
	len = (int)strlen(str);
	ts = (char*)tl_malloc(len+1);
	memset(ts,0,len+1);
	for(i = 0;i< len;i++){
		if(str[i]!=ch){
			ts[j] = str[i];
			j++;
		}
	}
	memset(str,0,strlen(str));
	memcpy(str,ts,strlen(ts));
	tl_free(ts);
}

int str_pos(const char *haystack,const char *needle,int ignorecase)  {  
	register unsigned char c, needc;
	unsigned char const *from, *end;
	const char *findreset;
	int i;
	int len = (int)strlen(haystack);
	int needlen = (int)strlen(needle);
	from = (unsigned char *)haystack;
	end = (unsigned char *)haystack + len;
	findreset = needle;

	for ( i = 0; from < end; ++i) {
		c = *from++;
		needc = *needle;
		if (ignorecase) {
			if (c >= 65 && c < 97)
				c += 32;
			if (needc >= 65 && needc < 97)
				needc += 32;
		}
		if(c == needc) {
			++needle;
			if(*needle == '\0') {
				if (len == needlen) 
					return 0;
				else
					return i - needlen+1;
			}
		}  
		else {  
			if(*needle == '\0' && needlen > 0)
				return i - needlen +1;
			needle = findreset;  
		}
	}  
	return  -1;  
}
static void 
f_splitByStrNode(char* str,const char* dest,int signStrLength,int start,int i,void (*pCallBack)(int*,char*),int* parms){
	tl_strsub(dest,str,start,i-signStrLength);
	pCallBack(parms,str);
}

#define _SIGN_LEN_ 1//标示的长度
void str_split(const char* dest,const char sign,void (*pCallBack)(int*,char*),int* parms)
{
	int i,start=0;
	char _ch;
	int len = (int)strlen(dest);
	int length = len+1;

	int signLen = _SIGN_LEN_;

	char* str = (char*)MALLOC(length);
	for(i = 0;i < len;i++){
		_ch = dest[i];
		if(_ch==sign){
			memset(str,0,length);
			f_splitByStrNode(str,dest,signLen,start,i,pCallBack,parms);
			start = i+signLen;
		}
	}
	memset(str,0,len+1);
	f_splitByStrNode(str,dest,signLen,start,i,pCallBack,parms);
	FREE(str);
}

static void 
f_split_string(char* str,int strLength,const char* dest,int* pStart,int i
			 //int (*_callBack)(struct StrCutSplit*),
			 //void (*pCallBack)(char*,void*),
			 //struct StrCutSplit* pCut
			 )
{
	memset(str,0,strLength);
	tl_strsub(dest,str,*pStart,i-_SIGN_LEN_);
	//pCut->str = str;
	//_callBack(pCut);
	*pStart = i+_SIGN_LEN_;
}
//#define _TEMP_SIZE_ G_BUFFER_512_SIZE
void str_split_cut(const char* dest,const char sign,void (*pCallBack)(char*,void*),void* parms){

	if(strlen(dest)+1>=G_BUFFER_512_SIZE){
		printf("检测dest长度超过函数处理长度 当前dest的长度为%d\n",(int)strlen(dest));
		assert(0);
	}else{
		int i,start=0;
		char _ch;
		int len = (int)strlen(dest);
		int strLength = len + 1;

		//将堆内存申请修改为栈内存申请,使速度更快
		char str[G_BUFFER_512_SIZE];

		//struct StrCutSplit _sc;

		//memset(&_sc,0,sizeof(struct StrCutSplit));

		//未参数引用赋值
		//_sc.parms = parms;
		for(i = 0;i < len;i++){
			_ch = dest[i];
			if(_ch==sign){
				f_split_string(str,G_BUFFER_512_SIZE,dest,&start,i);
				pCallBack(str,parms);
			}
		}
		f_split_string(str,G_BUFFER_512_SIZE,dest,&start,i);
		pCallBack(str,parms);
	}
}

//汉字第一个字节的范围
static int
f_is_first_code(unsigned char c){
	return (0x80 <= c) && (0xF7 >= c);
}
//汉字第二个字节的范围
static int
f_is_second_code(unsigned char c){
	return (0xA1 <= c) && (0xFE > c);
}

void 
str_parse_eg_cn(const char* str,void* inParam,
				void(pCallBack)(void*,char*))
{

	int i,len;

#ifdef DEBUG
		//printf("s = [%s]占用的字节大小%d字节\n",str,strlen(str));
#endif
	len = strlen(str);
	for(i = 0;i <len;i++){
		unsigned char c = str[i];
		char t[3];
		memset(t,0,3);
		t[0] = c;

		if(f_is_first_code(c)){
			if(i+1<len && f_is_second_code(str[i+1])){
				//验证到当前字节和后续的一个字节组成的是一个汉字
				
				
				/*t[0]=str[i];
				t[1]=str[i+1];*/
				memcpy(t,str+i,2);

//				printf("当前的汉字=[%s]\n",t);

				i++;
			}
		}
		#ifdef DEBUG
			printf("[%s]\n",t);
		#endif

		pCallBack(inParam,t);
	}
}