#ifndef _DEF_H_
#define _DEF_H_
//#define DEBUG//启用debug模式

#ifdef BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

//公共宏部分

//*****************************************************************************
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

#define  INF    1e9//定义无穷大
#define  EP     1e-9//定义无穷小

#define  TRUE 1
#define FALSE 0
//*****************************************************************************



#endif