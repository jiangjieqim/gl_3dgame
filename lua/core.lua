
FLAGS_RENDER_BOUND_BOX	=	0x01;
FLAGS_DRAW_RAY_COLLISION	=	0x02;	--绘制射线盒子	(是否绘制射线静态包围盒,用于测试 查看射线包围盒),即使没有设置FLAGS_RAY,那么也是可以绘制射线盒子的,这样可以用来查看
FLAGS_LINE_RENDER			=	0x04;	--是否显示渲染线框
FLAGS_OUTLINE				=	0x08;	--是否显示边缘轮廓
FLAGS_RAY					=	0x10;	--16是否具有射线拾取功能(此只用于3d拾取,2d拾取不用这个标记)
FLAGS_VISIBLE				=	0x20;	--32显示或者隐藏模型,(是否加入渲染队列中)
FLAGS_RENDER_DRAWSKELETON	=	64;		--渲染骨骼节点(md5模型才会显示)
FLAGS_ANIM_ADAPTIVE		=	128;		--是否根据关键帧的帧率适配关键帧的动画(是否适配fps 1,适配  0,不适配)
FLAGS_GLSL_OUTLINE		=	256;		--用GLSL实现的轮廓线
FLAGS_DRAW_PLOYGON_LINE	=	512;		--在第一个基本材质上用线框渲染(固定管线模式设置GL_LINE)
FLAGS_BUTTON_EFFECT		=	1024;	--设置按钮特效(在有点击事件的前提下才会有该特效)
FLAGS_DRAW_NORMAL 		= 	2048;	--法线
FLAGS_DRAG				=	4096;	--是否可拖拽
FLAGS_DRAW_PLOYGON_POINT= 	8192;	--点模式
FLAGS_DISABLE_CULL_FACE	=	16384;	--设置显示双面
FLAGS_REVERSE_FACE		=	32768;	--反转渲染面,只有设置了双面渲染才有效
--[[
--//文件类型定义
TYPE_OBJ_FILE = 0--	//obj类型
TYPE_MD5_FILE =1 --//md5类型
TYPE_MD2_FILE =2 --//md2类型
TYPE_SPRITE_FLIE =3--//UI界面类型中的sprite
--TYPE_TEXT_FILE	=4	--//文本类型
TYPE_OBJ_VBO_FILE=	5--//VBO文件数据

SUFFIX_OBJ ="obj"
SUFFIX_MD5MESH ="md5mesh"
SUFFIX_MD2 ="md2"

--]]

------------------------------------------------------------
EVENT_RAY_PICK = 1						--拾取点击回调
EVENT_ENGINE_RENDER_3D =100				--引擎渲染回调
EVENT_ENGINE_KEYBOARD  =101				--全局键盘事件
EVENT_ENGINE_RESIZE	   =102				--resize事件

EVENT_ENGINE_BASE_UPDATE	 =  102		--base更新事件
EVENT_ENGINE_BASE_END		 =  103		--base结束事件
EVENT_ENGINE_SPRITE_CLICK = 104
EVENT_ENGINE_SPRITE_CLICK_DOWN = 105
EVENT_ENGINE_SPRITE_CLICK_MOVE = 106    --click move
EVENT_ENGINE_TEX_LOAD_COMPLETE = 108	--纹理加载结束
EVENT_ENGINE_COMPLETE		   = 109	--完成事件

CUST_LUA_EVENT_SPRITE_FOCUS_CHANGE =110 --向lua层发送焦点变化
CUST_LUA_EVENT_INPUT_CHANGE = 111		--input输入内容发生变化

EVENT_TIMER = 201--计时器

ENGINE_EVENT_COMPLETE = 1000;	--结束事件

--键值枚举
 KEY_ESC = 27
 KEY_1 = 49
 KEY_2 = 50
 KEY_A = 97
 KEY_B = 98
 KEY_C = 99
 KEY_D = 100
 KEY_I = 105

--界面类型
UI_TYPE = {
	Label = 1,
	Button =2,
	ScrollBar = 3,
	Panel = 4,
	Skin = 5,
	CheckBox = 6,--选项卡类型
	ProgressBar = 7,--进度条
	ListBox = 8,--下拉列表
	Input = 9,--输入组件
	Image = 10,--image组件
	Shape = 11,--shape组件
	NScrollBar = 12,--滚动条组件
	NListBox = 13,--列表
	NLabel = 14,--Label
	NPanel = 15,
	NButton = 16,
};

local DEBUG = true;


require("stack")
require("xml")	--xml组件
require("vec3")	--自定义数学库
require("nstack");--链栈

require("evt")	--事件管理器


--转化出一个地址
--"=table: 0082E758"  ===>8578904
local function getDddress(value)
	local len = string.len("table:  ")
	local a = string.len(tostring(value))
	local v = string.sub(tostring(value),len,a)
	local s = tonumber('0x'..v);
	return s;
end
--是否开启debug模式
function func_enable_debug(v)
	if(v == true)then
		DEBUG=1;
		log_enable(1);
	else
		DEBUG=nil;
		log_enable(0);
	end
