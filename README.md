**说明**  
基于Opengl的3d引擎,C做的底层接口,Lua封装的组件  
1.优化精简tools.h接口  
2.2019.1.24 vbo模式的渲染的无法拾取,勿忘修复之.  
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
9.16:46 2019/2/14 实现一个tween渐变接口
