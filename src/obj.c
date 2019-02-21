#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tools.h"
#include "gettime.h"
#include "tl_malloc.h"
#include "obj.h"
#include "tlgl.h"
//=================================================================================


static const int G_GLOBAL_PRINT_OBJ_DEBUG = 0;	//是否打印时间消耗日志

/*obj模型数据结构*/
struct ObjParseVOType{
	float* uvArray;		/*uv数组*/
	int uvIndex;		/*uv索引*/
	float* vertexArray;	/*顶点数组*/
	int vertexIndex;	/*顶点索引*/
	float* normalArray;	/*法线数组*/
	int normalIndex;	/*法线索引*/
	float* outArray;	/*输出数组对象*/
	int outArrayIndex;	/*当前输出索引对象*/
	int outLen;			/*float数组个数*/
	int everyFloatCount;//每个顶点需要的float数量
	//=====================================
	int vertexCount;	/*顶点索引数量*/
	int vertexNormalCount;	/*顶点法线索引数量*/
	int uvCount;		/*uv索引数量*/
	int faceCount;		/*面索引数量*/
	int groupCount;		/*组索引个数*/

	int exportType;		//导出类型

	//索引方式的数据导出结构
	struct ExportOBJ_Data* ptrIndexObjData;

	/*uv数据索引*/
	struct LStackNode* _uvList;
	
	/*normal数据索引*/
	struct LStackNode* _normalList;
};

/*转化数据*/
struct ConvertData
{
	int index;//新索引号
	int oldIndex;//原索引
};
struct CopyData
{
	float* o;//老数据引用
	float* n;//新数据引用
};
struct Vertex
{
	float u,v,normalX,normalY,normalZ,vx,vy,vz;
};

static const char* OBJ_OBJ_SIGN_SPACE =	" "		;	//空格
static const char* OBJ_SIGN_BR	 =  	"\n"	; 	//换行
static const char OBJ_SIGN_CHAR_BR	=	'\n'	;
static const char* OBJ_SIGN_COMMAND =	"# "	;	//注释符
static const char* OBJ_SIGN_G		=	"g "	;	//组标示
static const char* OBJ_OBJ_SIGN_VN	=	"vn"	;	//法线标示
static const char* OBJ_SIGN_V		=	"v "	;	//顶点标示
static const char* OBJ_SIGN_VT		=	"vt"	;	//uv标示
static const char* OBJ_SIGN_F		=	"f ";		//面标示
static const char* OBJ_SIGN_S		=	"s "	;	//光滑组标示
static const char* OBJ_OBJ_SIGN_SIDELI	= "/";		//斜杠



///*
//v  -0.049 -0.717 -0.432
//字符串转化为float x y z
//*/
/*
static void 
strConvertFloatArray(const char *_sourceString,float* array,int startIndex,const char* seq){
	//float x,y,z;

#define STR_SIZE 128
	char query[STR_SIZE];
	char *q,*p;
	int i=0;
	memset(query,0,STR_SIZE);
	#undef STRSIZE
	memcpy(query,_sourceString,strlen(_sourceString));
	q = query;
	while(NULL!= (p=tl_strsep(&q,(char*)seq))) {
		float my;
		sscanf_s(p,"%f",&my);
		if(i>0){

			int __index = startIndex+i-1;
			array[__index] = my;
		}
		i++;
	}
}
*/

//static int g_normal = 1;
/*
	解析顶点
	str="1/2/3"
*/
static void 
ParseVertex(struct ObjParseVOType* obj,const char* str,struct Vertex* pVertex){
	//float uv_scale = 100.0f;

	int outIndex=0,gap=0,tIndex=0;
	int i0,i1,i2;
	sscanf_s(str,"%d/%d/%d",&i0,&i1,&i2);

	tIndex = i0 - 1;
	gap = VERTEX_GAP;
	pVertex->vx = obj->vertexArray[tIndex*gap];
	pVertex->vy = obj->vertexArray[tIndex*gap+1];
	pVertex->vz = obj->vertexArray[tIndex*gap+2];

	tIndex = i1 - 1;
	gap = UV_GAP;
	pVertex->u  = obj->uvArray[tIndex*gap];		//* uv_scale;
	pVertex->v  = obj->uvArray[tIndex*gap+1];	//* uv_scale;

	tIndex = i2 - 1;
	gap = NORMAL_GAP;
	pVertex->normalX = obj->normalArray[tIndex*gap];//* g_normal;
	pVertex->normalY = obj->normalArray[tIndex*gap+1];//* g_normal;
	pVertex->normalZ = obj->normalArray[tIndex*gap+2];//* g_normal;
}

