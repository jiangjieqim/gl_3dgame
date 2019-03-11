#pragma comment (lib, "freetype.lib")

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include <assert.h>
#include <Windows.h>
//#include "tl_malloc.h"

#define BUILDING_DLL

#include "ftfont.h"

//https://blog.csdn.net/finewind/article/details/38009731

//#define  DEBUG

//#define _DEBUG_DRAW_PIXEL_//是否打印日志

typedef struct FtFont{
	FT_Face face;
	FT_Library library;
}FtFont;
static void 
f_red2rgba(unsigned char* rgba,unsigned char* red, int size){

	int i;
	int buffersize = size * 4 * sizeof(unsigned char);
	//unsigned char* rgba = tl_malloc(buffersize);//new BYTE[4 * size];
	
	
#ifdef _DEBUG_DRAW_PIXEL_
	printf("****************************\n");
#endif
	for(i = 0 ; i < size; i++){
		rgba[i * 4 + 0] = red[i];
		rgba[i * 4 + 1] = red[i];
		rgba[i * 4 + 2] = red[i];
		rgba[i * 4 + 3] = red[i];

#ifdef _DEBUG_DRAW_PIXEL_
		
		printf("i = %d==>\t",i);
		{
			int k;
			for( k = 0;k < 4;k++){
				printf("%d ",rgba[i * 4 + k]);
			}
		}
		printf("\n");
#endif
	}
}
/*
static const wchar_t*
getWC(const char* c){
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc,c,cSize);
	return wc;
}

*/

//char* 转为wchar_t
static void
f_char2w_char(wchar_t* pwsUnicode,int cnt,char* pze){
	int iSize = MultiByteToWideChar(CP_ACP,0,pze,-1,NULL,0);
	if(iSize > cnt){
		printf("缓冲区大小太小!\n");
		assert(0);
	}
	memset(pwsUnicode,0,sizeof(wchar_t) * iSize);
	MultiByteToWideChar(CP_ACP,0,pze,-1,pwsUnicode,iSize);
}

void ft_parse(void* pt,unsigned char* outBuffer,int fontw,int fonth,int *iWidth,int *iHeight,int* iTop,char* str){
	wchar_t ch[2];
	//unsigned char* rgba = 0;
	FtFont* ftext = (FtFont*)pt;
	FT_Face face = ftext->face;
	FT_Error error;	
	f_char2w_char(ch,2,str);

	//字体大小, 有些有fixsize的.fon字体会在这里报错
	error = FT_Set_Pixel_Sizes(face, fontw, fonth);//设置单个字体的宽高
	if (error)
	{		
		printf("error:FT_Set_Pixel_Sizes");
		exit(0);
	}

	//get=========================================================
	{


		//取得字形槽
		FT_GlyphSlot slot = face->glyph;		
		//从字符码检索字形索引
		FT_UInt glyph_index = FT_Get_Char_Index(face, *ch);
		
		//加载到插槽中的字形图像（擦除前一个）
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{		
			printf("error:FT_Load_Glyph");
			exit(0);
		}
		//转换为位图
		if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			if (error)
			{
				printf("error:FT_Render_Glyph");
				exit(0);
			}
		}
		else
		{
			printf("FT_GLYPH_FORMAT_BITMAP (可选的列表中找不到宽高符合要求的文本像素)fw=%d fh=%d\n",fontw,fonth);//fon文件属于bitmap,不能render
			assert(0);
		}

		//提取数据
		{
			unsigned char* buffer	= slot->bitmap.buffer;	//数据
			int width				= slot->bitmap.width;	//宽 像素
			int height				= slot->bitmap.rows;	//高 像素
			int top					= slot->bitmap_top;		//相对横基准线偏移
			int advancex			= slot->advance.x>>6;	//字间水平跨距
			int pitch				= slot->bitmap.pitch;	//宽 byte

			#ifdef DEBUG
				printf("[w = %d h = %d top = %d advancex = %d pitch = %d]\n",width,height,top,advancex,pitch);
			#endif
			f_red2rgba(outBuffer,buffer, width * height);

			//printf("需要的缓冲区%d字节(max:%d)\n",width *  height * 4,fontSize*fontSize*4);

			*iWidth = width;
			*iHeight= height;
			*iTop   = top;
			/*
			m_tex = gen_image(rgba, width, height);
			m_tex_w = width;
			m_tex_h = height;

			printf("texture: %d width = %d height = %d\n",m_tex,m_tex_w,m_tex_h);

			*/
		}
	}
}

