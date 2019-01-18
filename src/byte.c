#include "tools.h"
#include "byte.h"

#define T struct BytesVO
//获取一个char数据
static char gc(T* v,int pos){
	return v->d[pos + v->mPos];
}
//为T增加一个字节
static void add(T* v,char ch){
	//	memcpy(v->d+v->length,&ch,sizeof(char));
	v->d[v->length] = ch;
	v->length++;
}

//初始化的时候触发,增加一个长度的字节,用来填充数据
void bs_read_start(T* v,char* s,int len,int isBig){
	/*
	int bigEndian;
	#ifdef BYTES_BIG_ENDIAN 
	bigEndian = 1;
	#else	
	bigEndian = 0;
	#endif
	*/

	v->mPos = -1;
	v->bigEndian = isBig;
	v->length=len;

	//printf("是否是大端:%d\n",v->bigEndian);

	v->d = (char*)tl_malloc(len+1);	
	//	memset(v->d,0,len+1);
	memcpy(v->d,s,len);
}
//往字节对象里写入一个byte
void bs_writeByte(T*t,char v){
	char tmp = (char)v&(0xff);
	add((T*)t,tmp);
	t->mPos+=sizeof(char);
}
//从字节对象读一个byte
void  bs_readByte(T*t,int* dest){	
	int i = sizeof(char);
	t->mPos+=i;
	*dest=gc(t,1-i)&0xff;
}
//写一个int
void  bs_writeInt(T*t,int v){		
	add(t,(char)((v&(0xff0000<<0x8))>>0x18));
	add(t,(char)((v&0xff0000)>>0x10));
	add(t,(char)((v&0xff00)>>0x08));
	add(t,(char)(v&0xff));
	t->mPos+=sizeof(int);
}
//读取一个int
void bs_readInt(T*t,int* out){
	int i = sizeof(int);
	int _p0[4] = {4,3,2,1};
	int _p1[4] = {1,2,3,4};
	int _p2[4];
	t->mPos+=i;
	if(t->bigEndian == 1)	memcpy(_p2,_p0,i*i);
	else					memcpy(_p2,_p1,i*i);
	*out = gc(t,_p2[0]-i)<<0x18&0xff000000 | gc(t,_p2[1]-i)<<0x10&0x00ff0000 | gc(t,_p2[2]-i)<<0x08&0x0000ff00 | gc(t,_p2[3]-i)& 0x000000ff;
	//memcpy(out,&res,sizeof(int));
	//*out=res;
}

//读取一个float
void bs_readFloat(T*t,float* out)
{
	/*
	int i = sizeof(float);
	int _p0[4] = {4,3,2,1};
	int _p1[4] = {1,2,3,4};
	int _p2[4];
	t->mPos+=i;
	if(t->bigEndian == 1)	memcpy(_p2,_p0,i*i);
	else					memcpy(_p2,_p1,i*i);
	*out = gc(t,_p2[0]-i)<<0x18&0xff000000 | gc(t,_p2[1]-i)<<0x10&0x00ff0000 | gc(t,_p2[2]-i)<<0x08&0x0000ff00 | gc(t,_p2[3]-i)& 0x000000ff;
	*/
	union ByteU
	{
		float v;
		char c[4];
	}u;
	int k;
	int i = sizeof(float);


	if(t->bigEndian)
	{
		for(k = 0;k<i;k++)
		{
			u.c[k] = t->d[t->mPos+k+1];
		}
	}
	else
	{
		for(k = i;k>0;k--)
		{
			u.c[k] = t->d[t->mPos+k+1];
		}
	}
	//printf("%f[%d %d %d %d]\n",u.v,t->d[t->mPos+1],t->d[t->mPos+2],t->d[t->mPos+3],t->d[t->mPos+4]);
	*out = u.v;
	t->mPos+=i;
}



void bs_readUnsignedLong(T* t,unsigned long* out){
	unsigned long p;
	bs_readInt(t,(int*)&p);
	*out=p;
}
void bs_writeShort(T* t,short v){
	//	add(t,(v&(0xff0000<<0x8))>>0x18);
	//	add(t,(v&0xff0000)>>0x10);
	add(t,(char)((v&0xff00)>>0x08));
	add(t,(char)(v&0xff));
	t->mPos+=sizeof(short);
}
void bs_readShort(T*t,short* out){
	int i = sizeof(short);
	int p[2];
	int p1[2] = {2,1};
	int p0[2] = {1,2};
	t->mPos+=i;
	if(t->bigEndian == 1)	memcpy(p,p1,sizeof(int)*i);
	else					memcpy(p,p0,sizeof(int)*i);
	*out = gc(t,p[0]-i)<<0x08&0x0000ff00 | gc(t,p[1]-i)& 0x000000ff;
}
void bs_readUnsignedShort(T* t,unsigned short *p){
	bs_readShort(t,(short *)&p);
}
//写一个字符串,前4个字节(int)存储字符串的长度,字符串的长度信息放在字节数据头部位
void  bs_writeUTF(T*t,char* s,int len){	
	bs_writeInt(t,len);	
	memcpy(t->d+t->mPos+1,s,len);
	t->length+=len;
	t->mPos+=len;
}
//读取一个字符串(版本2)
void  bs_readUTF(T*t,char* dest){
	int len;
	bs_readInt(t,&len);	
	memset(dest,0,len+1);
	memcpy(dest,t->d+t->mPos+1,len);
	t->mPos+=len;
}
//初始化字节流对象
void bs_write_start(T* t,int bufferSize){
	t->d=0;
	t->length=0;
	t->mPos=-1;
	t->d = (char*)tl_malloc(bufferSize);
	memset(t->d,0,bufferSize);
}
//在指定位置写一个Byte
void bs_writeByteByPos(T* t,int byte,int pos){
	if(pos <= t->length-(int)sizeof(char)){
		t->d[pos] = byte&0x000000ff;
	}
}
//在指定位置写一个Int
void bs_writeIntByPos(T*t,int v,int pos){
	if(pos <= t->length-(int)sizeof(int)){
		t->d[pos]  = (v&0xff000000)>>0x18;
		t->d[pos+1]= (v&0x00ff0000)>>0x10;
		t->d[pos+2]= (v&0x0000ff00)>>0x08;
		t->d[pos+3]= v&0x000000ff;
	}
}
//清除字节数组的内容，并将 length 和 position 属性重置为 0
void  bs_dispose(T*t){
	if(t->d != 0){
		tl_free(t->d);
		t->d = 0;
	}
	t->d=0;
	t->length=0;
	t->mPos=-1;
}