/*
	添加一个索引数据
*/
static void 
AddIndex(struct ExportOBJ_Data* p,int vertexIndex)
{
	int __size = sizeof(_UNSIGNED_SHORT);
	if(NULL==p->ptrIndex)
		p->ptrIndex = tl_malloc(__size);
	else{
		void* d = tl_malloc((p->indexLength + 1)*__size);
		memcpy(d,p->ptrIndex,__size*p->indexLength);
		tl_free(p->ptrIndex);
		p->ptrIndex = d;
	}

	p->ptrIndex[p->indexLength]=vertexIndex;
	p->indexLength++;
}

static void 
ParseObjData(struct ObjParseVOType* obj,struct ExportOBJ_Data* ptrData,const char*str)
{
	int i0,i1,i2;
	int vertexIndex;
	sscanf_s(str,"%d/%d/%d",&i0,&i1,&i2);

	vertexIndex = i0 - 1;
	//顶点索引
	AddIndex(ptrData,vertexIndex);

	//组织UV数据
	{
		struct ConvertData* uv = tl_malloc(sizeof(struct ConvertData));
		uv->index = vertexIndex;
		uv->oldIndex = i1 - 1;
		LStack_push(obj->_uvList,(int)uv);
	}
	//组织normal数据
	{
		struct ConvertData* normal = tl_malloc(sizeof(struct ConvertData));
		normal->index = vertexIndex;
		normal->oldIndex = i2 - 1;
		LStack_push(obj->_normalList,(int)normal);
	}
}

/*
设置单个对象合并数据
 u v		vx    vy    vz       x    y    z
str="1/2/3"
convert		vt vn v --->v vt vn
*/
static void 
MergeVertex(struct ObjParseVOType* obj,const char* str){

	if(obj->ptrIndexObjData)
	{
		ParseObjData(obj,obj->ptrIndexObjData,str);
	}
	else
	{
		/*缩放值*/
		float scale = 1.0f;
		int outIndex=0;
		struct Vertex vertex;

		ParseVertex(obj,str,&vertex);
		
		outIndex = obj->outArrayIndex;
		
		//合并数据
		if(obj->exportType == OBJ_UV_VERTEX_NORMAL)
		{
			obj->outArray[outIndex]   = vertex.u;
			obj->outArray[outIndex+1] = vertex.v;
			obj->outArray[outIndex+2] = vertex.normalX;
			obj->outArray[outIndex+3] = vertex.normalY;
			obj->outArray[outIndex+4] = vertex.normalZ;
			obj->outArray[outIndex+5] = vertex.vx * scale;
			obj->outArray[outIndex+6] = vertex.vy * scale;
			obj->outArray[outIndex+7] = vertex.vz * scale;
		}
		else if(obj->exportType == OBJ_UV_VERTEX)
		{
			obj->outArray[outIndex]   = vertex.u;
			obj->outArray[outIndex+1] = vertex.v;

			obj->outArray[outIndex+2] = vertex.vx * scale;
			obj->outArray[outIndex+3] = vertex.vy * scale;
			obj->outArray[outIndex+4] = vertex.vz * scale;
		}
		obj->outArrayIndex += obj->everyFloatCount;
	}
}

/*
合并数据
f 1/1/1 2/2/2 4/4/3
*/
static void 
convertArray(struct ObjParseVOType* obj,const char* str){

	char v0[G_BUFFER_64_SIZE];//1/1/1 一个顶点
	char v1[G_BUFFER_64_SIZE];//2/2/2
	char v2[G_BUFFER_64_SIZE];//4/4/3
	//sscanf(str,"f %s %s %s",&v0,&v1,&v2);
	sscanf_s(str,"f %s %s %s",&v0,G_BUFFER_64_SIZE,&v1,G_BUFFER_64_SIZE,&v2,G_BUFFER_64_SIZE);
	
	MergeVertex(obj,v0);
	MergeVertex(obj,v1);
	MergeVertex(obj,v2);
}
/*
	根据导出类型获取数据间隔
*/
static int 
GetFloatGap(int exportType)
{
	if(exportType == OBJ_UV_VERTEX)
	{
		return UV_GAP + VERTEX_GAP;
	}
	else if(exportType == OBJ_UV_VERTEX_NORMAL)
	{
		return UV_GAP + NORMAL_GAP + VERTEX_GAP;
	}
	return 0;
}