end
--将"table: ff"转化为number
function func_get_address(value)
	return getDddress(value);
end


--local nameKey = -1;
--生成一个名字
local function getName(suffix)
    local str = tl_newName(suffix);
    return str;
end
--[[
	根据一个配置加载生成一个数据对象

--]]
function func_load_material(url)
	
	local suffix = get_suffix(url);
	--print(s);
	local result;
	local xml = Xml:new();--xml_load(url);
	xml:load(url);
	if(suffix == "mat") then
		local node = xml:get_index(0);--xml_get_node_by_index(xml,0);
		local shader = xml_get_str(node,"shader");
		
		if(shader == nil) then
			func_error("shader is nil,"..url);
		else			
			if(shader == "") then
				func_error(url);
			end
			
			result = createMaterial(shader,
				xml_get_str(node,"tex0"),
				xml_get_str(node,"tex1"),
				xml_get_str(node,"tex2"),
				xml_get_str(node,"tex3"),
				xml_get_str(node,"tex4"),
				xml_get_str(node,"tex5"),
				xml_get_str(node,"tex6"),
				xml_get_str(node,"tex7")
			);

			if(shader == "diffuseStateBox") then
				local _lineColor =    xml_get_str(node,"_lineColor");
				--print(_lineColor);
				--local arr=func_split(_lineColor,",");

				glsl_set(result,string.format("_lineColor,%s",_lineColor));
			end
		end
	end
	
--	xml_del(xml);
	xml:dispose();
	return result;
end

--删除table下面的元素(遍历所有的表元素引用)
function func_clearTableItem(point)
	for k, v in pairs(point) do
		point[k] = nil
	end
end
--遍历打印表
local function printTable(t)
	func_print("start print table: "..tostring(t),0xffff00)
	--print("start print table: "..tostring(t))
	for key, value in pairs(t) do      
		--print('key=['..key..']'..'value['..value..']')
		local s = tostring(value)
		if(tonumber(value)) then
			
			--转化为16进制数据
			s =  string.format("%#x",tonumber(value)).."\t("..value..")"
		end
		
		print(tostring(key).."\t\t\t="..s)
		--print(value)
		--print("-------------")
		
		--print(string.format("%#x",tonumber(value)))
		
		--print(tonumber(value))
	end 
	func_print("end print table:   ["..tostring(t).."]",0xffff00)
	
end

--获取进程运行时间
function func_get_longTime()
	return get_attr(nil,"get_longTime");
end

--打印一个有颜色的日志到控制台
function func_print(s,c)
	if(DEBUG~=nil)then
		c = c or 0xffff00
		--c = c or 0;
		
		--向控制台输出有颜色的文本日志
		dofunc("LogPut",string.format("lua: %s\n",s),c);
		
		--打印堆栈
		--dofunc("LogPut",string.format("lua stack: %s\n",debug.traceback()),0x00ff00);
	end
end
function func_error(msg,func)
	msg = msg or "";
	local s = ''
	
	s = 'lua:'..msg..''
	--func_print(s,0xff0000)
	--func_print('lua error:'..s,0xff0000)
	
	func_print(s,0xff0000);
	
	s = ''
	if(func) then
		--print ('line num:'..debug.getinfo(1).currentline..',file name:'..debug.getinfo(1).name )
		local info = debug.getinfo(func)
		for k,v in pairs(info) do
			
				--全部打印
				--print(k, ':', info[k])
				
				--linedefined				
				--short_src
				--s = s..tostring(k)..':\t\t'..tostring(info[k])..'\n'
				
				if(tostring(k) == 'linedefined') then
					s = s..'lineNum:'..tostring(info[k])..'\t'
				end
				
				if (tostring(k) == 'short_src') then
					s = s..'file:'..tostring(info[k])..'\t'
				end
		end
	end
	func_print(s,0xff0000);
	--printTable(debug.getinfo(1))
	--print(debug.traceback())
	
	func_print(debug.traceback(),0xff0000);
	
	--assert(nil,s)
	--error(msg)
    --print ( debug.getinfo(1).name )
end

--[[
	设置sprite对象的相对于父对象的坐标
--]]
function func_set_local_pos(p,x,y)
	local pos = x..","..y;
	change_attr(p,"sprite_set_self_pos",pos);
end

--[[
	将child添加到parent中
--]]
function func_addchild(parent,child,x,y)
	x = x or 0;
	y = y or 0;
	
	if(parent==nil) then
		func_error();
	end
	
	sprite_addChild(parent,child);
	func_set_local_pos(child,x,y);
end

--内存回收C模块的
function func_gc()
	change_attr(nil,"gc");
	--print('gc')
end

--lua的GC
function func_lua_gc(key)
	local old =  collectgarbage("count");
	--func_print("==> GC之前 LUA VM使用的总内存数:"..(collectgarbage("count")*1024).." bytes");
	collectgarbage("collect");
	local n = collectgarbage("count");
	func_print((key or "").." GC完成 LUA VM使用的总内存数:\t"..(n/1024).." mb\t("..n.."kb) gc了["..((old -  n)*1024).."]bytes",0x00ff00);
