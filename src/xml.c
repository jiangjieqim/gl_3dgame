#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "tools.h"
#include "tl_malloc.h"
#include "xml.h"

struct FindXmlNode{
	char* key;
	char* value;
	struct XmlNode* result;
};

//#define _XML_DEBUG_			//是否是测试,该宏可以输出debug日志

#define NUM_STR_LENGTH G_BUFFER_64_SIZE	//数字文本的长度

/*
	获取临时的行数据
*/
static char* getNodeRow(struct XmlNode* node){

	//if(node->row==NULL){
	//	InitNode(node->parentXml->data,node);
	//	return node->row;
	//}

	char* rowChars = 0;
	char* _allStr = (char*)node->parentXml->data;
	
	int len = 0;
	int pos;
	int start,end;
	int halfLen = (int)strlen(G_SIGN_HALFWAY_LINE);
	char* ch = 0;

	if(!tl_strlen(_allStr)){
		printf("获取临时的行数据,主串length = 0\n");
		assert(0);
	}

	start=node->start;
	end=node->end;
	len =end - start + 1;

	//if(ROW_LENGTH < len+1){
	//	printf("行数据栈空间太小");
	//	assert(0);
	//}
	//char rowChars[ROW_LENGTH];
	rowChars = (char*)tl_malloc(len+1);
	memset(rowChars,0,len+1);

	//生成row字符串
	//node->row = (char*)tl_malloc(len+1);

	ch = (char*)tl_malloc(len+1);
	memset(ch,0,len+1);

	memset(rowChars,0,len+1);
	tl_strsub(_allStr,rowChars,start,end-2);

	tl_replace(rowChars,ch,G_SIGN_SPACE,G_SIGN_HALFWAY_LINE);
	pos = tl_strpos_s(ch,G_SIGN_HALFWAY_LINE);
	node->parent = pos;

	memset(rowChars,0,strlen(rowChars));
	memcpy(rowChars,ch+pos+halfLen,strlen(ch)-pos-halfLen);

	memset(ch,0,strlen(ch));
	tl_replace(rowChars,ch,G_SIGN_QUOTATION,G_SIGN_NULL);

	memset(rowChars,0,strlen(rowChars));
	tl_replace(ch,rowChars,G_SIGN_EQUAL,G_SIGN_COMMA);
	tl_free(ch);
	return rowChars;
}


/*	
	增加一个节点
	statr		--起始位置
	end			--结束位置
*/
static void f_xml_AddNode(struct XMLSList* sl,int start,int end){
	struct XmlNode* node;
	void* list=sl->list;
	node = (struct XmlNode*)tl_malloc(sizeof(struct XmlNode));
	memset(node,0,sizeof(struct XmlNode));
	node->parentXml = sl;
	node->start = start;
	node->end = end;
	//InitNode(sl->data,node);
	LStack_push((struct LStackNode*)list,(int)node);
}

/*xml 解析*/
struct XMLSList* xml_parse(const char* str)
{
	struct XMLSList* xml;
	int i, dataByteLength,_nodeLen,bufferSize;
	bufferSize = NUM_STR_LENGTH;

	//数据字节长度
	dataByteLength = (int)strlen(str);
	
	xml = (struct XMLSList*)tl_malloc(sizeof(struct XMLSList));

	xml->buffer = (char*)tl_malloc(bufferSize);
	//printf("初始化xml 活动缓冲区 (%d) bytes\n%s\n",bufferSize,str);

	//直接取引用
	xml->data = (char*)str;

	xml->list = LStack_create();
	//拆分节点
	for(i = 0;i <dataByteLength;i++){
		char c = str[i];
		//printf("%c,",c);
		if(c == G_SIGN_XML_LEFT){
			//'<' 一个节点开始
			_nodeLen = 0;
		}else if(c == G_SIGN_XML_RIGHT){
			//'>'   一个节点结束
			int st = i - _nodeLen;
			//将一条xml数据添加到xml->list
			f_xml_AddNode(xml,st,i);			
		}
		else
		{
			_nodeLen++;
		}
	}
	return xml;
}

static void delXmlNode(struct XmlNode* _node){
	_node->parentXml = 0;
	_node->start = 0;
	_node->end = 0;
	_node->parent = 0;

	/*if(_node->row!=NULL){
		tl_free(_node->row);
		_node->row = NULL;
	}*/
	tl_free(_node);
	_node = 0;
}

static int freeXmlDelNode(int data,int parm){
	delXmlNode((struct XmlNode*)data);
	return 1;
}
void xml_del(struct XMLSList* xml){
	tl_free(xml->buffer);
	LStack_ergodic(xml->list,freeXmlDelNode,0);
	LStack_delete((struct LStackNode*)xml->list);
	xml->list = 0;
	if(xml->data!=0){
		tl_free(xml->data);
		xml->data = 0;
	}
	tl_free(xml);
	xml = 0;

}

