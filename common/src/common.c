#include <stdio.h>     
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>

#define BUILDING_DLL

#include "common.h"
int 
x_md2load(const char* path){
	printf("path = %s\n",path);
	return 1;
}

int 
tl_is_file_exist(const char* file){
	FILE* stream;
	errno_t err=fopen_s( &stream,file, "r" );
	if(err == 0){
		fclose( stream );
		return 1;
	}
	return 0;
}

void 
tl_writeAppandFile(const char* filePath,char* buffer){
	FILE * pFile;
	errno_t err = fopen_s ( &pFile,filePath , "ab+" );
	if(err != 0){
		printf("文件 %s 不存在!\n",filePath);
		assert(0);
		return;
	}
	fwrite (buffer , strlen(buffer), 1, pFile );
	fclose (pFile);
}
static int _nameId;
void 
tl_newName(char* buffer,int bufferSize){
	memset(buffer,0,bufferSize);
	sprintf_s(buffer,bufferSize,"instance%d",_nameId);
	_nameId++;
	dllGlobalVar = _nameId;
}