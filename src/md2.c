#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tools.h"
#include "tl_malloc.h"
#include "md2.h"
#include "byte.h"
#include "list.h"
#include "gettime.h"
#include "vmath.h"
//====================================================================================
//MD2接口实现
struct MD2_ParseObj{
	//############################################################
	/** 关键帧列表引用*/
	struct MD2_Frame* _pframe;

	/**
	 * 总帧数
	 */
	int _numFrames;
	/*
	*	数据
	*/                                                                                                                                                                                                                                                                                                                                                                   
	struct BytesVO* b;
	
	//版本号
	int _version;
	
	//文件标志：844121161（"IPD2"）；
	int _ident;
	/**
	 * 纹理宽度
	 */
	int _skinWidth;
	/**
	 * 纹理高度
	 */
	int _skinHeight;
	/**
	* 每一帧的字节数
	*/
	int _frameSize;
	
	/**
	 * 纹理数目
	 */
	int	_numSkins;
	/**
	 * 顶点数目(每一帧)
	 */    
	int _numVertices;
	/**
	 * 纹理坐标数目 (每一帧)
	 */    
	int _numST;
	/**
	 * 三角形数目 (每一帧)
	 */
	int _numTris;
	/**
	 * gl命令数目(每一帧)
	 */
	int _numGlCmds;
	
	/**
	 * 纹理的偏移位置
	 */
	int _offsetSkins;
	/**
	 * 纹理坐标的偏移位置
	 */
	int _offsetST;
	/**
	 * 三角形索引的偏移位置
	 */
	int _offsetTris;
	/**
	 * 第一帧的偏移位置
	 */
	int _offsetFrames;
	/**
	 * OpenGL命令的偏移位置
	 */
	int _offsetGlCmds;
	/**
	 * 文件结尾的偏移位置
	 */
	int _offsetEnd;
	/**
	*	解析头部信息的标示
	*/
	int _parsedHeader;
	//============================
	/*
	*	解析uv的标示
	*/
	int _parsedUV;
	/*
	*	解析面片标示
	*/
	int _parsedFaces;
	/*
	*	关键帧标示
	*/
	int _parsedFrames;

	/**
	 * 顶点-坐标组合索引对；_vertIndices对应_uvIndices且_vertIndices.length<=_indices.length;
	 */
	struct List* _vertIndices;

	/**
	*	uv索引
	*/
	struct List* _uvIndices;

	/**
	* 三角形索引，三个一组，_indices = 3 * _numTris;
	*/
	struct List* _indices;
	
	float* _uvs;

	float* _finalUV;
};

static int fHasTime()
{
	return 1;
}

static int fgetpIndex(struct MD2_ParseObj* _md2,int j){
	int i = (int)_md2->_indices->list[j];	
 	int	k = (int)_md2->_vertIndices->list[i];
	return k;
}

/*
*	解析md2头部信息
*/
static void f_parseHeader(struct MD2_ParseObj* _md2)
{
	struct BytesVO *b = _md2->b;
	bs_readInt(b,&(_md2->_ident));
	bs_readInt(b,&(_md2->_version));
	bs_readInt(b,&(_md2->_skinWidth));
	bs_readInt(b,&(_md2->_skinHeight));
	bs_readInt(b,&(_md2->_frameSize));
	bs_readInt(b,&(_md2->_numSkins));
	bs_readInt(b,&(_md2->_numVertices));
	bs_readInt(b,&(_md2->_numST));
	bs_readInt(b,&(_md2->_numTris));
	bs_readInt(b,&(_md2->_numGlCmds));
	bs_readInt(b,&(_md2->_numFrames));
	bs_readInt(b,&(_md2->_offsetSkins));
	bs_readInt(b,&(_md2->_offsetST));
	bs_readInt(b,&(_md2->_offsetTris));
	bs_readInt(b,&(_md2->_offsetFrames));
	bs_readInt(b,&(_md2->_offsetGlCmds));
	bs_readInt(b,&(_md2->_offsetEnd));
	_md2->_parsedHeader = 1;
	//printf("%d,%d,%d,%d,%d _offsetEnd：%d\n",_md2->_ident,_md2->_version,_md2->_skinWidth,_md2->_skinHeight,_md2->_frameSize,_md2->_offsetEnd);
}
/*
*	Parses the file names for the materials.
*	解析文件中材质信息
*/
static void f_parseMaterialNames(struct MD2_ParseObj* _md2)
{
	int i = 0;
	int k,v;
	//char _nameBuffer[64];
	//printf("->%d,%d\n",_md2->_offsetSkins,(_md2->b)->mPos);
	//printf("%d,%d\n",sizeof(int),_md2->_numSkins);
	for(i = 0;i < _md2->_numSkins;i++)
	{
		for(k=0;k< 16;k++)
		{
			bs_readInt(_md2->b,&v);
		}
		//读取64个字节的文本,需要调整
	}
}
/*
	增加一个顶点数据
*/
static struct Vec3 f_addOneVertices(struct MD2_ParseObj* _md2,struct List* _l,int i,int j,float* _lver,int index,int _offset)
{
	float px,py,pz,u,v;
	int tmp,cur;
	struct Vec3 pos;
	px = _l->list[i*3];
	py = _l->list[i*3+1];
	pz = _l->list[i*3+2];
	
