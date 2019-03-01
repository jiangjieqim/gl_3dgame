//#pragma warning(disable:4996) 
#include <gl/glew.h>
#include <gl/glut.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")

#include "common.h"
#include "str.h"
#include "tl_malloc.h"
#include "gettime.h"
#include "tools.h"
#include "jgl.h"
#include "bmp.h"

//#include "ftfont.h"


//#define _DEBUG_PRINT_RGB_	//打印rgb

#define  RGB_LENGTH 3

typedef struct
{
	GLbyte	identsize;              // Size of ID field that follows header (0)
	GLbyte	colorMapType;           // 0 = None, 1 = paletted
	GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;          // First colour map entry
	unsigned short	colorMapLength;         // Number of colors
	//unsigned char 	colorMapBits;   // bits per palette entry
	unsigned short	xstart;                 // image x origin
	unsigned short	ystart;                 // image y origin
	unsigned short	width;                  // width in pixels
	unsigned short	height;                 // height in pixels
	GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
	GLbyte	descriptor;             // image descriptor
} TGAHEADER;


/************************************************************************/
/* 打印RGB值                                                    */
/************************************************************************/
static void
printfRGB(GLbyte * bytes,int count){
	int i;
	GLbyte* b = bytes;
	int c = RGB_LENGTH;//r g b 3个字节
	struct RGB rgb;
	for(i = 0;i < count;i++){
		memcpy(&rgb,b,c * sizeof(GLubyte));

		printf("i = %d r = %d,g = %d,b = %d\n",i,rgb.red,rgb.green,rgb.blue);

		b = b + sizeof(GLubyte) * c;
	}
}

static GLuint
f_loadImgLs(const char *szFileName,int width,int height,GLuint colorType);
/*
	创建一块rgb位图内存,并且填充为纯色
*/
GLbyte*
jgl_createRGB_Bitmap(int imageWidth,int imageHeight){
	int length = sizeof(struct RGB) * imageWidth * imageHeight;
	GLbyte* bytes = (GLbyte*)tl_malloc(length);
	memset(bytes,0x00,length);
	//printf("%d\n",sizeof(struct RGB));
	return bytes;
}

/*
	创建一块rgba位图内存,并且填充为透明色
*/
GLbyte*
jgl_createRGBA(int imageWidth,int imageHeight){
	int length = sizeof(struct RGBA) * imageWidth * imageHeight;
	GLbyte* bytes = (GLbyte*)tl_malloc(length);
	memset(bytes,0x00,length);
	//printf("%d\n",sizeof(struct RGB));
	return bytes;
}

/*
	在指定位置绘制一个像素点

	一个4x4像素矩阵如下图所示
	0,3		1,3		2,3		3,3
	0,2		1,2		2,2		3,2
	0,1		1,1		2,1		3,1
	0,0		1,0		2,0		3,0
*/
static void
drawPixelRGB(GLbyte* bytes,int x,int y,GLint w, GLint h,struct RGB* rgb){
	if(x>=0 && x<w && y>=0 && y<h){
		int _l = sizeof(struct RGB);
		GLbyte* b = bytes;
		//b = b + x * _l + y * _l;
		b = b + _l * ( x * h + y);
		memcpy(b,rgb,_l);
	}else{
		printf("绘制像素坐标超出范围 %d,%d\n",x,y);
		assert(0);
	}
}
/*
	左手坐标系转化为右手坐标系
*/
static void
convertRightXY(int* px,int* py,int w,int h){
	*py = -(*py) + h - 1;
}
/*
	绘制一个像素点
*/
void
jgl_drawPontPixelRGB(GLbyte* bytes,int x,int y,GLint w, GLint h,struct RGB* rgb){
	convertRightXY(&x,&y,w,h);
	drawPixelRGB(bytes,x,y,w,h,rgb);
}

