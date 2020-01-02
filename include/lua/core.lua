CONST_DIRECTION_HORIZONTAL = 0	--水平,横
CONST_DIRECTION_VERTICAL   = 1 	--垂直,竖

CONST_PI	=	3.1415929
PI	= 3.1415929

FLAGS_RENDER_BOUND_BOX	=	0x01
FLAGS_DRAW_RAY_COLLISION	=	0x02	--绘制射线盒子	(是否绘制射线静态包围盒,用于测试 查看射线包围盒),即使没有设置FLAGS_RAY,那么也是可以绘制射线盒子的,这样可以用来查看
FLAGS_LINE_RENDER			=	0x04	--是否显示渲染线框
FLAGS_OUTLINE				=	0x08	--是否显示边缘轮廓
FLAGS_RAY					=	0x10	--16是否具有射线拾取功能(此只用于3d拾取,2d拾取不用这个标记)
FLAGS_VISIBLE				=	0x20	--32显示或者隐藏模型,(是否加入渲染队列中)
FLAGS_RENDER_DRAWSKELETON	=	64		--渲染骨骼节点(md5模型才会显示)
FLAGS_ANIM_ADAPTIVE		=	128		--是否根据关键帧的帧率适配关键帧的动画(是否适配fps 1,适配  0,不适配)
FLAGS_GLSL_OUTLINE		=	256		--用GLSL实现的轮廓线
FLAGS_DRAW_PLOYGON_LINE	=	512		--在第一个基本材质上用线框渲染(固定管线模式设置GL_LINE)
FLAGS_BUTTON_EFFECT		=	1024	--设置按钮特效(在有点击事件的前提下才会有该特效)
FLAGS_DRAW_NORMAL 		= 	2048	--法线
FLAGS_DRAG				=	4096	--是否可拖拽
FLAGS_DRAW_PLOYGON_POINT= 	8192	--点模式
FLAGS_DISABLE_CULL_FACE	=	16384	--设置显示双面
FLAGS_REVERSE_FACE		=	32768	--反转渲染面,只有设置了双面渲染才有效


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
EVENT_ENGINE_TEX_LOAD_COMPLETE = 108
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

--print(UI_TYPE.Label,UI_TYPE.ScrollBar);

dofile("..\\include\\lua\\stack.lua")
dofile("..\\include\\lua\\xml.lua")	--xml组件
dofile("..\\include\\lua\\vec3.lua")	--自定义数学库
--dofile("..\\include\\lua\\sprite.lua");
dofile("..\\include\\lua\\evt.lua")	--事件管理器
dofile("..\\include\\lua\\ftext.lua");

local defalutTex = '1.tga'

local tableInstance={}
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
--是否是table字符串
function func_is_table_str(value)
	local s = tostring(value);
	return string.find(s,"table: ") ~= nil;
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
function func_load(url)
	
	local suffix = get_suffix(url);
	--print(s);
	local result;
	local xml = xml_load(url);
	
	if(suffix == "mat") then
		local node = xml_get_node_by_index(xml,0);
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
		
	elseif(suffix == "obj") then
	
	end
	
	xml_del(xml);
	
	return result;
end

--删除table下面的元素(遍历所有的表元素引用)
function func_clearTableItem(point)
	for k, v in pairs(point) do
		point[k] = nil
	end
end
--遍历打印表
function func_printTable(t)
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

--获取table中的字段数
function func_get_table_count(t)
	local n = 0;
	for key, value in pairs(t) do      
		n = n + 1;
	end 
	return n
end

--[[
	打印tabel中的数据项
--]]
function func_print_tableInstance()
	print("####func_print_tableInstance Start...")
	
	local i = 0
	for key, value in pairs(tableInstance) do		
		
	--	if(size) then
	--		print("table size = "..(getDddress(value) - size).." bytes\n")
	--	end
	
		print("\n***** index = "..i..",key="..tostring(key)..",value="..tostring(value)..'-->'..getDddress(value).." *****")
		func_printTable(value)
		i = i + 1
		
	--	size = getDddress(value)	
	end
		
	print("####func_print_tableInstance End...")
end

--[[
	获取到table引用
--]]
function func_getTable(name)
	--return instance[name]
	
	--print("func_getTable-->"..name)
	return tableInstance[name]
end

--[[
	获取到table名(其实是table的lua空间地址)
	table:0EA02968
--]]
function func_getTableName(point)
--	return tableToInt(point)
	return tostring(point)
end

--[[
	获取当前时间 精确到豪秒
--]]
function func_getTime()
	return  dofunc("GetLongTime");
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
	
	print(s);
	
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
	print(s)
	--func_printTable(debug.getinfo(1))
	print(debug.traceback())
	
	--assert(nil,s)
	--error(msg)
    --print ( debug.getinfo(1).name )