static int findXmlNode(int data,int _inputParam){

	struct FindXmlNode* fNode = (struct FindXmlNode*)_inputParam;
	struct XmlNode* _node = (struct XmlNode*)data;
	struct XMLSList* _xml = _node->parentXml;
	if(fNode->result==0){
		char* row =	getNodeRow(_node);//_node->row;
		int pos=-1;

		if(row){
			
			int len =	strlen(fNode->key)+strlen(fNode->value)+strlen(G_SIGN_COMMA)+1;//(int)strlen(row);
			
			char* _str = (char*)tl_malloc(len);
			
			memset(_str,0,len);
			
			sprintf_s(_str,len,"%s%s%s",fNode->key,G_SIGN_COMMA,fNode->value);
			//printf(":::%d _str=:%s\n",len,_str);

			pos = tl_strpos_s((const char*)row,(const char*)_str);
			/*_position=strstr((const char*)row,(const char*)_str);
			if(!_position)
				pos = -1;
			else
				pos = row - _position;*/
			
			tl_free(_str);
		}

		if(row!=0){
			tl_free(row);
			row = 0;
		}

		if(pos>=0){
			fNode->result = _node;
			return 0;
		}
	}
	return 1;
}

struct XmlNode* xml_getrow(struct XMLSList* xml,const char* tParms,const char* tParmsValue)
{
	struct FindXmlNode _mynode;
	memset(&_mynode,0,sizeof(struct FindXmlNode));
	_mynode.key = (char*)tParms;
	_mynode.value = (char*)tParmsValue;
	LStack_ergodic(xml->list,findXmlNode,(int)&_mynode);
	if(!_mynode.result){
		printf("未找到关键字:%s,value:%s\n",tParms,tParmsValue);
	}
	return _mynode.result;
}
/*
	将获取的数据设置到xml->out中
*/
static void 
fillData(const struct XmlNode* node,const char* key){
	const struct XMLSList* xml = node->parentXml;
	xml_getstr(node,key,xml->buffer,NUM_STR_LENGTH);
}

int xml_getint(const struct XmlNode* node,const char* key){
	int res;
	fillData(node,key);
	sscanf_s(node->parentXml->buffer,"%d",&res);
	return res;
}

float 
xml_getfloat(const struct XmlNode* node,const char* key)
{
	float res;
	fillData(node,key);
	sscanf_s(node->parentXml->buffer,"%f",&res);
	//sscanf(node->parentXml->buffer,"%f",&res);
	return res;
}
char* xml_getCopystr(const struct XmlNode* node,const char* key){
	char* str = getNodeRow((struct XmlNode*)node);//node->row;
	int pos = tl_strpos_s(str,key);
	char* buffer = 0;
	if(pos>=0)
	{
		int len = (int)strlen(str);
		char* chs = (char*)tl_malloc(len+1);
		int _pos,h,cpos;
		memset(chs,0,len+1);
		memcpy(chs,str+pos,len-pos);
		_pos = tl_strpos_s(chs,G_SIGN_HALFWAY_LINE);
		if(_pos!=-1)
		{
			memcpy(chs,chs,_pos);
			memset(chs+_pos,0,strlen(chs)-_pos);
		}
		h = (int)strlen(G_SIGN_COMMA);
		cpos = tl_strpos_s(chs,G_SIGN_COMMA);
		if(cpos!=-1)
		{
			int _len = (int)strlen(chs)-cpos-h+1;
			buffer = (char*)tl_malloc(_len);
			memset(buffer,0,_len);
			memcpy(buffer,chs+cpos+h,_len-1);
		}
		else
		{
			printf("did find G_SIGN_COMMA!\n");
		}
		tl_free(chs);
	}
	tl_free(str);
	str = 0;
	return buffer;
}

static int 
getKeyPos(const char* str,const char* key){

	//char*	buffer,int bufferSzie
	int bufferSzie = G_BUFFER_64_SIZE;
	char buffer[G_BUFFER_64_SIZE];

	if(strlen(key)+strlen(G_SIGN_COMMA)>(size_t)bufferSzie)
	{
		printf("关键字(%s)溢出\n",key);
		assert(0);
	}
	
	memset(buffer,0,bufferSzie);
	sprintf_s(buffer,bufferSzie,"%s%s",key,G_SIGN_COMMA);

	return tl_strpos_s(str,buffer);
}


int 
xml_getstr(const struct XmlNode* node,const char* key,char* buffer,int bufferSize){

	int stat = 0;

	//printf("%d\n",tl_memByteSize());
	char* str =	getNodeRow((struct XmlNode*)node);	//node->row;

	int pos = getKeyPos(str,key);//tl_strpos_s(str,key);//bug	---x,

	//getKeyPos(key);
	memset(buffer,0,bufferSize);//清空缓存

	if(pos>=0){
		int len = (int)strlen(str);

		char* chs = (char*)tl_malloc(len+1);
		int _pos,h,cpos;
		memset(chs,0,len+1);
		memcpy(chs,str+pos,len-pos);
		_pos = tl_strpos_s(chs,G_SIGN_HALFWAY_LINE);
		if(_pos!=-1)
		{
			memcpy(chs,chs,_pos);
			memset(chs+_pos,0,strlen(chs)-_pos);
		}
		h = (unsigned int)strlen(G_SIGN_COMMA);
		cpos = tl_strpos_s(chs,G_SIGN_COMMA);
		if(cpos!=-1)
		{
			memcpy(buffer,chs+cpos+h,strlen(chs)-cpos-h);
		}
		else
		{
			printf("did find G_SIGN_COMMA!\n");
		}
		tl_free(chs);

		stat=1;
	}
	tl_free(str);
	str = 0;
	//printf("%d\n",tl_memByteSize());
	return stat;
}