/************************************************************************/
/* 绘制像素的测试方法                                          


/************************************************************************/
static GLbyte*
drawPixelTest(GLint w, GLint h){
	
	GLbyte* pBits;
	
	pBits = jgl_createRGB_Bitmap(w,h);

	{
		struct RGB rgb;
		rgb.red = 255;
		rgb.green = 0;
		rgb.blue = 0;
		jgl_drawPontPixelRGB(pBits,3,3,w,h,&rgb);
	}
	
	return pBits;
}

GLbyte *
jgl_readTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{

	#ifdef _DEBUG_MODE_
		int _t = get_longTime();
	#endif

	FILE *pFile;			// File pointer
	TGAHEADER tgaHeader;		// TGA file header
	unsigned long lImageSize;		// Size in bytes of image	位图的大小
	short sDepth;			// Pixel depth;
	GLbyte	*pBits = NULL;          // Pointer to bits

	char outStr[G_BUFFER_256_SIZE];

	char* xfile;
	// Default/Failed values
	*iWidth = 0;
	*iHeight = 0;
	*eFormat = GL_RGB;
	*iComponents = GL_RGB;

	{
//#define __FTEXT_TEXT_//测试宏
#ifdef  __FTEXT_TEXT_
		{
		int fontSize = 32;
		unsigned char* rgba = tl_malloc(fontSize*fontSize*4);
		*eFormat = GL_BGRA;
		*iComponents = GL_RGBA;
		ft_load(rgba,fontSize,fontSize,iWidth,iHeight,"国");
		return rgba;
		}
#endif
	}
	
	

	if(tl_convert_path((char *)szFileName,outStr,G_BUFFER_256_SIZE))
		xfile = outStr;
	else
		xfile = (char*)szFileName;

	// Attempt to open the file
	//pFile = fopen(xfile, "rb");
	fopen_s(&pFile,xfile,"rb");
	if(pFile == NULL){
		log_code(ERROR_NOT_EXIST_FILE);
		printf("(%s)加载失败\n",outStr);
		assert(0);
		return NULL;
	}
	
	//printf("%d\n",sizeof(TGAHEADER));
	// Read in header (binary)
	fread(&tgaHeader,  /*sizeof(TGAHEADER)*/18, 1, pFile);

	// Do byte swap for big vs little endian
#ifdef __APPLE__
	LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
	LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
	LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
	LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
	LITTLE_ENDIAN_WORD(&tgaHeader.width);
	LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif


	// Get width, height, and depth of texture
	*iWidth = tgaHeader.width;
	*iHeight = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;

	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return NULL;

	// Calculate size of image buffer
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// Allocate memory and check for success
	pBits = (GLbyte*)tl_malloc(lImageSize * sizeof(GLbyte));
	if(pBits == NULL)
		return NULL;

	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	if(fread(pBits, lImageSize, 1, pFile) != 1)
	{
		tl_free(pBits);
		return NULL;
	}

	// Set OpenGL format expected
	switch(sDepth)
	{
#ifndef OPENGL_ES
		case 3:     // Most likely case
			//3个字节,b g r 储存三种颜色
			*eFormat = GL_BGR;
			*iComponents = GL_RGB;

#ifdef _DEBUG_PRINT_RGB_
			printfRGB(pBits,lImageSize / 3);//打印输出RGB值
#endif

			break;
#endif
		case 4:
			*eFormat = GL_BGRA;
			*iComponents = GL_RGBA;
			break;
		case 1:
			*eFormat = GL_LUMINANCE;
			*iComponents = GL_LUMINANCE;
			break;
		default:        // RGB
			// If on the iPhone, TGA's are BGR, and the iPhone does not 
			// support BGR without alpha, but it does support RGB,
			// so a simple swizzle of the red and blue bytes will suffice.
			// For faster iPhone loads however, save your TGA's with an Alpha!
#ifdef OPENGL_ES
			for(int i = 0; i < lImageSize; i+=3)
			{
				GLbyte temp = pBits[i];
				pBits[i] = pBits[i+2];
				pBits[i+2] = temp;
			}
#endif
			break;
	}

	// Done with File
	fclose(pFile);

	//========================================
	//绘制像素点的测试代码
	//pBits=drawPixelTest(*iWidth,*iHeight);
	//========================================
#ifdef _DEBUG_MODE_
	log_color(0xffffff,"构建纹理 [URL %s]宽高 %d %d,消耗 %ld 毫秒\n",szFileName,*iWidth,*iHeight,(get_longTime()-_t));
#endif

	// Return pointer to image data
	return pBits;
}