/*
	申请一块内存用于储存顶点数据
*/
static void 
CreateOutBuffer(struct ObjParseVOType * obj)
{
	int byteSize;
	obj->outLen = obj->faceCount * VERTEX_GAP *	obj->everyFloatCount;
	byteSize = sizeof(float)*(obj->outLen)+1;
	obj->outArray =(float*) tl_malloc(byteSize);
	memset(obj->outArray,0,byteSize);
}

/*
	获取各种标示的数量,目的:为开辟动态缓存
*/
void 
Create(struct ObjParseVOType * obj,const char* soures){
	int len;
	char* _buffer;
	
	int byteSize = 0;
	
	char* token;

	char seps[] = "\n"; 
	char *next_token = NULL;

	len = sizeof(char) * (int)strlen(soures) + 1;
	_buffer = (char*)tl_malloc(len);
	memset(_buffer,0,len);
	memcpy(_buffer,soures,len-1);
	
	obj->vertexCount = 0;
	obj->vertexNormalCount = 0;
	obj->uvCount = 0;
	obj->faceCount = 0;
	obj->groupCount = 0;
	
	obj->everyFloatCount=GetFloatGap(obj->exportType);
	
	//token = strtok(_buffer,OBJ_SIGN_BR);
	token = strtok_s(_buffer, seps, &next_token);
	while(token!= NULL){
		char b[3];
		memset(b,0,3);
		memcpy(b,token,2);
		if(!strcmp(b,OBJ_SIGN_COMMAND)){
			//obj 注释行
		}else if(!strcmp(b,OBJ_SIGN_G)){
			//group
			obj->groupCount++;
		}else if(!strcmp(b,OBJ_OBJ_SIGN_VN)){
			//vertex normal
			obj->vertexNormalCount++;
		}else if(!strcmp(b,OBJ_SIGN_V)){
			//vertex
			obj->vertexCount++;
		}else if(!strcmp(b,OBJ_SIGN_VT)){
			//uv
			obj->uvCount++;
		}else if(!strcmp(b,OBJ_SIGN_F)){
			//face
			obj->faceCount++;
		}else if(!strcmp(b,OBJ_SIGN_S)){
			//smooth
		}else{

		}
		//token = strtok( NULL, OBJ_SIGN_BR );
		token = strtok_s(NULL, seps, &next_token);
	}
	obj->uvIndex = 0;
	obj->vertexIndex = 0;
	obj->normalIndex = 0;
	obj->outArrayIndex = 0;
	obj->normalArray = (float*)tl_malloc(sizeof(float) * obj->vertexNormalCount * NORMAL_GAP);
	obj->vertexArray = (float*)tl_malloc(sizeof(float) * obj->vertexCount * VERTEX_GAP);
	obj->uvArray     = (float*)tl_malloc(sizeof(float) * obj->uvCount * UV_GAP);
	
	tl_free(_buffer);
}

/*
	解析顶点Position数据
*/
static void 
f_ParseVertex(const char* str,float* _s,const int _i)
{
	sscanf_s(str,"v %f %f %f",_s+_i,_s+_i+1,_s+_i+2);
}

/*
	解析顶点UV数据
*/
static void 
f_ParseUV(const char* str,float* _s,const int _i)
{
	sscanf_s(str,"vt %f %f",_s+_i,_s+_i+1);
}

/*
	解析顶点Normal数据
*/
static void 
f_ParseVertexNormal(const char* str,float* _s,const int _i)
{
	sscanf_s(str,"vn %f %f %f",_s+_i,_s+_i+1,_s+_i+2);
}

