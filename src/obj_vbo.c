#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "tl_malloc.h"
#include "tlgl.h"
#include "obj.h"
#include "obj_vbo.h"
#include "ex.h"
#include "tmat.h"
#include "anim.h"
#include "node.h"
#include "base.h"

//销毁
static void
objData_dispose(struct ExportOBJ_Data* obj)
{
	tl_free(obj->ptrVertex);
	tl_free(obj->ptrUV);
	if(obj->ptrNormal){
		tl_free(obj->ptrNormal);
	}
	tl_free(obj->ptrIndex);
	obj->ptrIndex = NULL;
}
//销毁vbo
static void 
vbo_dispose(struct ObjVBO* p)
{
	if(!p)
	{
		return;
	}
	if(p->vertexID)
		glDeleteBuffers(1,&p->vertexID);

	if(p->uvID)
		glDeleteBuffers(1,&p->uvID);

	if(p->normalID)
		glDeleteBuffers(1,&p->normalID);

	if(p->indexID)
		glDeleteBuffers(1,&p->indexID);

	tlgl_setVboSize(p->byteSize,0);
}

/************************************************************************/
/* 这里只渲染一个VBO节点                                                          */
/************************************************************************/
void 
objVBO_renderNode(struct ObjVBO* vbo,struct GMaterial* tmat,
				  const char* _shaderName,Matrix44f m,int mode,struct HeadInfo* base,
				  void (*renderCallBack)(int,struct ObjVBO*),void* grid9)
{
	GLboolean cull;
	
	//顶点
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo->vertexID);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//uv
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER,vbo->uvID);
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	
	if(vbo->normalID)
	{
		//normal
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER,vbo->normalID);
		glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo->indexID);

	//////////////////////////////////////////////////////////////////////////
	if(tmat->_Alpha<1.0f){
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);//混合函数  
		glEnable(GL_BLEND);//启用透明，注意不要开启深度测试,即不要有glEnable(GL_DEPTH_TEST)  
	}

	//双面绘制
	cull = base_cullface(base);
	//printf("***%s\n",base->name);
	tmat_render(tmat,_shaderName,m,grid9);

	//设置状态
	if(renderCallBack!=NULL)
		renderCallBack(1,vbo);
	
	glPolygonMode (GL_FRONT_AND_BACK, mode);

	{
		GLenum _enum; 
#ifdef _USE_SIGNED_SHORT_
		_enum = GL_UNSIGNED_SHORT;
#else
		_enum = GL_UNSIGNED_INT;
#endif

		glDrawElements(GL_TRIANGLES,vbo->renderVertCount,_enum , 0);
	}

	//清理状态
	if(renderCallBack!=NULL)
		renderCallBack(0,vbo);
	
	if(tmat->_Alpha<1.0f)
	{
		glDisable(GL_BLEND);
	}
	//////////////////////////////////////////////////////////////////////////
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	if(vbo->normalID)
		glDisableVertexAttribArray(2);

	//glBindVertexArray(0);	//glsl 110 不注释会报错

	if(cull){
		glDisable(GL_CULL_FACE);//清除双面状态
	}
}

void
objVBO_render(int data,int parms)
{
	if(data){
		struct Node* ptr = (struct Node*)parms;
		struct HeadInfo* base =	base_get(ptr);
		
		objVBO_renderNode((struct ObjVBO*)data,(struct GMaterial*)base->tmat,base->tmat->curGlslType,base->m,base_get_ploygonLineMode(base),
			base,
			ptr->renderCallBack,0);
	}else{
		printf("objVBO_render(..)--->data = null!\n");
	}
}

