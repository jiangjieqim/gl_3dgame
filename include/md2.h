#ifndef _MD2PARSE_H_
#define _MD2PARSE_H_



/*
*	关键帧结构体
*/
struct MD2_Frame
{
	/*
	*	关键帧名
	*/
	char name[G_BUFFER_16_SIZE];

	/*
	*	关键帧索引号
	*/
	int index;

	/*
	*	顶点定义列表
	*/
	float* vertices;

	/*
	*	顶点个数
	*/
	int vertCount;

	/*
	*	索引定义
	*/
	int* indices;
};

struct MD2_ParseObj
{
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
	int numVertices;
	/**
	 * 纹理坐标数目 (每一帧)
	 */    
	int _numST;
	/**
	 * 三角形数目 (每一帧)
	 */
	int numTris;
	/**
	 * gl命令数目(每一帧)
	 */
	int _numGlCmds;
	/**
	 * 总帧数
	 */
	int _numFrames;
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

	struct MD2_Frame* pframe;

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

//md2解析模块

/*
	解析
*/
void md2parse_load(struct MD2_ParseObj* _md2,const char* str,int len);

/*
	销毁
*/
void md2parse_dispose(struct MD2_ParseObj* _md2);



#endif