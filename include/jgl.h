#ifndef _JGL_H_
#define _JGL_H_
#include <gl/glew.h>

struct RGB { 
	GLubyte blue;
	GLubyte green;
	GLubyte red;
	//GLubyte alpha;
};
typedef struct RGBA { 
	GLubyte blue;
	GLubyte green;
	GLubyte red;
	//GLubyte alpha;
}RGBA;
/*
	根据路径创建一张贴图
	根据路径中.tga,.bmp创建不同类型的贴图

	返回mTextureID
*/
GLuint 
jgl_loadTex(const char *szFileName);

/*
	创建一个colorType类型的自定义位图

	返回mTextureID
*/
GLuint
jgl_createRGB_Texture(int width,int height,GLuint colorType);


GLuint
jgl_create_opengl_RGBA_Tex(int width,int height,GLenum rgbaTypes);
////////////////////////////////////////////////////////////////////
// 读取tga文件
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas..... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte *jgl_readTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

/*
	更新部分贴图纹理
	const char* imgUrl		:纹理路径
*/
void 
jgl_subImage(GLuint texName,const char* imgUrl,int offsetX,int offsetY,int subImageWidth,int subImageHeight);

/*
 *向texName传递字节流数据更新texture
 **/
void 
jsl_sub(GLuint texName,		//贴图句柄
		GLubyte* pBytes,GLenum eFormat,GLenum type,
		int offsetX,int offsetY,
		int subImageWidth,int subImageHeight);

/*
	创建一块位图内存,并且填充为纯色,这里其实创建的是一块CPU中的缓冲区buffer
*/
GLbyte*
jgl_createRGB_Bitmap(int imageWidth,int imageHeight);

/*
	创建一块rgba位图内存,并且填充为透明色
*/
GLbyte*
jgl_createRGBA(int imageWidth,int imageHeight);
/************************************************************************/
/* 指定内存块内的像素值                                                                     */
/************************************************************************/
void
jgl_drawPontPixelRGB(GLbyte* bytes,int x,int y,GLint w, GLint h,struct RGB* rgb);
#endif