end

--创建名字
function func_create_name(suffix)
	return getName(suffix);
end

--为sprite设置贴图
function func_setIcon(sprite,url)
	--获取一个atals图集,没有图集的界面是黑色的
	local atals = JEngine:getIns():get_atals();
	if(atals) then
		sprite_bindAtals(sprite,atals);

		sprite_texName(sprite,url);
	end
end

--获取sprite的宽高
function func_get_sprite_size(o)
	return get_attr(o,"spriteSize")	
end

--重置sprite的宽高
function func_set_sprite_size(o,w,h)
	change_attr(o,"sprite_resize",string.format('%d,%d',w,h));
end

--获取鼠标拾取的sprite相对坐标
function func_get_sprite_mouse_xy(o)
	local x , y=get_attr(o,"spriteLocalXY");
	return x,y
end

--加载文件返回一个字符串
function func_loadfile(url)
    return change_attr(nil,"loadfile",url);
end


--字符串分割成table
function func_split(str, delimiter)
    delimiter = delimiter or ","
    if str==nil or string.len(str)==0 or delimiter==nil then
        return nil
    end
 
    local result = {}
    for match in (str..delimiter):gmatch("(.-)"..delimiter) do
        table.insert(result, match)
    end
    return result
end

--加载完tex之后的回调方法
function func_texloadend(n)
	sprite_set_9grid(n.sprite,n.url,3,3,3,3,JEngine:getIns():get_atals());
end

--创建一个支持九宫格的的sprite
function func_create_grid_sprite(x,y,w,h,url,name)
	url = url or "gundi.png";
	local sprite = sprite_create_typical(name,x,y,w,h);
	loadtexs(url,func_texloadend, { sprite=sprite;url=url});
	JEngine:getIns():add(sprite);
	return sprite;
end
--相当于addchild
function func_addnode(parent,n,x,y)
	x = x or 0;
	y = y or 0;
	local _type = n.type;
	
	if(_type == UI_TYPE.ScrollBar) then
		func_addchild(parent,scrollBar_get_container(n),x,y);
	elseif(_type == UI_TYPE.Button)then
		func_addchild(parent,btn_get_container(n),x,y);
	elseif(	
		_type == UI_TYPE.Image
		or _type == UI_TYPE.Shape
		) then
		func_addchild(parent,n.container,x,y);
	elseif(_type == UI_TYPE.ListBox) then
		func_addchild(parent,listbox_get_container(n),x,y);	
	elseif(_type == UI_TYPE.NScrollBar
			or _type == UI_TYPE.ProgressBar 
			or _type == UI_TYPE.NListBox
			or _type == UI_TYPE.NLabel
			or _type == UI_TYPE.NButton
			or _type == UI_TYPE.CheckBox
			or _type == UI_TYPE.Input

	) then
		local c = n:get_container();
		if(c == nil) then
			func_error();
		end
		
		--print(c);
		func_addchild(parent,c,x,y);	
	else
		func_error(string.format("type = %s未实现",tostring(_type)));
	end
end

require("base");
require("linenode");

require("label");--label是对ftext进行的一次封装

require("input");

require("loadtexs")	--资源加载器

require("cam");
require("engine")	--引擎1

require("fbo")	--fbo


require("shape")	--shape组件
require("NPanel");

require("NListbox")

require("NScrollBar")	--滑动条组件

require("button");		--按钮组件

require("image")	--image组件

require("checkbox")	--checkbox组件
require("progrossbar")
require("NSkin");--NSkin皮肤组件

require("NUnit");
require("UnitBase");		--角色单位
require("md5unit");		--md5对象

require("scrollview")	--scrollview

--[[
local tf = require("tf");
print(tf);
tf = nil;
--print(tf.get());
--]]


core = {};
local M = core;
--延迟ms毫秒,执行callback
function M.setTimeout(ms,callback)
	local timer = timelater_new(ms);
	local o = {t=timer,c=callback};

	evt_once(timer,EVENT_TIMER,function()
			if(o.c) then
				o.c();
			end
		end
		);
	return o;
end

function M.clearTimeout(o)
	--evt_off(o.t,EVENT_TIMER,o.c);
	o.c = nil;
	o.t = nil;
end

function M.now()
	return func_get_longTime();
end

function M.gc()
	func_lua_gc();
end

--移除模块,如果有的模块需要重新加载的初始化的,可以使用该接口
function M.removeRequire( preName )
    for key, _ in pairs(package.preload) do
        if string.find(tostring(key), preName) == 1 then
			print("preload remoeve preName:"..preName);
			package.preload[key] = nil;
        end
    end
    for key, _ in pairs(package.loaded) do
		if string.find(tostring(key), preName) == 1 then
			print("loaded remoeve preName:"..preName);
            package.loaded[key] = nil;
        end
    end
end



-- local function fc()
-- 	print("fc..."..func_get_longTime());
-- end

--print("xx..."..func_get_longTime());
--local o = setTimeout(1000,fc);
--clearTimeout(o);
M.UI_TYPE = UI_TYPE;


-- print("core init!!!");

return core;