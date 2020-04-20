**说明**  
基于Opengl的3d引擎,C做的底层接口,Lua封装的组件  

![image](https://github.com/jiangjieqim/gl_3dgame/blob/master/%E7%A0%94%E5%8F%91%E6%96%87%E6%A1%A3/%E9%A2%84%E8%A7%88%E5%9B%BE0.png)  

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
关于VC 中 debug和Release模式下的变量初始化问题大家都知道，debug跟release在初始化变量时所做的操作是不同的，debug是将每个字节位都赋成0xcc，而release的赋值近似于随机(我想是直接从内存中分配的，没有初始化过)。这样就明确了，如果你的程序中的某个变量没被初始化就被引用，就很有可能出现异常：用作控制变量将导致流程导向不一致；用作数组下标将会使程序崩溃；更加可能是造成其他变量的不准确而引起其他的错误。所以在声明变量后马上对其初始化一个默认的值是最简单有效的办法，否则项目大了你找都没地方找。代码存在错误在debug方式下可能会忽略而不被察觉到，如debug方式下数组越界也大多不会出错，在 release中就暴露出来了，这个找起来就比较难了
```
(20)参考火炬之光 实现一套camera跟随,角色左键常按，角色根据鼠标的方向进行移动  
需要实现四元数角位移渐变  
~~重构UlistBox接口~~  
(21)FBO,PBO实现  
在texture上渲染一个视图
![image](https://github.com/jiangjieqim/gl_3dgame/blob/master/resource/texture/tri.png)


(22)1000000次随机,c使用14毫秒,lua47毫秒,所以lua是存在性能损耗的  

```
int i = 0;
int t = get_longTime();
for(i = 0;i < 1000000;i++){
	rand();
}
printf("%ld ms\n",get_longTime()-t);//14 ms
```

```
local t = func_get_longTime();
for i=1,1000000,1 do
    math.random();
end
print(func_get_longTime()-t); --47 ms
```
(23)
lua继承的实现,resize之后的碰撞盒的的修正

## (24)lua的类实现
```
Account = {}
function Account:add1()
    if(self.value == nil) then
    else
        self.value = self.value + 2;
    end
    return self.value;
end
function Account:new(o)
    o = o or {}   -- create object if user does not provide one
    setmetatable(o, self)
    self.__index = self
    o:add1();--这里不用self
    return o
end
local a = Account:new({value = 1});
local b = Account:new({value = 2});
a:add1();
print(a.value);
print(b.value);
--输出5,4
```

## (25)UI 拖动抖动问题解决2019/8/17
因为拖动会有enterframe的事件回调所以在lua部分会卡顿的,所以将render loop放在C部分,如果在lua里循环逻辑，会出现掉帧的问题.

## (26)引擎初始化的设置UI和3d层通过pos_z值来处理,z越大越在上层,其实界面层和3d层是在一个渲染循环里面的
```
void 
ex_init(struct EX* p,GLdouble zfar){
	p->allzBuffer = -90000;	//初始化一个Z深度,此深度标识3d层的
	p->ui_pos_z =  -1000;	//此深度如果小于3d层,那么界面将在3d界面后面
	...
}
	
```
## 关于mask list的使用方法(scrollview)
```
local itemHeight = 30;
local cam;
local function f_create()
	local sprite = sprite_create(nil,0,0,99,itemHeight,0,1,cam);

    local node ={};
--	setv(sprite,FLAGS_DRAW_PLOYGON_LINE);
    func_setIcon(sprite, "smallbtn.png");
    engine_addNode(sprite);


	if(true) then
		local tf = ftext_create(64,64,13,12,cam);
		local con = ftext_get_container(tf);
		func_addchild(sprite,con);
		node.tf = tf;
	end
	node.view = sprite;
	--setv(sprite,FLAGS_DRAW_PLOYGON_LINE);

    node.data = nil;
	
    node.used = 1;
	node.index = nil;
   
    return node;
end

--刷新视图
local function itemRefreshFunc(node)
--    print(node.data);
	if(node.tf) then
		fext_clear(node.tf);
		local str = string.format("i = %d,__%d",node.index,node.data);
		ftext_parse(node.tf,str);
	end
	--print("index",node.index,"刷新视图,设置数据",node.data);
end

local t = {}; --{10,20,30,40,50,60,70,80,90,100,110,120};


for i = 1,20,1 do
	t[i] = i;
end
--print(#t);--table的长度

local sv = scrollView_init(100,itemHeight*4,20,30);
sv.itemFunc = f_create;--设置itemRende的创建回调
sv.itemRefreshFunc = itemRefreshFunc;
cam = scrollView_get_cam(sv);
scrollView_set_data(sv,t);
```
## 灰度滤镜ps
```
	vec4 finalColor=texture2D(texture1, result);
	float v = dot(finalColor.rgb,vec3(0.3,0.59,0.11));
	gl_FragColor = vec4(v,v,v,1.0);
```

## resload创建tex的lua测试代码
```
--点击一次按钮,创建一个tex对象
local arr = {};
arr[0] = "gundi.png";
arr[1] = "arrow3.png";
arr[2] = "checkbox.png";
arr[3] = "dagou.png";
local a = 0;
local function f_callback1(data,obj)
	print("******************************加载结束",data,obj);
	local spr = sprite_create_9grid(data,150,(a-1)*50,200,50,0,3,3,3,3);
	engine_addNode(spr);
end
local function f_onClick()
	evt_once(0,EVENT_ENGINE_TEX_LOAD_COMPLETE,f_callback1,0);
	load_tex(arr[a]);
	print(arr[a]);
	a = a + 1;
end
local btn = btn_create(0,50);
btn_bindClick(btn,f_onClick);
```


## 根据xml解析界面
```
<ui name="1" type="Panel" drag="1" center="1" width="200" height="250"/>
<ui name="rx_sc" type="ScrollBar" x="0" y="0" parent="1"/>
<ui name="rx_label" type="Label" x="100" y="0" label="rx" parent="1"/>
```
参考接口function skin_parse(skin)  

### include/lua文件夹下的说明
/..	引擎组件模块  
view/..	业务模块  


dll编译  
```
#define MYDLL __declspec(dllexport)

MYDLL double Add2(double a, double b){
    return a + b;
}
```
cl -c Math.c  
link -DLL -out:Math.dll Math.obj  
检查一下导出的函数列表  
dumpbin -exports Math.dll  



### 5配置lua开发环境
[https://marketplace.visualstudio.com/items?itemName=actboy168.lua-debug]  
```
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "lua",
            "request": "launch",
            "name": "launch_test_exe",
            "port": 7003,
            "runtimeExecutable": "${workspaceFolder}/test.exe",
        },
    ]
}
```

###  TortoiseProc.exe提交的CMD
提交文件README.md和to_do_list.md   
```
set PATH=C:\Program Files\TortoiseSVN\bin;
start
TortoiseProc.exe /command:commit /path:README.md*to_do_list.md /closeonend:1
taskkill /f /im cmd.exe
exit
```

### glFiush和glFinish
glFlush是异步的，发送完成之后立即返回的,glFinish要等待GPU执行完成之后再返回的  
glFinish是阻塞模式的  