	tmp =	(int)_md2->_indices->list[j*3+_offset];//get_IndicesIndex(_md2,j*3);
	u = _md2->_finalUV[ tmp*2];
	v = _md2->_finalUV[ tmp*2+1 ];

	cur = index;

	//
	//	翻转uv :	1.0-uv
	//

	//uv
	_lver[cur]  = u;
	_lver[cur+1]= 1.0f-v;//翻转v

	//法线
	_lver[cur+2]=1;
	_lver[cur+3]=0;
	_lver[cur+4]=0;

	//顶点数据
	_lver[cur+5]=px;
	_lver[cur+6]=py;
	_lver[cur+7]=pz;

	pos.x = px;
	pos.y = py;
	pos.z = pz;
	return pos;
}
/*
*	解析uv
*/
static void f_parseUV(struct MD2_ParseObj* _md2)
{
	int i;
	int j = 0;
	struct BytesVO *b = _md2->b;
	short u,v;
	float _u,_v;
	//printf("parseUV %d,%d %d,%d  _md2->_numST:%d\n",_md2->_offsetST,(_md2->b)->mPos, _md2->_skinWidth,_md2->_skinHeight,_md2->_numST);
	//printf("%d,%d\n",sizeof(double),sizeof(float));

	_md2->_uvs = (float*)tl_malloc(sizeof(float)*_md2->_numST * 2);
	
	for(i = 0;i < _md2->_numST;i++)
	{
		bs_readShort(b,&u);
		bs_readShort(b,&v);
		_u = (float)u/(float)_md2->_skinWidth;
		_v = (float)v/(float)_md2->_skinHeight;
		
		_md2->_uvs[j++] = _u;
		_md2->_uvs[j++] = _v;
		//存储_u,_v
		//printf("uv:%f,%f \n",_u,_v);
	}
	_md2->_parsedUV = 1;
}
static void fDeleteVert(struct List* _l){
	if(_l!=0)
	{
		list_dispose(_l);
		tl_free(_l);
	}
}
/*
	绘制一帧
*/
void fBuildVertices(struct MD2_ParseObj* _md2,struct List* _l,const char* frameName,int frameIndex)
{
	int j,t,index;
	int pindex1,pindex2,pindex3;

	int useCalulateNormal = 0;//是否生成计算法线向量

	struct MD2_Frame* frame = 0;
	
	//三角形的个数
	int triangleNum =	_md2->_indices->length / 3;
	float* vertices;
	int* indices;

	vertices = (float*)tl_malloc((sizeof(float) * triangleNum * 8 * 3));
	//struct List* vertices = (struct List*)tl_malloc(sizeof(struct List));
	//list_create(vertices);

	indices = (int*)tl_malloc((sizeof(int)*triangleNum*3));
	t=0;
	index = 0;
	for (j = 0; j < triangleNum; j++) 
	{
		struct Vec3 verticeX;
		struct Vec3 verticeY;
		struct Vec3 verticeZ;
		//struct Vec3 _outNormal;
	
		int spacing = 8;//一个顶点的跨度

		pindex1 = fgetpIndex(_md2,j*3);
		pindex2 = fgetpIndex(_md2,j*3+1);
		pindex3 = fgetpIndex(_md2,j*3+2);
		
		verticeX=f_addOneVertices(_md2,_l,pindex1,j,vertices,index,0);
		index += spacing;
		verticeY=f_addOneVertices(_md2,_l,pindex2,j,vertices,index,1);
		index += spacing;
		verticeZ=f_addOneVertices(_md2,_l,pindex3,j,vertices,index,2);
		index += spacing;
		
		//if(useCalulateNormal)
		//{
		//	int n;
		//	//计算法线向量
		//	vec3CalculateNormal(&verticeX,&verticeY,&verticeZ,&_outNormal);
		//	
		//	//向量归一
		//	vec3Normalize(&_outNormal);
		//	index-=spacing*3;

		//	for(n = 0;n < 3;n++)
		//	{
		//		vertices[index+2]=_outNormal.z;
		//		vertices[index+3]=_outNormal.y;
		//		vertices[index+4]=_outNormal.x;
		//	}
		//	//恢复索引序号
		//	index+=spacing*3;
		//}


		t = j*3;
		indices[t] = j * 3;
		indices[t+1] = j * 3 + 1;
		indices[t+2] = j * 3 + 2;
	}

	//说明:为名字建立一个索引,根据动作命获取动作索引列表。一条索引列表有一段动作片段	
	//设置关键帧名


	frame = &(_md2->_pframe[frameIndex]);
	memset(frame->_name,0,G_BUFFER_16_SIZE);
	if(frameName != 0)
	{
		memcpy(frame->_name,frameName,strlen(frameName));
		//printf("_name:%s\n",frame->_name);
	}
	frame->vertices = vertices;
	frame->vertCount = triangleNum * 3;
	frame->_indices = indices;
	frame->_index = frameIndex;
	
	//输出关键帧名字
	//printf("name: %s index:%d\n",frame->name,frame->index);
}

