/*字节流处理*/
#ifndef _BYTE_H_
#define _BYTE_H_
#include "def.h"

#define IS_BIG_ENDIAN 	1	//大端
#define IS_SMALL_ENDIAN 0	//小端

//字节流读写模块===============================================================================================

/*
	字节流操作处理 
*/
struct BytesVO{
	char* d;										//字节数组数据对象,数据池
	int   mPos;										//写操作 字节游标的位置 0为起始的第一个坐标
	int   length;									//字节的长度	
	int	  bigEndian;								//是否是大端模式1 0
};
/*
Example:

void bs_example()
{
	struct	BytesVO bs,_b;
	int v;
	int len = 128;
	char str[32];
	memset(str,0,32);
	bs_write_start(&bs,len);
	bs_writeByte(&bs,2);
	bs_writeInt(&bs,2339);
	bs_writeShort(&bs,65535);
	bs_writeUTF(&bs,"helloworld!",11);

	bs_read_start(&_b,bs.d,len);
	bs_readByte(&_b,&v);			printf("%d\n",v);
	bs_readInt(&_b,&v);				printf("%d\n",v);
	bs_readShort(&_b,(short*)&v);	printf("%d\n",v);
	bs_readUTF(&_b,str);			printf("%s\n",str);

	bs_dispose(&bs);
	bs_dispose(&_b);
}
*/
//void bs_setbig(struct BytesVO* v,int state);/*设置大端和小端*/
DLLIMPORT void bs_read_start(struct BytesVO* v,char* s,int len,int isBig);/*初始化的时候触发,增加一个长度的字节,用力填充数据*/
DLLIMPORT void bs_writeByte(struct BytesVO*t,char v);/*往字节对象里写入一个byte*/
DLLIMPORT void bs_readByte(struct BytesVO*t,int* dest);/*从字节对象读一个byte*/
DLLIMPORT void bs_readInt(struct BytesVO*t,int* out);/*读取一个int*/
DLLIMPORT void bs_readFloat(struct BytesVO*t,float* out);/*读取一个float*/
DLLIMPORT void bs_readUnsignedLong(struct BytesVO* t,unsigned long* out);/*读取一个无符号的long型数据*/
DLLIMPORT void bs_readShort(struct BytesVO*t,short* out);/*读取一个short*/
DLLIMPORT void bs_readUnsignedShort(struct BytesVO* t,unsigned short *p);/*读取一个无符号的short*/
DLLIMPORT void bs_readUTF(struct BytesVO*,char* dest);/*读取一个字符串*/
DLLIMPORT void bs_writeInt(struct BytesVO*t,int v);/*写一个int*/
DLLIMPORT void bs_writeShort(struct BytesVO* t,short v);/*写入一个short*/
DLLIMPORT void bs_writeUTF(struct BytesVO*t,char* s,int len);/*	写入一个长度为len的数据,写一个字符串,前4个字节(int)存储字符串的长度,字符串的长度信息放在字节数据头部位*/
DLLIMPORT void bs_write_start(struct BytesVO* t,int bufferSize);/*初始化字节流对象*/
DLLIMPORT void bs_writeByteByPos(struct BytesVO* t,int byte,int pos);/*在指定位置写一个Byte*/
DLLIMPORT void bs_writeIntByPos(struct BytesVO*t,int v,int pos);/*在指定位置写一个Int*/
DLLIMPORT void bs_dispose(struct BytesVO*t);/*清除字节数组的内容，并将 length 和 position 属性重置为 0*/
#endif