/*
解析一行
*/
static void 
parseOneLine(struct ObjParseVOType* obj,char* s)
{
	int i;
	char b[3];
	const char* seq = OBJ_OBJ_SIGN_SPACE;

//	printf("one line data:%s\n",s);

	memset(b,0,3);
	memcpy(b,s,2);
	
	if(!strcmp(b,OBJ_SIGN_COMMAND)){
		//obj 注释行

	}else if(!strcmp(b,OBJ_SIGN_G)){
		//group

	}else if(!strcmp(b,OBJ_OBJ_SIGN_VN)){
		//vertex normal
		i = obj->normalIndex;
		//strConvertFloatArray(s,obj->normalArray,i,seq);
		f_ParseVertexNormal(s,obj->normalArray,i);

		obj->normalIndex+=NORMAL_GAP;
	}else if(!strcmp(b,OBJ_SIGN_V)){
		//vertex
		i = obj->vertexIndex;
		//strConvertFloatArray(s,obj->vertexArray,i,seq);
		f_ParseVertex(s,obj->vertexArray,i);

		obj->vertexIndex+=VERTEX_GAP;
	}else if(!strcmp(b,OBJ_SIGN_VT)){
		//uv
		i = obj->uvIndex;
		//strConvertFloatArray(s,obj->uvArray,i,seq);
		f_ParseUV(s,obj->uvArray,i);
		obj->uvIndex+=UV_GAP;
//		printf("uv %s\n",s);
	}else if(!strcmp(b,OBJ_SIGN_F))
	{
		convertArray(obj,s);
	}
	else if(!strcmp(b,OBJ_SIGN_S)){
		//smooth
	}else{

	}
}

/*
	解析一行字符
*/
static void 
f_objSplit(int* inputParms,char* splitStr)
{
	parseOneLine((struct ObjParseVOType*)inputParms,splitStr);
}

static void
DelConvertNode(int data,int parms)
{
	struct ConvertData* vd = (struct ConvertData*)data;
	tl_free(vd);
}
/*
	释放解析obj数据处理的资源
*/
static void Dispose(int h){
	struct ObjParseVOType* ob = (struct ObjParseVOType*)h;

	tl_free(ob->vertexArray);
	ob->vertexArray = NULL;

	tl_free(ob->normalArray);
	ob->normalArray = NULL;

	tl_free(ob->uvArray);
	ob->uvArray=NULL;

	//tl_free(ob->outArray);//不释放输出顶点

	if(ob->_uvList){
		LStack_ergodic_t(ob->_uvList,DelConvertNode,0);
		LStack_delete(ob->_uvList);
	}

	if(ob->_normalList){
		LStack_ergodic_t(ob->_normalList,DelConvertNode,0);
		LStack_delete(ob->_normalList);
	}

	tl_free(ob);
}
/*
	获取一个实例
*/
static struct ObjParseVOType* 
GetNewObjParseVO()
{
	struct ObjParseVOType * ptr=(struct ObjParseVOType *)tl_malloc(sizeof(struct ObjParseVOType));
	memset(ptr,0,sizeof(struct ObjParseVOType));
	return ptr;
}

/*
	copy数据
*/
static void
CopyUVConvertNode(int data,int parms)
{
	struct ConvertData* v = (struct ConvertData*)data;
	struct CopyData* c = (struct CopyData*)parms;
	memcpy(c->n+v->index*UV_GAP,c->o+v->oldIndex*UV_GAP,sizeof(float)*UV_GAP);
}
static void
CopyNormalConvertNode(int data,int parms)
{
	struct ConvertData* v = (struct ConvertData*)data;
	struct CopyData* c = (struct CopyData*)parms;
	memcpy(c->n+v->index*NORMAL_GAP,c->o+v->oldIndex*NORMAL_GAP,sizeof(float)*NORMAL_GAP);
}