///////////////////////////////////////////////////////////////////////////////
// This function opens the "bitmap" file given (szFileName), verifies that it is
// a 24bit .BMP file and loads the bitmap bits needed so that it can be used
// as a texture. The width and height of the bitmap are returned in nWidth and
// nHeight. The memory block allocated and returned must be deleted with free();
// The returned array is an 888 BGR texture
// These structures match the layout of the equivalent Windows specific structs 
// used by Win32
#pragma pack(1)

static int f_loadTGATextureRect(const char *szFileName)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	// 读入纹理位
	pBits = jgl_readTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);

	//{
	//	int i = 0;
	//	struct RGB rgb;
	//	rgb.red = 255;
	//	rgb.green = 0;
	//	rgb.blue = 0;
	//	for(i = 0;i < nWidth;i++){
	//		drawPontPixelRGB(pBits,i,0,nWidth,nHeight,&rgb);
	//	}
	//}










	/*{
		int size = sizeof(*pBits);
		printf("%d\n",size);
	}*/

	if(pBits == NULL) 
		return 0;

	/*
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, wrapMode);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, nComponents, nWidth, nHeight, 0,eFormat, GL_UNSIGNED_BYTE, pBits);
	*/

	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,eFormat, GL_UNSIGNED_BYTE, pBits);

	tl_free(pBits);

	return 1;
}
void
f_loadPicByPath(const char *szFileName){

	#ifdef _DEBUG_MODE_
		int _t = get_longTime();
	#endif

	char suffix[G_BUFFER_16_SIZE];
	tl_getSuffixByPath((char*)szFileName,suffix,G_BUFFER_16_SIZE);
	if(!strcmp(suffix,"tga")){

		if(f_loadTGATextureRect(szFileName)){
			
		}
		else{
			//解析失败
		}
	}
	else if(!strcmp(suffix,"bmp"))
	{
		GLsizei _bmpWidth;
		GLsizei _bmpHeight;			
		GLubyte* image = tl_bmp24(szFileName,&_bmpWidth,&_bmpHeight);
		
		//printf("****bmp %d %d\n",_bmpWidth, _bmpWidth);

		glTexImage2D(GL_TEXTURE_2D,0,3,_bmpWidth,_bmpHeight,0,GL_RGB,GL_UNSIGNED_BYTE,image);
		tl_free((void*)image);

		#ifdef _DEBUG_MODE_
				printf("构建纹理 [URL %s]宽高 %d %d,消耗 %ld 毫秒\n",szFileName,_bmpWidth,_bmpHeight,(get_longTime()-_t));
		#endif
	}
}

//设置位图样式
static void
f_setTex_parameteri(){
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT );
}

GLuint
jgl_createRGB_Texture(int width,int height,GLuint colorType){
	GLuint h = f_loadImgLs((const char*)NULL,width,height,colorType);
	return h;
}

