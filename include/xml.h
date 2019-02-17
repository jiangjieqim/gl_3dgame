#ifndef _XML_H_
#define _XML_H_

/*
*	Type:XMLSList
*	-------------
*	
*	XML 节点列表
	
	outlen		--输出的长度
	out			--临时缓存去 用于处理数据

	pNodeKey	--搜索的根节点
	pKey		--用来搜索的关键字
	pRes		--用来保存搜索到的值
	pNode		--用来保存XmlNode指针

*/
typedef struct XMLSList{
	struct LStackNode* list;
	char* data;
	char* buffer;
};
/*
这是一个单节点结构体

xml			--xml指针
parent		--根节点坐标
start		--起始坐标
end			--结束坐标
row			--行描述数据
*/
typedef struct XmlNode{
	//父结点引用
	struct XMLSList* parentXml;
	int parent;
	int start;
	int end;
};

/*
Type:XmlNode
-----------------

/*
	功能:解析xml字符串
	-------------
	str				--	xml文本
	outMaxLen		--	parms输出的长度,此长度为最大长度

	--------------------------------------
	example:

	struct XMLSList* xml = NULL;
	char* xmlstr =NULL;
	char out[G_BUFFER_MAX_SIZE];
	struct XmlNode* node=NULL;
	tl_memInit();


	xmlstr=tl_loadfile("d:\\1.xml",NULL);

	xml = xml_parse(xmlstr,G_BUFFER_MAX_SIZE);

	node =	xml_getrow(xml,"id","10");
	if(node!=NULL)
	{
	int num;
	xml_getstr(node,"id",out);
	sscanf(out,"%d",&num);
	printf("(%s,%d)\n",out,num);
	xml_getstr(node,"path",out);
	printf("(%s)\n",out);
	xml_getstr(node,"language",out);
	printf("(%s)\n",out);
	node =	xml_getrow(xml,"id","2");
	p->nameMaxSize = xml_getint(node,"value");

	node =	xml_getrow(xml,"id","3");
	rate = xml_getfloat(node,"value");
	}
	tl_free(xmlstr);
	getchar();
	--------------------------------------
*/
struct XMLSList* xml_parse(const char* str);

/*
Function:	xml_getRow
---------------
获取表内的一行

*/
struct XmlNode* 
xml_getrow(struct XMLSList* xml,const char* parms,const char* parmsValue);
/*
 *根据索引获取node引用
 **/
void* 
xml_getNodeByIndex(void* xml,int index);
/*
销毁xml句柄
*/
void xml_del(struct XMLSList* xml);

/*
获取参数值

buffer		--缓冲区
bufferSize	--缓冲区大小

return	--1：获取成功, 2:获取失败

*/
int xml_getstr(const struct XmlNode* node,const char* key,
			   char* buffer,int bufferSize);

/*
	获取int
*/
int xml_getint(const struct XmlNode* node,const char* key);

/*
	获取float
*/
float xml_getfloat(const struct XmlNode* node,const char* key);

/*
	返回一个tl_malloc的字符串引用,在堆上,用完要释放
*/
char* xml_getCopystr(const struct XmlNode* node,const char* key);
#endif