/*
*	解析关键帧
*/
static void 
fParseFrames(struct MD2_ParseObj* _md2)
{
	int i,k,j,t;
	int ch,ctx,cty,ctz;
	//int _normalIndex;//法线索引号
	
	float sx,sy,sz,tx,ty,tz,_x,_y,_z;
	struct BytesVO *b = _md2->b;
	struct MD2_Frame* frame = 0;
	struct List* tvertices = 0;

	//关键帧名
	char name[16];
	char _tempName[16];
	memset(_tempName,0,16);


	
	//开辟关键帧列表这里开辟了所有的缓存区,那么释放其实只要一次
	_md2->_pframe = (struct MD2_Frame*)tl_malloc(sizeof(struct MD2_Frame) * _md2->_numFrames);
	
	//printf(/*"%0x\n%0x\n"*/"%d\n%d\n",&(_md2->pframe[0]),&(_md2->pframe[1]));
	
	for(i = 0;i < _md2->_numFrames;i++)
	{
		#ifdef _DEBUG_MODE_
			int __time = get_longTime();
		#endif

		fDeleteVert(tvertices);
		tvertices= (struct List*)tl_malloc(sizeof(struct List));
		list_create(tvertices);

		//void bs_readInt(struct BytesVO*t,int* out);/*读取一个int*/

		//缩放和位移
		bs_readFloat(b,&sx);
		bs_readFloat(b,&sy);
		bs_readFloat(b,&sz);
		bs_readFloat(b,&tx);
		bs_readFloat(b,&ty);
		bs_readFloat(b,&tz);
		
		//printf("_md2->_numFrames: %d  %f,%f,%f,%f,%f,%f\n",i,(float)sx,(float)sy,(float)sz,(float)tx,(float)ty,(float)tz);
		//printf("%f,",(float)sx);

		//打印法线数据


		k = 0;
		t = 0;
		memset(name,0,16);
		//memcpy(name,"frameName",strlen("frameName"));

		for(j = 0;j < 16;j++)
		{
			bs_readByte(b,&ch);
			if (ch >= 0x39 && ch <= 0x7A && k == 0) 
			{
				//name += String.fromCharCode(ch);
				//printf("%d %c,",ch,ch);
				name[t] = (char)ch;
				t++;
			}
			if (ch >= 0x30 && ch <= 0x39) 
			{
				k++;
			}
		}

		for (j = 0; j < _md2->_numVertices; j++,b->mPos++) 
		{
			
			//顶点坐标
			//tvertices.push(sx * _byteData.readUnsignedByte() + tx, sy * _byteData.readUnsignedByte() + ty, sz * _byteData.readUnsignedByte() + tz);
			bs_readByte(b,&ctx);
			bs_readByte(b,&cty);
			bs_readByte(b,&ctz);

			//bs_readShort(b,&normalIndex);
			//printf("=====>  %d,%d\n",j,_normalIndex);
			//bs_readByte(b,&_normalIndex);
			//b->mPos-=sizeof(char);

			_x = sx * ctx + tx;
			_y = sy * cty + ty;
			_z = sz * ctz + tz;
			
			//printf("index:%d\n",normalIndex);

			list_add(tvertices,_x);
			list_add(tvertices,_y);
			list_add(tvertices,_z);

		}
		k = 0;

		//if(name!=NULL && strcmp(name,"run")==0){
		{
			//打印动画信息数据
			int _stat = strcmp(_tempName,name);
			if(_stat!=0){
				memset(_tempName,0,16);
				memcpy(_tempName,name,strlen(name));
				//log_color(0xffff00,"动画name: (%s)\t结束索引:%d\n",name,i);
			}
		}
		fBuildVertices(_md2,tvertices,(const char*)name,i);
		//}
		#ifdef _DEBUG_MODE_
			//printf("解析关键帧索引%d消耗 %ld 毫秒\n",i,(get_longTime() - __time));
			{
				int c = i + 1;
				log_color(0x00ff00,"############################### md2:关键帧数 = %d 顶点总数 = %d  %d/%d %.2f%% \r",_md2->_numFrames,_md2->_numVertices,c,_md2->_numFrames,(float)c/(float)_md2->_numFrames * 100);
			}
		#endif	
	}

	#ifdef _DEBUG_MODE_ 
		printf("\n");	
	#endif

	fDeleteVert(tvertices);
	tvertices = 0;

	_md2->_parsedFrames =1;
}
/**
 * Finds the final index corresponding to the original MD2's vertex and uv indices. Returns -1 if it wasn't added yet.
 * @param vertexIndex The original index in the vertex list.
 * @param uvIndex The original index in the uv list.
 * @return The index of the final mesh corresponding to the original vertex and uv index. -1 if it doesn't exist yet.
 */
