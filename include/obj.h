#ifndef _OBJ_H_
#define _OBJ_H_

#define _USE_SIGNED_SHORT_

#ifdef _USE_SIGNED_SHORT_
	typedef  unsigned short _UNSIGNED_SHORT;//：0~65535 //2字节
#else
	typedef  unsigned int _UNSIGNED_SHORT;	//0~65535	//4字节
#endif

//typedef  int _UNSIGNED_SHORT;
struct ExportOBJ_Data
{
	float *ptrVertex;
	float *ptrUV;
	float* ptrNormal;
	int vertexCount;		/*顶点索引数量*/

	//索引
	_UNSIGNED_SHORT *ptrIndex;
	int indexLength;

	int renderVertCount;//需要渲染的顶点个数

	
};
/*
	obj文件加载并开始解析 ,len:数组的长度

	int* handle:struct ObjParseVOType*=====>即为ObjParseVOType对象引用
	
	注:	int* outArray 是一个tl_malloc出来的内存块,处理完之后记得释放
	案例代码如下:正确的使用方式
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//加载一个四边形面片
	static void loadquad()
	{
		int vertexCount = 0;//顶点个数

		int _len=0;
		int verts = 0,obj = 0;
		char* _objStr;

		printf("内存池:%d\n",tl_memByteSize());

		_objStr=tl_loadfile("..\\resource\\obj\\quad.obj",NULL);
		
		obj_parse(_objStr,&_len,&verts,&obj);
		
		vertexCount = _len / 8;
		
		//释放字符引用
		tl_free(_objStr);

		//释放顶点数据
		tl_free(verts);

		printf("内存池:%d\n",tl_memByteSize());
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	char* 输入的字符串
	int*  outlen:	数据个数引用
	int*  outArray	数据引用
	int exportType	导出的类型
*/
void obj_parse(const char* data,int* outlen,int* ptrOutArray,int exportType);
/*
	将data字符串解析成一个struct OBJ_Data结构体
*/
struct ExportOBJ_Data* obj_getData(const char* data);
/*
	销毁
*/
void obj_delData(struct ExportOBJ_Data* ptr);

#endif