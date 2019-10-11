#include "tlgl.h"
#include "camera.h"
#include "ex.h"

struct GM_Font 
{
	GLuint program3D;
	GLint matrix;
	GLint ui_perspectivePtr;
};

void 
font1_updateVarCallback(void* material,Matrix44f M){
	struct GMaterial* gm =  (struct GMaterial*)material;
	struct GM_Font* f;
	if(!gm->shaderVars){
		//这里初始化的时候只处理一次,在帧循环中其实是不需要每次都取GLSL中的共享变量的
		//每次都取是代价高昂的,所以这里优化处理,对每一个Shader都要做这样的优化

		GLuint program3D = ex_getProgrom3D(gm->glslType);
		//获取第1个矩阵引用
		GLint _matrix4x4 = glGetUniformLocation(program3D,"_mat1");

		GLint ui_perspectivePtr = glGetUniformLocation(program3D,"ui_PerspectiveMatrix4x4");
		gm->shaderVars = tl_malloc(sizeof(struct GM_Font));
		f = (struct GM_Font*)gm->shaderVars;
		f->matrix = _matrix4x4;
		f->program3D = program3D;
		f->ui_perspectivePtr = ui_perspectivePtr;
	}
	f = (struct GM_Font*)gm->shaderVars;
	glUseProgram(f->program3D);
	tmat_updateTexture(f->program3D,gm);

	tmat_uploadMat4x4(f->matrix,M);

	//上传2d齐次坐标矩阵
	tmat_uploadMat4x4(f->ui_perspectivePtr,cam_getPerctive(ex_getIns()->_2dCurCam));	
}

struct GM_Grid9
{
	GLuint program3D;
	GLint matrix;
	GLint ui_perspectivePtr;
	GLint ui_modelViewPtr;
	GLint top;
};

void
grid9CallBack(void* material,Matrix44f M){
	struct GMaterial* gm =  (struct GMaterial*)material;
	struct GM_Grid9* f;
	if(!gm->shaderVars){

		GLuint program3D = ex_getProgrom3D(gm->glslType);
		//获取第1个矩阵引用
		GLint _matrix4x4 = glGetUniformLocation(program3D,"_mat1");

		GLint ui_perspectivePtr = glGetUniformLocation(program3D,"ui_PerspectiveMatrix4x4");
		
		GLint ui_modelViewPtr = glGetUniformLocation(program3D,"ui_ModelViewMatrix4x4");

		gm->shaderVars = tl_malloc(sizeof(struct GM_Grid9));
		f = (struct GM_Grid9*)gm->shaderVars;
		f->matrix = _matrix4x4;
		f->program3D = program3D;
		f->ui_perspectivePtr = ui_perspectivePtr;
		f->ui_modelViewPtr = ui_modelViewPtr;

		f->top = glGetUniformLocation(program3D,"top");
			
		printf("%d,",f->top);
		
	}
	f = (struct GM_Grid9*)gm->shaderVars;
	glUseProgram(f->program3D);
	tmat_updateTexture(f->program3D,gm);

	tmat_uploadMat4x4(f->matrix,M);
	//上传2d齐次坐标矩阵
	tmat_uploadMat4x4(f->ui_perspectivePtr,cam_getPerctive(ex_getIns()->_2dCurCam));	
	tmat_uploadMat4x4(f->ui_modelViewPtr,cam_getModel(ex_getIns()->_2dCurCam));

	glUniform1f(f->top,2.0f);	

}