end
--[[
	设置坐标(兼容模式)
--]]
function func_setPos(p,x,y)
	--local t = JEngine:getIns():get_type(p)
	local pos = x..","..y;
	--if(t == TYPE_SPRITE_FLIE) then	
	change_attr(p,"sprite_position",pos)
	--elseif(t == TYPE_TEXT_FILE)	then
	--	change_attr(p,"txt_position",pos)
	--end
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

--内存回收
function func_gc()
	change_attr(nil,"gc")
	--print('gc')
end



--加载一个md5模型
--"\\resource\\texture\\wolf.bmp"
function func_loadmd5(name,scale,texpath,meshName,animName)
	
	--if (name == nil or name == '') then name = "wolf" end
	name = name or 'wolf'
		
	--if(scale == nil) then scale = 1.0 end
	scale = scale or 1.0

	local path = "\\resource\\md5\\"..name
	
	if(texpath == nil) then texpath =  "\\resource\\texture\\"..defalutTex end
	
	--if(meshName == nil) then meshName = "body.md5mesh" end
	meshName = meshName or 'body.md5mesh'
	--if(animName == nil) then animName = "walk.md5anim" end
	animName = animName or 'walk.md5anim'

	local md5=load_model(name,path.."\\"..meshName,0,0,0,scale);	
	local mat1=createMaterial("diffuse",texpath,"");
	setMaterial(md5,mat1);
	
	md5_loadAnim(md5, path.."\\"..animName,"walk");
	setanim(md5,"walk");
	
--	md5_loadAnim(md5, path.."\\stand.md5anim","stand");
--	setanim(md5,"stand");
	
	setv(md5,FLAGS_VISIBLE);
	return md5;
	
	
	--print('error不存在md5文件:['..name..']')
	--assert(nil,"my Error!")
end



--创建名字
function func_create_name(suffix)
	return getName(suffix);
end


--加载md2模型
function func_loadmd2(objName,tex,type,modelName)
	local time = func_getTime()
	--if(type == nil) then type = "vbo" end
	type = type or 'vbo'
	
	
	local name = modelName or getName();
	
	--if( objName == nil or objName == "") then objName = "bauul" end
	objName = objName or "triangle"
	
	--if( tex == nil) then tex = defalutTex end
	tex = tex or defalutTex
	
	local o
	if(type == "vbo") then
		o=load_VBO_model(name,"\\resource\\md2\\"..objName..".md2");
		setv(o,FLAGS_VISIBLE);
		
		local m=createMaterial("vboDiffuse","\\resource\\texture\\"..tex);
		setMaterial(o,m);
	end
	
	local s = string.format("[加载解析%s耗时 %d 毫秒]",objName,(func_getTime() - time))
	--if(DEBUG) then func_print(s) end
	return o;
end
--加载一个obj 使用vbo模式
function func_loadobj(objName,tex,nName,vbo)
	local name
	if(nName==nil) then
		name = getName()
	else
		name = nName
	end
	objName = objName or 'quad';
	
	--func_print(	string.format("[%s]",tostring(objName)),0xff0000)
	
	--if( tex == nil) then tex = defalutTex end
	
	tex = tex or defalutTex
	if(vbo == nil) then
		vbo = true
	end
	
	
	--vbo = vbo or true
	
	---[[

	local o;
	local _path = "\\resource\\obj\\"..objName..".obj";
	local _shader;
	local _texturePath = "\\resource\\texture\\"..tex;

	if(vbo == true) then
		_shader = "vboDiffuse";
		o=load_VBO_model(name,_path);
	else
		_shader = "diffuse";
		o=load_model(name,_path,0,0,0,1.0);	
	end
	setv(o,FLAGS_VISIBLE);
	
	--print("func_loadobj===============>",_texturePath,string.format("加载得模型(%s),模型名(%s)是否是VBO模式:%s",_path,nName,tostring(vbo)));

	local m=createMaterial(_shader,_texturePath);
	--local m = core_load("//resource//mat1.mat");
	--local m = func_load("//resource//mat1.mat");
	
	setMaterial(o,m);
	return o;
	--]]
end
------------------------------------------------------
--切换状态
--返回true or false
------------------------------------------------------
function func_changeFlags(_o,_flag)
	if(_o==nil) then
		func_error("switchFlags 目标对象 = nil")
		return;
	end;
	
	if(_flag == nil)then
		func_error("flag = nil ")
	else
		
		if	getv(_o,_flag) == 1	then
			resetv(_o,_flag);
		else
			setv(_o,_flag);
		end
		
		--print('对象'..string.format("%#x",_o).."设置标示".._flag.."状态为:"..getv(_o,_flag) );
	end
--[[	
	local _stat = getv(_o,_flag)
	if (_stat == 1) then 
		return true
	end
	return false
--]]
	return getv(_o,_flag) == 1
