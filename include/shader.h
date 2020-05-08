#ifndef _SHADER_H_
#define _SHADER_H_
#define SHADER_VBO_SIMPLE 1 

////font1文本着色器上传数据到GPU的回调
//void 
//font1_updateVarCallback(void* material,Matrix44f M,void* param,void* cam);

//9宫格着色器回调
void
grid9CallBack(void* material,Matrix44f M,void* param,void* cam);

//SpriteVBO着色器
void
vboSimpleCallBack(void* material,Matrix44f M,void* param,void* cams);

//线框渲染着色器
void 
vboLineCallBack(void* material,Matrix44f M,void* param,void* cam);
#endif