void* ft_create(const char* fileName){
	FtFont* pt = malloc(sizeof(FtFont));
	//const char* fileName = "C:\\Windows\\Fonts\\simsun.ttc";
	
	FT_Library library;
	FT_Error error;	
	FT_Face face;
	
	//init=========================================================
	error = FT_Init_FreeType(&library);
	if (error)
	{
		printf("error:FT_Init\n");
		return 0;
	}
	//load=========================================================

	//printf("加载文件:%s\n",fileName);
	error = FT_New_Face(library, fileName, 0, &face);	
	if (error)
	{		
		free(pt);
		printf("未找到文件(%s)\n",fileName);
		printf("error:FT_New_Face\n");//if (error == FT_Err_Unknown_File_Format)MsgBox("FT_Err_Unknown_File");	
		return 0;
	}
	pt->library = library;
	pt->face = face;

	//view======================================================
	//printf("编码方式个数: %d \n", face->num_charmaps);
	#ifdef DEBUG
	{
		int i;
		for(i = 0; i < face->num_charmaps; i++)
		{
			int encoding_id = face->charmaps[i]->encoding_id;
			int platform_id = face->charmaps[i]->platform_id;
			//printf("[%d]: \n encoding_id: %d \n platform_id: %d \n", i, encoding_id, platform_id);
		}
		
		printf("固定尺寸个数: %d -->%d %d[%s]\n", face->num_fixed_sizes,fontw,fonth,str);
		
		for(i = 0; i < face->num_fixed_sizes; i++)
		{
			int width = face->available_sizes[i].width;
			int height = face->available_sizes[i].height;
			
			printf("[%d]: width: %d  height: %d \n", i, width, height);
		}
		printf("字符数: %d \n", face->num_glyphs);
	}
	//set=========================================================
	#endif
	return pt;
}
//销毁
void ft_free(void* pt){
	FtFont* ftext = (FtFont*)pt;
	FT_Done_Face (ftext->face);
	FT_Done_FreeType(ftext->library);
	free(pt);
}

static void 
ft_load(unsigned char* outBuffer,int fontw,int fonth,int *iWidth, int *iHeight,char* str){
	const char* fileName = "C:\\Windows\\Fonts\\simsun.ttc";
	
	FT_Library library;
	FT_Error error;	
	FT_Face face;
	
	unsigned char* rgba = 0;

	int i;
	
	wchar_t ch[2];
	f_char2w_char(ch,2,str);
	
	//init=========================================================
	error = FT_Init_FreeType(&library);
	if (error)
	{
		printf("error:FT_Init\n");
		exit(0);
	}
	//load=========================================================

	//printf("加载文件:%s\n",fileName);
	error = FT_New_Face(library, fileName, 0, &face);	
	if (error)
	{		
		printf("未找到文件(%s)\n",fileName);
		printf("error:FT_New_Face\n");//if (error == FT_Err_Unknown_File_Format)MsgBox("FT_Err_Unknown_File");	
		exit(0);
	}
	//view======================================================
	//printf("编码方式个数: %d \n", face->num_charmaps);

	for(i = 0; i < face->num_charmaps; i++)
	{
		int encoding_id = face->charmaps[i]->encoding_id;
		int platform_id = face->charmaps[i]->platform_id;
		//printf("[%d]: \n encoding_id: %d \n platform_id: %d \n", i, encoding_id, platform_id);
	}
	#ifdef DEBUG
			printf("固定尺寸个数: %d -->%d %d[%s]\n", face->num_fixed_sizes,fontw,fonth,str);
	#endif

	for(i = 0; i < face->num_fixed_sizes; i++)
	{
		int width = face->available_sizes[i].width;
		int height = face->available_sizes[i].height;
		#ifdef DEBUG
			printf("[%d]: width: %d  height: %d \n", i, width, height);
		#endif
	}
	#ifdef DEBUG
		printf("字符数: %d \n", face->num_glyphs);
	#endif
	//set=========================================================

	//字体大小, 有些有fixsize的.fon字体会在这里报错
	error = FT_Set_Pixel_Sizes(face, fontw, fonth);//设置单个字体的宽高
	if (error)
	{		
		printf("error:FT_Set_Pixel_Sizes");
		exit(0);
	}
	//get=========================================================
	{
		//取得字形槽
		FT_GlyphSlot slot = face->glyph;		
		//从字符码检索字形索引
		FT_UInt glyph_index = FT_Get_Char_Index(face, *ch);
		
		//加载到插槽中的字形图像（擦除前一个）
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{		
			printf("error:FT_Load_Glyph");
			exit(0);
		}
		//转换为位图
		if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			if (error)
			{
				printf("error:FT_Render_Glyph");
				exit(0);
			}
		}
		else
		{
			printf("FT_GLYPH_FORMAT_BITMAP (可选的列表中找不到宽高符合要求的文本像素)fw=%d fh=%d\n",fontw,fonth);//fon文件属于bitmap,不能render
			assert(0);
		}

		//提取数据
		{
			unsigned char* buffer	= slot->bitmap.buffer;	//数据
			int width				= slot->bitmap.width;	//宽 像素
			int height				= slot->bitmap.rows;	//高 像素
			int top					= slot->bitmap_top;		//相对横基准线偏移
			int advancex			= slot->advance.x>>6;	//字间水平跨距
			int pitch				= slot->bitmap.pitch;	//宽 byte

			#ifdef DEBUG
				printf("[w = %d h = %d top = %d advancex = %d pitch = %d]\n",width,height,top,advancex,pitch);
			#endif
			f_red2rgba(outBuffer,buffer, width * height);

			//printf("需要的缓冲区%d字节(max:%d)\n",width *  height * 4,fontSize*fontSize*4);

			*iWidth = width;
			*iHeight= height;
		}

		//finish=======================================================
		//system("pause");			
		FT_Done_Face (face);
		FT_Done_FreeType(library);
	}
}