int findIndex(struct MD2_ParseObj* _md2,int vertexIndex,int uvIndex)
{
	int i,vert,uv;
	int len = _md2->_vertIndices->length;//LStack_length(_md2->_vertIndices);
	for (i = 0; i < len; ++i){
		vert = (int)_md2->_vertIndices->list[i];
		uv = (int)_md2->_uvIndices->list[i];
		if(vert == vertexIndex && uv == uvIndex){
			return i;
		}
		
	}
	return -1;
}
/*
*	增加索引
*/
static void 
fAddIndex(struct MD2_ParseObj* _md2,unsigned short vertexIndex,unsigned short uvIndex){
	int index = findIndex(_md2,vertexIndex, uvIndex);
	if(index == -1){
		list_add(_md2->_indices,(float)_md2->_vertIndices->length);

		list_add(_md2->_vertIndices,vertexIndex);

		list_add(_md2->_uvIndices,uvIndex);
	}
	else
	{
		//LStack_push(_md2->_indices,index);
		list_add(_md2->_indices,(float)index);
	}
}

static void 
fParseFaces(struct MD2_ParseObj* _md2)
{
	short  a,b,c,ta,tb,tc;
//	void bs_readUnsignedShort(struct BytesVO* t,unsigned short *p);/*读取一个无符号的short*/
	struct BytesVO *byte = _md2->b;
	int i,len;
	int _tmp;

	//创建顶点索引列表
	_md2->_vertIndices = (struct List*)tl_malloc(sizeof(struct List));//(void*)LStack_create();
	list_create(_md2->_vertIndices);

	//创建uv索引列表
	_md2->_uvIndices = (struct List*)tl_malloc(sizeof(struct List));//(void*)LStack_create();
	list_create(_md2->_uvIndices);

	_md2->_indices = (struct List*)tl_malloc(sizeof(struct List));//(void*)LStack_create();
	list_create(_md2->_indices);

	//printf("_md2->numTris:%d (_md2->b)->mPos:%d _md2->_offsetTris:%d\n",_md2->numTris,(_md2->b)->mPos,_md2->_offsetTris);
	//(_md2->b)->mPos = _md2->_offsetTris;
	for (i = 0; i < _md2->_numTris; i++) 
	{
		//collect vertex indices
		bs_readShort(byte,&a);

		bs_readShort(byte,&b);
		bs_readShort(byte,&c);

		//collect uv indices
		bs_readShort(byte,&ta);
		bs_readShort(byte,&tb);
		bs_readShort(byte,&tc);
		fAddIndex(_md2,a,ta);
		fAddIndex(_md2,b,tb);
		fAddIndex(_md2,c,tc);

		//printf("%d %d %d %d %d %d-",a,ta,b,tb,c,tc);
		//printf("%u,%u,%u,%u,%u,%u\n",a,ta,b,tb,c,tc);
	}

	len = _md2->_uvIndices->length;

	_md2->_finalUV = (float*)tl_malloc(sizeof(float)*len*2);

	for (i = 0; i < len; ++i) 
	{
		_tmp = (int)_md2->_uvIndices->list[i];

		//printf("%d,",_tmp);
		_md2->_finalUV[(i << 1)] = _md2->_uvs[(_tmp << 1)];
		_md2->_finalUV[(((i << 1) + 1))] = _md2->_uvs[( (_tmp << 1) + 1)];
	}

	_md2->_parsedFaces = 1;
}

