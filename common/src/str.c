#include <memory.h>
#include <string.h>
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