end

--获取当前的焦点
function func_get_curFocus()
    return get_attr(nil,"curFocus");
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

--获取sprite的xy
function func_get_sprite_xy(o)
	return get_attr(o,"spritePos")	
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

function func_sprite_set_z(s,n)
    sprite_set_z(s,n);
end

--获取进程运行时间
function func_get_longTime()
	return get_attr(nil,"get_longTime");
end

--加载文件返回一个字符串
function func_loadfile(url)
    return change_attr(nil,"loadfile",url);
end

--获取屏幕的尺寸
function func_screenSize()
	local w,h=get_attr(nil,"screenSize");
	return w,h
end

function func_get_xyz(o)
	return get_attr(o,"xyz");
end
function func_get_scale(o)
	return get_attr(o,"scale");
end
function func_get_rotate(o)
	return get_attr(o,"rotate");
end
--让角色按照time毫秒转向某个方向
function func_look_at(o,x,y,z,time)
    time = time or 0;
	change_attr(o,"lookat",string.format("%f,%f,%f,%f",x,y,z,time));
end

function func_set_position(o,x,y,z)
	change_attr(o,"set_position",string.format("%f,%f,%f",x,y,z));
end

--让角色朝向某个方向
function func_move(o,ms,x,y,z)
	change_attr(o,"move",string.format("%d,%f,%f,%f",ms,x,y,z));
end

--更新对象矩阵
function func_update_mat4x4(o)
	change_attr(o,"base_updateMat4x4");
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
	sprite_set_9grid(n.sprite,n.url,n.l or 3,n.r or 3,n.t or 3,n.b or 3,JEngine:getIns():get_atals());
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
	
	if(_type == UI_TYPE.Label) then
		func_addchild(parent,ftext_get_container(n.tf),x,y);
	elseif(_type == UI_TYPE.ScrollBar) then
		func_addchild(parent,scrollBar_get_container(n),x,y);
	elseif(_type == UI_TYPE.Button)then
		func_addchild(parent,btn_get_container(n),x,y);
	elseif(
	
		--_type == UI_TYPE.Input
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


--销毁组件
function func_dispose(n)
   
	local _type = n.type;
	if(_type == UI_TYPE.Label) then
		label_dispose(n);
	elseif(_type == UI_TYPE.ScrollBar) then
		scrollBar_del(n);
--	elseif(_type == UI_TYPE.Button)then
--		btn_dispose(n);
	elseif(_type == UI_TYPE.CheckBox
		or _type == UI_TYPE.ProgressBar 
		or _type == UI_TYPE.Input
		or _type == UI_TYPE.Image
		or _type == UI_TYPE.Shape
		or _type == UI_TYPE.NScrollBar
		or _type == UI_TYPE.NListBox
		or _type == UI_TYPE.NPanel
		or _type == UI_TYPE.Skin
		or _type == UI_TYPE.NLabel
		or _type == UI_TYPE.NButton
										)then
		
		n:dispose();--删除组件
        
		
	elseif(_type == UI_TYPE.ListBox) then	
		listbox_del(n);
	else
		func_error();
	end
end

dofile("..\\include\\lua\\base.lua");
dofile("..\\include\\lua\\linenode.lua");

dofile("..\\include\\lua\\label.lua");--label是对ftext进行的一次封装

dofile("..\\include\\lua\\input.lua");

dofile("..\\include\\lua\\loadtexs.lua")	--资源加载器

dofile("..\\include\\lua\\cam.lua");
dofile("..\\include\\lua\\engine.lua")	--引擎1

dofile("..\\include\\lua\\fbo.lua")	--fbo

dofile("..\\include\\lua\\fps.lua")	--fps组件
dofile("..\\include\\lua\\shape.lua")	--shape组件
dofile("..\\include\\lua\\NPanel.lua");

dofile("..\\include\\lua\\NListbox.lua")
dofile("..\\include\\lua\\UListBox.lua")	--选项列表组件

dofile("..\\include\\lua\\UScrollBar.lua")	--滑动条组件

dofile("..\\include\\lua\\NScrollBar.lua")	--滑动条组件

dofile("..\\include\\lua\\button.lua");		--按钮组件

dofile("..\\include\\lua\\infowin.lua")	--fps组件

dofile("..\\include\\lua\\image.lua")	--image组件

dofile("..\\include\\lua\\checkbox.lua")	--checkbox组件
dofile("..\\include\\lua\\progrossbar.lua")
dofile("..\\include\\lua\\NSkin.lua");--NSkin皮肤组件

dofile("..\\include\\lua\\NUnit.lua");
dofile("..\\include\\lua\\UnitBase.lua");		--角色单位
dofile("..\\include\\lua\\md5unit.lua");

dofile("..\\include\\lua\\scrollview.lua")	--scrollview
