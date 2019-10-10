#include <gl/glew.h>
#include "progrom3D.h"
#include "tools.h"
#include "tlgl.h"


struct ShaderNode{
	char* name;
	GLuint progrom;
};

struct GProgram3d
{
	struct LStackNode* list;
};

void*
progrom3d_create(){
	struct GProgram3d* ptr = (struct GProgram3d*)tl_malloc(sizeof(struct GProgram3d));
	ptr->list = LStack_create();
	return ptr;
}
//void
//progrom3d_remove(void* _ptr,const char* shaderName){
//	struct GProgram3d* ptr = (struct GProgram3d*)_ptr;
//	struct LStackNode* s = ptr->list;
//
//	struct ShaderNode* shaderNode;
//	void* top,*p;
//	top = s;
//	p=top;
//	while((int)LStack_next(p)){
//		int data;
//		p=(void*)LStack_next(p);
//		data = LStack_data(p);
//
//		shaderNode = (struct ShaderNode*)data;
//		if (!strcmp(shaderNode->name,shaderName))
//		{
//		//	return shaderNode->progrom;
//			shaderNode->cnt--;
//			if(shaderNode->cnt<=0){
//
//			}
//			break;
//		}
//	}
//}
void*
progrom3d_get(void* _ptr,const char* shaderName){
	struct GProgram3d* ptr = (struct GProgram3d*)_ptr;
	struct LStackNode* s = ptr->list;
	
	struct ShaderNode* shaderNode;
	void* top,*p;
	top = s;
	p=top;
	while((int)LStack_next(p)){
		int data;
		p=(void*)LStack_next(p);
		data = LStack_data(p);

		shaderNode = (struct ShaderNode*)data;
		if (!strcmp(shaderNode->name,shaderName))
		{
			//shaderNode->cnt+=1;
			return (void*)shaderNode->progrom;
		}
	}
	
	shaderNode=(struct ShaderNode*)tl_malloc(sizeof(struct ShaderNode));
	shaderNode->name = tl_malloc(strlen(shaderName)+1);
	memset(shaderNode->name,0,strlen(shaderName)+1);
	memcpy(shaderNode->name,shaderName,strlen(shaderName));

	tlgl_glslProgrom(&(shaderNode->progrom),shaderName);
	LStack_push(ptr->list,(void*)shaderNode);
	//printf("*****%s\n",shaderName);

	return (void*)shaderNode->progrom;
}