GLuint
jgl_create_opengl_RGBA_Tex(int width,int height){
	GLuint mTextureID = 0;
	//分配一块贴图缓存
	GLbyte* image = jgl_createRGBA(width,height);
	glGenTextures(1,&mTextureID);
	glBindTexture(GL_TEXTURE_2D,mTextureID);
	glTexImage2D(GL_TEXTURE_2D,0,3,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
	f_setTex_parameteri();
	glBindTexture(GL_TEXTURE_2D,0);//解除绑定
	tl_free((void*)image);
	return mTextureID;
}

static GLuint
f_loadImgLs(const char *szFileName,int width,int height,GLuint colorType)
{

	GLuint mTextureID = 0;
	//分配一块贴图缓存
	glGenTextures(1,&mTextureID);
	glBindTexture(GL_TEXTURE_2D,mTextureID);
	
	{
		int index = str_pos(szFileName,".",1);
		if(index < 0){
			log_code(ERROR_SUIFFIX);//路径中没有".",路径有错误
			printf("f_loadImgLs===>(%s)(%d)\n",szFileName,index);
		}
	}

	if(szFileName)
	{
		f_loadPicByPath(szFileName);	
	}
	else
	{
		if(colorType == GL_RGB){
			GLbyte* image = jgl_createRGB_Bitmap(width,height);



		
			{
				
				//测试代码
				int i = 0;
				struct RGB rgb;
				rgb.red = 0;
				rgb.green = 0;
				rgb.blue = 255;
				for(i = 0;i < width;i++){
					jgl_drawPontPixelRGB(image,i,0,width,height,&rgb);
				}
				rgb.red = 0;	rgb.green = 255;	rgb.blue = 0;
				for(i = 0;i < height;i++){
					jgl_drawPontPixelRGB(image,0,i,width,height,&rgb);
				}
				log_color(0xff0000,"Test Code ###");
			}
			



			glTexImage2D(GL_TEXTURE_2D,0,3,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);
			tl_free((void*)image);
		}
		else
		{
			printf("未实现解析colorType = %d 的逻辑!\n" , colorType);
			assert(0);
		}
	}
	f_setTex_parameteri();
	glBindTexture(GL_TEXTURE_2D,0);//解除绑定

	return mTextureID;
}


/*
	根据路径加载纹理	
*/
static GLubyte*  
f_loadImg(const char* szFileName,GLenum* eFormat,GLenum* pType)
{
	GLbyte *pBits=NULL;
	char suffix[G_BUFFER_16_SIZE];
	tl_getSuffixByPath((char*)szFileName,suffix,G_BUFFER_16_SIZE);

	if(!strcmp(suffix,"tga")){
		int nWidth, nHeight, nComponents;
		pBits = jgl_readTGABits(szFileName, &nWidth, &nHeight, &nComponents, eFormat);
	}
	else if(!strcmp(suffix,"bmp"))
	{
		GLsizei _bmpWidth;
		GLsizei _bmpHeight;			
		pBits = tl_bmp24(szFileName,&_bmpWidth,&_bmpHeight);
		*eFormat = GL_RGB;
	}
	*pType = GL_UNSIGNED_BYTE;
	return pBits;
}

void 
jgl_subImage(GLuint texName,const char* imgUrl,
			 int offsetX,int offsetY,int subImageWidth,int subImageHeight)
{
	GLenum eFormat,type;
	GLubyte* pBytes = f_loadImg(imgUrl,&eFormat,&type);
	if(pBytes){
		glBindTexture(GL_TEXTURE_2D, texName);
		glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, 
			subImageWidth,subImageHeight, eFormat,type, pBytes);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		tl_free(pBytes);
	}else{
		printf("纹理(%s)加载失败\n");
		assert(0);
	}
}

void 
jsl_sub(GLuint texName,		//贴图句柄
		GLubyte* pBytes,GLenum eFormat,GLenum type,
		int offsetX,int offsetY,
		int subImageWidth,int subImageHeight)
{
	if(pBytes){
		glBindTexture(GL_TEXTURE_2D, texName);
		glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, 
			subImageWidth,subImageHeight, eFormat,type, pBytes);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		tl_free(pBytes);
	}else{
		log_code(ERROR_BAD_VALUE);//纹理数据不存在
		assert(0);
	}
}

GLuint jgl_loadTex(const char *szFileName){
	return f_loadImgLs(szFileName,0,0,0);
}