**说明**  
基于Opengl的3d引擎,C做的底层接口,Lua封装的组件  


0.编译步骤  
```
common.bat ftfont.bat test.bat
```
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
(9).16:46 2019/2/14 ~~需要实现一个tween渐变接口,上层业务应该用lua隔离出来~~


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
```
	int fontSize = 32;
	unsigned char* rgba = tl_malloc(fontSize*fontSize*4);//像素数据
	*eFormat = GL_BGRA;
	*iComponents = GL_RGBA;
	ft_load(rgba,fontSize,fontSize,iWidth,iHeight,"9");
```
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


bug  
```
local _floor = load_model(func_create_name(),"\\resource\\obj\\plane.obj")		-- func_loadobj('quad',nil,'myObj1',false)--quad
setMaterial(_floor,func_load("//resource//material//font.mat"));	
setv(_floor,FLAGS_VISIBLE);
--setv(_floor,FLAGS_RAY)
setv(_floor,FLAGS_DRAW_RAY_COLLISION)		--BUG 启动的时候会有空指针异常
setv(_floor,FLAGS_DISABLE_CULL_FACE);
```
(17)  
实现一个旋转渐变(主要利用思想是四元数的线性插值实现)  
0<=angle<=PI 所以两个四元数角位移叠加后即得到最终结果  

(18)  
FText相当于继承自Sprite基础上扩展出来的组件  
how to创建一块非2次幂尺寸的贴图,并且用自定义填充像素数据?  

(19)
```
tl_malloc.c
//是否使用自定义的内存管理
#define CUST_MEMORY
```

```
对于0xcccccccc和0xcdcdcdcd，在 Debug 模式下，VC 会把未初始化的栈内存上的指针全部填成 0xcccccccc ，当字符串看就是 “烫烫烫烫……”；会把未初始化的堆内存上的指针全部填成 0xcdcdcdcd，当字符串看就是 “屯屯屯屯……”。那么调试器为什么要这么做呢？VC的DEBUG版会把未初始化的指针自动初始化为0xcccccccc或0xcdcdcdcd，而不是就让取随机值，那是为了方便我们调试程序，如果野指针的初值不确定，那么每次调试同一个程序就可能出现不一样的结果，比如这次程序崩掉，下次却能正常运行，这样显然对我们解bug是非常不利的，所以自动初始化的目的是为了让我们一眼就能确定我们使用了未初始化的野指针了。

对于0xfeeefeee，是用来标记堆上已经释放掉的内存。注意，如果指针指向的内存被释放了，变量变量本身的地址如未做改动，还是之前指向的内存的地址。如果该指针是一个类的指针，并且类中包含有指针变量，则内存被释放后（对于C++类，通常是执行delete操作），类中的指针变量就会被赋值为0xfeeefeee。如果早调试代码过程中，发现有值为0xfeeefeee的指针，就说明对应的内存被释放掉了，我们的代码已经出问题了。

关于VC 中 debug和Release模式下的变量初始化问题

大家都知道，debug跟release在初始化变量时所做的操作是不同的，debug是将每个字节位都赋成0xcc，而release的赋值近似于随机(我想是直接从内存中分配的，没有初始化过)。这样就明确了，如果你的程序中的某个变量没被初始化就被引用，就很有可能出现异常：用作控制变量将导致流程导向不一致；用作数组下标将会使程序崩溃；更加可能是造成其他变量的不准确而引起其他的错误。所以在声明变量后马上对其初始化一个默认的值是最简单有效的办法，否则项目大了你找都没地方找。代码存在错误在debug方式下可能会忽略而不被察觉到，如debug方式下数组越界也大多不会出错，在 release中就暴露出来了，这个找起来就比较难了
```
(20)参考火炬之光 实现一套camera跟随,角色左键常按，角色根据鼠标的方向进行移动  
需要实现四元数角位移渐变  
~~重构UlistBox接口~~  
(21) FBO,PBO实现


