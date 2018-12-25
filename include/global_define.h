#ifndef _GLOBAL_DEFINE_H_
#define _GLOBAL_DEFINE_H_

#define DEFINE_BASE_64_STRING  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="

#define PI 3.1415926f    //定义π(圆周率180度)
#define EPSILON 0.000001
//===================buffer尺寸定义===================
#define G_CHAR_SIZE_ 10			/*所有的char尺寸 10*/
#define G_BUFFER_64K_SIZE 65536/*64kb*/
#define G_BUFFER_8192_SIZE 8192	/*4096字节缓存区*/
#define G_BUFFER_4096_SIZE 4096	/*4096字节缓存区*/
#define G_BUFFER_2048_SIZE 2048	/*2048字节缓存区*/
#define G_BUFFER_MAX_SIZE 1024	/*1024字节缓冲区*/
#define G_BUFFER_1024_SIZE 1024	/*1024字节缓冲区*/
#define G_BUFFER_512_SIZE 512	/*512字节的数据*/
#define G_BUFFER_256_SIZE 256	/*256字节的数据*/
#define G_BUFFER_128_SIZE 128	/*128字节尺寸的缓存*/
#define G_BUFFER_64_SIZE 64		/*64字节尺寸的缓存*/
#define G_BUFFER_32_SIZE 32		/*32字节尺寸的缓存*/
#define G_BUFFER_16_SIZE 16		/*16字节尺寸的缓存*/
#define G_BUFFER_8_SIZE 8		/*8字节尺寸的缓存*/
#define G_SIGN_POINT '.'		/*符号.*/
#define G_SIGN_SWAY '\\'		/*斜杆/ */
#define G_SIGN_XML_LEFT '<'
#define G_SIGN_XML_RIGHT '>'

#define G_SIGN_SWAY_REVERSE '/'/*	\\/		*/
#define G_SIGN_RIGHT_BRACKETS "}"
#define G_SIGN_LEFT_BRACKETS "{"
#define G_SIGN_QUOTATION "\""	/*双引号*/
#define G_SIGN_COMMA ","		/*逗号*/
#define G_SIGN_HALFWAY_LINE "|"	/*中线*/
#define G_SIGN_NULL ""			/*空字符串*/
#define G_SIGN_SPACE " "		/*空格*/
#define G_SIGN_EQUAL "="		/*等号*/
#define G_SIGN_BR "\n"			/*换行*/
#define G_SIGN_AITA '@'
#define SORT_ASCEND 1	//升序排序
#define SORT_DROP   2	//降序排序

#define EX_SIGN_SIZE 5.0f		//标记的尺寸

#define BOX_SIZE 108		//6面体(12个三角形)包围盒尺寸(float个数)

#define IS_BIG_ENDIAN 	1	//大端
#define IS_SMALL_ENDIAN 0	//小端

//#define ANIM_STR "stand,walk,att,skill01,skill02"

#define  INF    1e9//定义无穷大
#define  EP     1e-9//定义无穷小

#define  TRUE 1
#define FALSE 0

//===================状态码定义=====================

//===================错误码定义=====================

/* 位图位数错误*/
//#define ERR_TEXTURE_BIT 0x03
//#define ERROR_ENGINE_NOT_INITIALIZATION 0	//引擎未初始化

//=================================================================================================
//顶点分布方式
//#define OBJ_UV_VERTEX			0
//#define OBJ_UV_VERTEX_NORMAL	1
enum {
	//OBJ_NULL = -1,
	/*
	 * uv vertex
	 */
	OBJ_UV_VERTEX=0,
	/*
	 * uv vertex normal
	 */
	OBJ_UV_VERTEX_NORMAL=1,
};

//顶点数据间隔数
enum
{
	UV_GAP		=	2,		//uv索引间隔
	NORMAL_GAP	=	3,		//normal索引间隔
	VERTEX_GAP	=	3		//顶点索引间隔
};

//=================================================================================================

//键值枚举
#define KEY_ESC 27
#define KEY_1 49
#define KEY_2 50
#define KEY_A 97
#define KEY_D 100

//文件类型定义
#define TYPE_OBJ_FILE 0	//obj类型
#define TYPE_MD5_FILE 1 //md5类型
#define TYPE_MD2_FILE 2 //md2类型
#define TYPE_SPRITE_FLIE 3//UI界面类型中的sprite
#define TYPE_TEXT_FILE	4	//文本类型
#define TYPE_OBJ_VBO_FILE	5//VBO文件数据

#define  SUFFIX_OBJ "obj"
#define  SUFFIX_MD5MESH "md5mesh"
#define  SUFFIX_MD2 "md2"


/* 文件类型*/
#define TYPE_UNKNOW_FILE -1
#define FILE_FORMAT_STR "obj,md5mesh"
#define TYPE_ERROR_FILE -1//无法识别的类型

#endif