/*
	初始化VAO数据 
	int renderVertCount:	渲染的顶点个数
	int indexCount:		索引个数
*/
static void
initVBO(struct ExportOBJ_Data* _ptr,struct ObjVBO* vbo,int type,int renderVertCount,int indexCount)
{
	int dataByteSize=0;
	vbo->renderVertCount = renderVertCount;

	glEnableVertexAttribArray(0);//顶点坐标
	glEnableVertexAttribArray(1);//顶点uv
	
	//顶点
	glGenBuffers(1, &vbo->vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo->vertexID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*VERTEX_GAP * _ptr->vertexCount,_ptr->ptrVertex,GL_STATIC_DRAW);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	dataByteSize+=sizeof(GLfloat)*VERTEX_GAP * _ptr->vertexCount;

	//uv
	glGenBuffers(1, &vbo->uvID);
	glBindBuffer(GL_ARRAY_BUFFER,vbo->uvID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*UV_GAP * _ptr->vertexCount,_ptr->ptrUV,GL_STATIC_DRAW);
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	dataByteSize+=sizeof(GLfloat)*UV_GAP * _ptr->vertexCount;

	if(type== OBJ_UV_VERTEX_NORMAL)
	{
		glEnableVertexAttribArray(2);//顶点normal
		//normal
		glGenBuffers(1, &vbo->normalID);
		glBindBuffer(GL_ARRAY_BUFFER,vbo->normalID);
		glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*NORMAL_GAP* _ptr->vertexCount,_ptr->ptrNormal,GL_STATIC_DRAW);
		glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, 0 );
		glDisableVertexAttribArray(2);
		dataByteSize+=sizeof(GLfloat)*NORMAL_GAP* _ptr->vertexCount;
	}

	//索引数据
	{
		//===========================================================================
		int _indexSize = sizeof(_UNSIGNED_SHORT);
		int _dataCnt = _indexSize*indexCount;
		glGenBuffers(1, &vbo->indexID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo->indexID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _dataCnt,_ptr->ptrIndex,GL_STATIC_DRAW);
		if(_dataCnt > 65535)
		{
			printf("尺寸太大,dataByteSize=%d",dataByteSize);
			assert(0);
		}
		dataByteSize+=_dataCnt;
	}

	//状态清除操作
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	//计算VBO的占用的内存数
	//printf("VBO缓冲区申请的大小 %d bytes\n",dataByteSize);
	
	{
		vbo->byteSize = dataByteSize;
		tlgl_setVboSize(dataByteSize,1);
	}

}

static void
delVBOList(int data,int parms)
{
	struct ObjVBO* vbo=(struct ObjVBO*)data;
	vbo_dispose(vbo);
	tl_free(vbo);
}

void objVBO_dispose(struct Obj_vbo_model* p)
{
	//删除VBO列表
	LStack_ergodic_t(p->ptrList,delVBOList,0);
	LStack_delete(p->ptrList);
	tl_free(p);
}

static struct Obj_vbo_model*
init()
{
	struct Obj_vbo_model* _ptr = tl_malloc(sizeof(struct Obj_vbo_model));
	memset(_ptr,0,sizeof(struct Obj_vbo_model));
	_ptr->ptrList = LStack_create();

	return _ptr;
}

struct Obj_vbo_model* 
objVBO_create(const char* name,int type)
{
	struct Obj_vbo_model* _pvboModel = init();
	_pvboModel->dataType = type;
	return _pvboModel;
}

/*
 *根据数据类型设置数据间隔
 */
static int 
getGapByVertexType(int vertsType)
{
	return (vertsType == OBJ_UV_VERTEX) ? (UV_GAP + VERTEX_GAP) : (UV_GAP + VERTEX_GAP + NORMAL_GAP);
}
/*
 *复制数据
 */
static void 
copyData(struct ExportOBJ_Data* ptr,int* ptri,int* ptrk,float* ptrVerts,int vertsType)
{
	float*pVertex,*pUV,*pNormal;

	int i = *ptri;
	int k = *ptrk;

	pUV = &(ptrVerts[i]);

	//设置uv数据
	memcpy(&(ptr->ptrUV[k*UV_GAP]),pUV,sizeof(GLfloat)*UV_GAP);

	if(vertsType == OBJ_UV_VERTEX)
	{
		pVertex = &(ptrVerts[i+UV_GAP]);
	}
	else
	{
		pNormal = &(ptrVerts[i+UV_GAP]);

		//设置法线数据
		memcpy(&(ptr->ptrNormal[k*NORMAL_GAP]),pNormal,sizeof(GLfloat)*NORMAL_GAP);	
		{
			//int n=0;
			//for(n = 0;n < 3;n++)
			//	printf("%f,%f,%f\n",pNormal[n],pNormal[n+1],pNormal[n+2]);
		}
		pVertex = &(ptrVerts[i+UV_GAP+NORMAL_GAP]);
	}

	//设置顶点数据
	memcpy(&(ptr->ptrVertex[k*VERTEX_GAP]),pVertex,sizeof(GLfloat)*VERTEX_GAP);

	ptr->ptrIndex[k] = k;
}
/*
	创建一个节点,并压入列表
*/
static struct ObjVBO* 
CreateNewNode(struct Obj_vbo_model* _pvboModel)
{
	struct ObjVBO* vboPtr=tl_malloc(sizeof(struct ObjVBO));
	memset(vboPtr,0,sizeof(struct ObjVBO));
	LStack_push((void*)_pvboModel->ptrList,vboPtr);
	return vboPtr;
}