struct ExportOBJ_Data* 
obj_getData(const char* data)
{
	struct ExportOBJ_Data* objData = tl_malloc(sizeof(struct ExportOBJ_Data));
	struct ObjParseVOType* _obj;
	memset(objData,0,sizeof(struct ExportOBJ_Data));

	_obj = GetNewObjParseVO();
	_obj->ptrIndexObjData = objData;

	_obj->_normalList = LStack_create();
	_obj->_uvList = LStack_create();

	Create(_obj,data);
	
	tl_splitByStr((const char*)data,OBJ_SIGN_CHAR_BR,f_objSplit,(int*)_obj);

	//obj->vertexArray = (float*)tl_malloc(sizeof(float) * obj->vertexCount * OBJ_VERTEX_GAP);

	//copy数据
	{
		int _vertexSize = sizeof(float) * _obj->vertexCount * VERTEX_GAP;
		
		//copy顶点数据
		objData->ptrVertex = tl_malloc(_vertexSize);
		memcpy(objData->ptrVertex,_obj->vertexArray,_vertexSize);
		
		//copy uv数据
		{
			int _uvSize = sizeof(float) * _obj->vertexCount * UV_GAP;
			struct CopyData uv;
			objData->ptrUV = tl_malloc(_uvSize);
			uv.o = _obj->uvArray;
			uv.n = objData->ptrUV;
			LStack_ergodic_t(_obj->_uvList,CopyUVConvertNode,(int)&uv);
		}
		
		//copy normal数据
		{
			struct CopyData normal;
			int _normalSize = sizeof(float) * _obj->vertexCount * NORMAL_GAP;
			objData->ptrNormal = tl_malloc(_normalSize);
			normal.o =_obj->normalArray;
			normal.n = objData->ptrNormal;
			LStack_ergodic_t(_obj->_normalList,CopyNormalConvertNode,(int)&normal);
		}
		
		//{
		//	//测试
		//	int i,k;
		//	if(NULL)
		//	{
		//		k = 0;
		//		for(i = 0;i < _obj->vertexCount * OBJ_UV_GAP;i++){
		//			
		//			if(k>=OBJ_UV_GAP)
		//			{
		//				k = 0;
		//				printf("\n");
		//			}
		//			printf("%f,",objData->ptrUV[i]);
		//			k++;
		//		}
		//		printf("\n");
		//	}
		//	if(NULL)
		//	{
		//		k = 0;
		//		for(i = 0;i < _obj->uvCount * OBJ_UV_GAP;i++)
		//		{
		//			if(k>=OBJ_UV_GAP)
		//			{
		//				k = 0;
		//				printf("\n");
		//			}
		//			printf("%f,",_obj->uvArray[i]);
		//			k++;
		//		}
		//	}
		//}
	}
	objData->vertexCount = _obj->vertexCount;
	objData->renderVertCount = _obj->faceCount * 3;
	Dispose((int)_obj);

	return objData;
}
void obj_delData(struct ExportOBJ_Data* ptr)
{
	if(ptr->ptrVertex){
		tl_free(ptr->ptrVertex);
		ptr->ptrVertex=NULL;
	}
	if(ptr->ptrUV){
		tl_free(ptr->ptrUV);
		ptr->ptrUV = NULL;
	}
	if(ptr->ptrNormal){
		tl_free(ptr->ptrNormal);
		ptr->ptrNormal = NULL;
	}
	if(ptr->ptrIndex){
		/*
		int i;
		for(i = 0;i < ptr->indexLength;i++){
			printf("===>%d:%d\n",i,ptr->ptrIndex[i]);
		}
		*/
		tl_free(ptr->ptrIndex);
	}

	tl_free(ptr);
}
void 
obj_parse(const char* data,int* _outLength,int* outArray,int exportType)
{
	struct ObjParseVOType* t;

	long tmpms;
	
	if(G_GLOBAL_PRINT_OBJ_DEBUG){
		tmpms = get_longTime();
	}
	t = GetNewObjParseVO();

	t->exportType = exportType;
	Create(t,data);

	//创建一个数据缓冲区
	CreateOutBuffer(t);

	if(t!=NULL){
		
		tl_splitByStr((const char*)data,OBJ_SIGN_CHAR_BR,f_objSplit,(int*)t);

		*outArray = (int)t->outArray;
		*_outLength = t->outLen;

		//tl_printfEnt3D((float*)*outArray,*len);
	}else{
		assert(t == NULL);
	}
	Dispose((int)t);

	if(G_GLOBAL_PRINT_OBJ_DEBUG){
		printf("解析obj模型消耗%ld毫秒\n",get_longTime()-tmpms);
	}
}
//=================================================================================