void* md2parse_load(const char* str,int len){
	struct MD2_ParseObj* _md2;
#ifdef  _DEBUG_MODE_
	int __time = get_longTime();
	log_color(0x00ff00,"############################### md2文件开始解析(%d字节)%.3fkb\n",len,(float)len/1024);
#endif

	_md2 = (struct MD2_ParseObj*)tl_malloc(sizeof(struct MD2_ParseObj));
	_md2->b = (struct BytesVO*)tl_malloc(sizeof(struct BytesVO));
	
	(_md2->b)->bigEndian = IS_BIG_ENDIAN;
	//_md2->tempData = getTemp();//用一个三角形进行测试

	bs_read_start(_md2->b,(char*)str,len,IS_BIG_ENDIAN);

	//_md2->curFrame = 0;
	_md2->_parsedHeader = 0;
	_md2->_parsedUV = 0;
	_md2->_parsedFaces = 0;
	_md2->_parsedFrames = 0;

	//设置线框尺寸,0的时候不绘制线框
	//_md2->lineSize = 0.0f;
	
	//memset(_md2->curAnim,0,G_BUFFER_16_SIZE);
	
	while(fHasTime())
	{
		if(!_md2->_parsedHeader)
		{
			f_parseHeader(_md2);
			f_parseMaterialNames(_md2);
		}
		else
		{
			if(!_md2->_parsedUV)
			{
				f_parseUV(_md2);
			}
			else
			{
				if(!_md2->_parsedFaces)
				{
					fParseFaces(_md2);
				}
				else
				{
					if(!_md2->_parsedFrames)
					{
						fParseFrames(_md2);
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	//清理临时数据
	list_dispose(_md2->_vertIndices);
	tl_free(_md2->_vertIndices);
	_md2->_vertIndices = 0;

	list_dispose(_md2->_uvIndices);
	tl_free(_md2->_uvIndices);
	_md2->_uvIndices = 0;
	
	list_dispose(_md2->_indices);
	tl_free(_md2->_indices);
	_md2->_indices = 0;

	tl_free(_md2->_uvs);

	tl_free(_md2->_finalUV);

	//printf("解析结束\n");
	bs_dispose(_md2->b);
	tl_free(_md2->b);

#ifdef  _DEBUG_MODE_
	log_color(0x00ff00,"############################### md2文件解析结束,耗时 %ld 毫秒\n\n",(get_longTime()-__time));
#endif
	
	return _md2;
}
void md2parse_dispose(void* ptr){
	int i,len;
	struct MD2_ParseObj* _md2 = (struct MD2_ParseObj*)ptr;
	len =  _md2->_numFrames;
	//_md2->pframe需要逐个free
	for(i = 0;i < len;i++){
		struct MD2_Frame* frame = &(_md2->_pframe[i]);
		if(frame->_indices){
			tl_free(frame->_indices);
		}
		if(frame->vertices){
			tl_free(frame->vertices);
		}
		//memset(frame,0,sizeof(struct MD2_Frame));
		//tl_free(frame);
	}
	tl_free(_md2->_pframe);
	tl_free(_md2);
}
/**
*	md2的总帧数
*/
int md2parse_totalFrames(void* ptr){
	return ((struct MD2_ParseObj*)ptr)->_numFrames;
}
/**
*	获取指定索引的MD2_Frame引用
*/
void* md2parse_getFrame(void* ptr,int index){
	struct MD2_ParseObj* p = (struct MD2_ParseObj*)ptr;
	return &(p->_pframe[index]);
}
///*
//	获取顶点个数和,顶点引用
//*/
//void md2parse_getFrameVertex(void* md2Frame,float* pVertices,int* pVertCnt){
//	struct MD2_Frame* p = (struct MD2_Frame*)md2Frame;
//	pVertices = p->vertices;
//	*pVertCnt = p->vertCount;
//}
//====================================================================================