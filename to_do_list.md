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