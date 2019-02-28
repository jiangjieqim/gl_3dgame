**说明**  
基于Opengl的3d引擎,C做的底层接口,Lua封装的组件  
1.优化精简tools.h接口  
2.2019.1.24 **vbo模式的渲染的无法拾取,勿忘修复之.**  
3.为3d对象添加一个接口targetToVec(x,y);让对象转向一个指向向量.  
4.可以参考[ www.3dgamestudio.com] 的接口设计引擎.  
5.20:04 2019/1/29	example01:实现一个点击地面旋转角色移动到指定目的地.  
6.12:52 2019/1/30	VBO模式的拾取有bug!  
7.21:06 2019/2/13	目前只在diffuse.vs中实现了uvScale,后续要优化重构一下,对多个着色器共享变量  
8.9:52 2019/2/14to do list:  
材质独立出成一个配置,通用数据类型封装出来  
一个文件结构由xml结构组织节点表示一个数据,一个数据用自己的后缀(如.mat,.exobject),由lua实现这样的上层逻辑  
```
材质对象:
<mat name='mat1' shader='diffuse' shaderParm='uvScale:2|_LineWidth:2' tex='1.tga|2.tga'/>
引擎对象:
<exobject name='model1' modelurl='md2/monster.md2' mat='mat1'/>
```  
(9).16:46 2019/2/14 需要实现一个tween渐变接口,上层业务应该用lua隔离出来


(10).1:26 2019/2/15存疑的记录代码片段  
```
int ex_zBuffer()
{
	struct EX* p = ex_getInstance();
	//return p->zBuffer++;//这里存疑,UI正交zbuffer叠加的影响
	return -1;
}

```

(11)23:47 2019/2/15 
  config.xml 为ANSI格式  
  debug模式下esc退出空指针异常的bug  
(12)8:51 2019/2/20 分割层common.dll(公共组件层)和excore.dll(引擎层) exrender(渲染层) 

(13)14:50 2019/2/21 
查看dll函数列表  
prompt  
dumpbin /exports C:\gl_3dgame\common\Debug\common.dll  
项目common添加宏BUILDING_DLL  

warning C4312: "类型转换" 
属性->c++->常规->检查64位可移植问题->-"否" 

(14)11:29 2019/2/23 将业务代码剥离到lua模块,集成污染

(15)获取文件大小
```
@echo off  
::提取txt、doc、xls文件总大小 
for /f "delims=" %%a in ('dir /b *.txt *.doc *.xls') do set /a "s+=%%~za/1024"
echo,%s%KB
pause
```
(16)########################################################################  
2019.2.28freetype字体  
*1.设计一个ftext接口渲染freetype生成的文本数据*  
*2.cope像素填充到一个texture中*  
char* 转为wchar_t  
```
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
```
