#ifndef _MD2PARSE_H_
#define _MD2PARSE_H_
/*
*	关键帧结构体,代表一个md2关键帧数据
*/
struct MD2_Frame{
	/*
	*	关键帧名
	*/
	char _name[G_BUFFER_16_SIZE];
	/*
	*	关键帧索引号
	*/
	int _index;
	/*
	*	索引定义
	*/
	int* _indices;
	//#####################################################
	/*
	*	顶点定义列表
	*/
	float* vertices;
	/*
	*	顶点数量
	*/
	int vertCount;
};
/*
	解析,返回一个md2句柄
*/
void* md2parse_load(const char* str,int len);
/*
	销毁
*/
void md2parse_dispose(void* md2);
/**
*	md2的总的关键帧总数
*/
int md2parse_totalFrames(void* md2);
/**
*	获取指定索引的struct MD2_Frame*引用,index从0开始
*/
void* md2parse_getFrame(void* md2,int index);
/*
	获取顶点个数和,顶点引用
*/
//void md2parse_getFrameVertex(void* md2Frame,float* pVertices,int* pVertCnt);
#endif