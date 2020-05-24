### 需求文档  
MD格式教程[https://blog.csdn.net/zuggs_/article/details/82381269] 
1.GLSL实现GLOW辉光[https://blog.csdn.net/jaccen2012/article/details/78215066]  
```
原理:只要对要添加辉光效果的物体进行一次模糊操作，然后将模糊过后的图片与原先的图片进行Alpha Blend
```
### H5相关
```
cd \workspace_client_h5_chenyuan2\chenyuan2\
git pull -v --progress "origin"
d:
cd \workspace_client_h5_chenyuan3\chenyuan3\
git pull -v --progress "origin"
d:
cd \workspace_client_h5_xuanyuanjian2\xuanyuanjian2\
git pull -v --progress "origin"
d:
cd \workspace_client_h5_ghc\ghc\
git pull -v --progress "origin"
```
=======
### 需求文档  
MD格式教程[https://blog.csdn.net/zuggs_/article/details/82381269]  
### GLSL实现GLOW辉光  
[https://blog.csdn.net/jaccen2012/article/details/78215066]  
```
原理:只要对要添加辉光效果的物体进行一次模糊操作，然后将模糊过后的图片与原先的图片进行Alpha Blend
```
### 实现阴影

localToGlobal

### 实现fbo下的sprite交互
1.List的Scroll的mask遮罩的处理

### Opengl性能优化
1.将getUniform的值存起来,这样不用每次去着色器对象中频繁获取,因为频繁获取是代价高昂的,因为上切换设备上下文

### scrollview使用对象池优化之(已解决)

### list.sort 其实是冒泡排序,实现一个LStackNode的.sort回调接口

### 用xml配置实现界面的组合!!!

21:15 2019/11/22

### lua里面 try catch异常

### LStack_ergodic修改为实现变参的接口

### 如果input的文本发生变化 就立即更新input的游标坐标

### 实现一个类似魔兽争霸3的地图编辑器

### 像VsCode 这样的架构应该多学习之
1.检查下lua模块中是否频繁使用字符串拼接  
2.使用微核模式(插件模式)对系统进行扩展  
3.做出各种DLL插件  
4.加入pool这样的内存池管理对象  
5.2020.4.1将lua的内核和应用层剥离开来,分离出引擎层和业务层  
6.实现一个lua的json的解析类  
7.全局搜tostring的调用,处理掉  
8.实现onDisplay和unDisplay  
9.将很多可以一改成Lua的C模块改成Lua的 
10.用Line绘制各种立方盒子  
11.button穿透的时候还有点击效果  
12.构造shader,剔除掉glslType  
13.材质直接引用矩阵空间  
14.余以为尽可除去固定管线不用..
15.将相同图集的Sprite合并到一个网格
16.将Sprite按下效果处理到button.lua模块中  
17.直接修改Sprite的顶点值来处理x,y,w,h
18.BUG: bauul.mat的 cb="1"的时候 <mat shader="simple;vboSimple" cb="10" tex0="\resource\texture\bauul.tga"/>
19.将Sprite分成点击拾取模块