void 
objVBO_pushExportObj(struct Obj_vbo_model* _pvboModel,const char* _objStr)
{
	struct ObjVBO* vboPtr = CreateNewNode(_pvboModel);
	//printf("CreateNewNode\n");
	{
		struct ExportOBJ_Data* ptr=obj_getData(_objStr);
		//printf("obj_getData\n");
		initVBO(ptr,vboPtr,_pvboModel->dataType,ptr->renderVertCount,ptr->indexLength);
		obj_delData(ptr);
	}
}

/*
	添加一个VBO节点
*/
void
objVBO_pushNode(struct Obj_vbo_model* _pvboModel,GLfloat* verts,int _bufferSize)
{
	float* ptrVerts;
	int i,vertCount;//_bufferSize	float的个数
	int __gap;//数据间隔
	struct ExportOBJ_Data* ptr;
	int k = 0;
	int normalSize,uvSize,positionSize;
	int vertsType;
	//////////////////////////////////////////////////////////////////////////

	/*struct ObjVBO* vboPtr=tl_malloc(sizeof(struct ObjVBO));
	memset(vboPtr,0,sizeof(struct ObjVBO));
	LStack_push((void*)_pvboModel->ptrList,(int)vboPtr);*/
	struct ObjVBO* vboPtr=CreateNewNode(_pvboModel);
	
	vertsType = _pvboModel->dataType;

	//根据数据类型设置数据间隔
	__gap = getGapByVertexType(vertsType);

	//////////////////////////////////////////////////////////////////////////
	//一个类型的数据需要的字节大小
	uvSize = sizeof(GLfloat)*UV_GAP;
	positionSize = sizeof(GLfloat)*VERTEX_GAP;
	//////////////////////////////////////////////////////////////////////////

	ptrVerts = (GLfloat*)verts;

	vertCount = _bufferSize / __gap;//顶点个数

	ptr = tl_malloc(sizeof(struct ExportOBJ_Data));
	memset(ptr,0,sizeof(struct ExportOBJ_Data));
	ptr->vertexCount = vertCount;

	ptr->ptrVertex = tl_malloc(positionSize * vertCount);

	if(_pvboModel->dataType== OBJ_UV_VERTEX_NORMAL)
	{
		normalSize = sizeof(GLfloat)*NORMAL_GAP;
		ptr->ptrNormal = tl_malloc(normalSize * vertCount);
		memset(ptr->ptrNormal,0,normalSize * vertCount);
	}
	ptr->ptrUV = tl_malloc(uvSize*vertCount);
	ptr->indexLength = vertCount;
	ptr->ptrIndex = tl_malloc(ptr->indexLength*sizeof(_UNSIGNED_SHORT));

	memset(ptr->ptrVertex,0,positionSize * vertCount);
	memset(ptr->ptrUV,0,uvSize * vertCount);
	//////////////////////////////////////////////////////////////////////////

	for(i = 0;i < _bufferSize;i+=__gap)
	{
		copyData(ptr,&i,&k,ptrVerts,vertsType);

		k++;
	}

	//////////////////////////////////////////////////////////////////////////

	initVBO(ptr,vboPtr,_pvboModel->dataType,ptr->vertexCount,ptr->vertexCount);

	objData_dispose(ptr);
	tl_free(ptr);
}
