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
FLAGS_REVERSE_FACE		=	32768	--反转渲染面


--//文件类型定义
TYPE_OBJ_FILE = 0--	//obj类型
TYPE_MD5_FILE =1 --//md5类型
TYPE_MD2_FILE =2 --//md2类型
TYPE_SPRITE_FLIE =3--//UI界面类型中的sprite
TYPE_TEXT_FILE	=4	--//文本类型
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

dofile("..\\include\\lua\\xml.lua")	--xml组件
dofile("..\\include\\lua\\vec3.lua")	--自定义数学库
dofile("..\\include\\lua\\evt.lua")	--事件管理器

local defalutTex = '1.tga'
local defalutObjModel = 'quad'

local tableInstance={}
--转化出一个地址
--"=table: 0082E758"  ===>8578904
local function 
getDddress(value)
	local len = string.len("table:  ")
	local a = string.len(tostring(value))
	local v = string.sub(tostring(value),len,a)
	local s = tonumber('0x'..v)
	return s
end

--获取一个默认的贴图
function func_get_default_tex()
	return defalutTex
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
		
		local shaderParm = xml_get_str(node,"shaderParm");
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
		
	elseif(suffix == "obj") then
	
	end
	
	xml_del(xml);
	
	return result;
end



--[[
	point:tabel引用保存在一个全局tabel中
--]]
function func_tableSave(point)
	--point['index'] = getIndex()
	
	--local s = tableToInt(point)
	--instance[s] =  point
	
	--print(tostring(point)..','..getDddress(point))
	--getDddress
	tableInstance[tostring(point)]=point
end
--[[
	point:删除一个表引用
--]]
function func_tableDel(point)
	--清理掉table中的元素
	func_clearTableItem(point)
	
	--清理table的引用
	tableInstance[tostring(point)] = nil
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
	local n = 0
	for key, value in pairs(t) do      
		n = n + 1
	end 
	return n
end

--[[
	打印tabel中的数据项
--]]
function func_print_tableInstance()
	print("####func_print_tableInstance Start...")
--	local size
	
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

--创建一个Sprite
function func_sprite_create(name,x,y,w,h)
	local sprite = sprite_create(name,x,y,w,h);
	return sprite
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
	0EA02968
--]]
function func_getTableName(point)
--	return tableToInt(point)
	return tostring(point)
end
--[[
	获取对象的类型
#define TYPE_OBJ_FILE 0	//obj类型
#define TYPE_MD5_FILE 1 //md5类型
#define TYPE_MD2_FILE 2 //md2类型
#define TYPE_SPRITE_FLIE 3//UI界面类型中的sprite
#define TYPE_TEXT_FILE	4	//文本类型
#define TYPE_OBJ_VBO_FILE	5//VBO文件数据

--]]
function func_get_type(p)--func_getGameObjectType
	return	get_attr(p,"type")
end


function func_get_type_str(p)
	local t = func_get_type(p)
	if(t == 0) then
		return 'obj'
	elseif(t == 1)then
		return 'md5'
	elseif(t == 2)then
		return 'md2'
	elseif(t == 3)then
		return 'sprite'
	elseif(t == 4)then
		return 'text'
	elseif(t == 5)then
		return 'vbo'
	end
end

--[[
	获取当前时间 精确到豪秒
--]]
function func_getTime()
	return  dofunc("GetLongTime")
end

--打印一个有颜色的日志到控制台
function func_print(s,c)
	c = c or 0xffff00
	dofunc("LogPut",string.format("%s\n",s),c)
end
function func_error(msg,func)
	local s = ''
	
	s = 'lua:'..msg..''
	--func_print(s,0xff0000)
	--func_print('lua error:'..s,0xff0000)
	
	func_print(s,0xff0000)
	
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
	func_print(s,0xffff00)
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
	
	local t = func_get_type(p)
	
	local pos = x..","..y
	
	if(t == TYPE_SPRITE_FLIE) then
		
		change_attr(p,"sprite_position",pos)
	
	elseif(t == TYPE_TEXT_FILE)	then
	
		change_attr(p,"txt_position",pos)
	
	end
	
end




--内存回收
function func_gc()
	change_attr(nil,"gc")
	--print('gc')
end

--绑定拾取回调
function func_set_pick(o,func)
	change_attr(o,"set_pickBind",func);
end
--设置碰撞盒子颜色
function func_set_box_color(o,r,g,b)
	change_attr(o,"set_box_color",string.format("%f,%f,%f",r,g,b))
end

--改变对象的Rotate x的值
function func_setRotateX(o,value)
	change_attr(o,"rx",tostring(value));
end




--改变对象的Rotate y的值
function func_setRotateY(o,value)
	change_attr(o,"ry",tostring(value));
end


--改变对象的Rotate z的值
function func_setRotateZ(o,value)
	change_attr(o,"rz",tostring(value));
end

--重命名
function func_rename(o,value)
	change_attr(o,"rename",tostring(value));
end

--设置ray拾取半径
function func_set_rayRadius(o,value)
	change_attr(o,"set_rayRadius",tostring(value));
end
--设置坐标
function func_set_x(o,x)
	change_attr(o,"x",x)
end
function func_set_y(o,y)
	change_attr(o,"y",y)
end
function func_set_z(o,z)
	change_attr(o,"z",z)
end

--设置缩放比率
function func_set_scale(o,v)
	change_attr(o,"scale",v)
end
--[[
function func_set_camera_pos(x,y,z)
		dofunc("SetCameraPosition",x,y,z);
end
--]]
--获取对象名
function func_get_name(o)
	return get_attr(o,"get_name")
end

--获取对象的后缀类型字符串
function func_get_suffix(o)
	return get_attr(o,"suffix")
end

--加载一个固定管线模型数据md2,obj
function func_fixed_load(url,scale,texpath)
	local o=load_model(func_create_name(),url or "\\resource\\obj\\torus.obj",0,0,0,scale or 1.0)
	local mat1=createMaterial("diffuse",texpath or ("\\resource\\texture\\"..defalutTex),"")
	setMaterial(o,mat1)
	setv(o,FLAGS_VISIBLE);
	return o
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

local nameKey = -1;
--生成一个名字
local function getName()
	nameKey=nameKey+1;
	return "instance_"..tostring(nameKey);
end

--创建名字
function 
func_create_name()
	return getName()
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
--	if( objName == nil or objName == "") then objName = defalutObjModel end
	objName = objName or defalutObjModel
	
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
--绑定一个碰撞盒
function func_bind_collide(o,model,frame)
	change_attr(o,"collide_load",model or "\\resource\\obj\\box.obj",frame or 0);
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

local uiAtals

--获取一个界面图集(没有就创建,在调用的时候创建)
function func_getUI_Atals()
	--print(uiAtals)
	
	if(uiAtals == nil) then		
		uiAtals=atals_load("//resource//texture//","1")--构造界面图集	
	end
	return uiAtals
end
--为sprite设置贴图
--sprite_texName(listBox.sprite,"gundi.png");废弃
function func_setIcon(sprite,url)
	--获取一个atals图集,没有图集的界面是黑色的
	local atals = func_getUI_Atals()
	if(atals) then
		sprite_bindAtals(sprite,atals)

		sprite_texName(sprite,url);
	end
end
--从引擎层获取对对象
function func_find_obj(name)--func_getObj_ByName_EG
	return dofunc("FindNode",name);
end
--根据区间播放动画
function func_play_anim(o,s,e)
	change_attr(o,"animtor_play_start_end",string.format('%d,%d',s,e))
end

--播放动画
function func_play(o)
	change_attr(o,"animtor_play");--播放
end
--获取动作关键帧数(md5)
function func_get_frame_count(o)
	return get_attr(o,"frameCount")
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
--获取进程运行时间
function func_get_longTime()
	return get_attr(nil,"get_longTime");
end

--获取进程运行时间
function func_screenSize()
	local w,h=get_attr(nil,"screenSize");
	return w,h
end

--获取fps
function func_fps()
	return get_attr(nil,"fps");
end
--获取delayTime
function func_delayTime()
	return get_attr(nil,"delayTime");
end
--设置对象的关键帧帧率
function func_set_ptr_fps(o,v)
	change_attr(o,"fps",tostring(v))
end
--设置动作
function func_set_anim(o,anim)
	change_attr(o,"animtor_setcur",anim);
end

--获取文本
function func_get_tf_text(tf)
	return dofunc("tf_get_str",tf);
end

--移除引擎对象(md5,md2,textfiled,sprite,obj)
function func_ptr_remove(o)
	return ptr_remove(o)
end

--设置glsl参数
--有可能当前材质的着色器对象里没有关键字key,那么这时候设置值的时候就会失败
function func_set_glsl_parms(o,key,v)
	change_attr(o,"glslParms",string.format('%s,%s',key,tostring(v)))
end

--是否采用的是VBO模式渲染的
function func_is_vbo(obj,tips)
	if(func_get_type_str(obj)=='vbo') then
		return true
	end
	return false
end

--分割配置动画
function func_anim_push(obj,animname,s,e)
	change_attr(obj,"animtor_push",animname,string.format('%s,%s',s,e));
end

--获取引擎信息
function func_ex_info()
	get_attr(nil,"ex_info");
end

function func_get_xyz(o)
	return get_attr(o,"xyz");
end
--让角色朝向某个方向
function func_look_at(o,x,y,z)
	change_attr(o,"lookat",string.format("%f,%f,%f",x,y,z));
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

--function func

dofile("..\\include\\lua\\cam.lua")	--cam
dofile("..\\include\\lua\\fps.lua")	--fps组件

dofile("..\\include\\lua\\UListBox.lua")	--选项列表组件
dofile("..\\include\\lua\\UScrollBar.lua")	--滑un动条组件
dofile("..\\include\\lua\\UIlib.lua")		--按钮组件


dofile("..\\include\\lua\\infowin.lua")	--fps组件

dofile("..\\include\\lua\\alert.lua")		--弹出框组件
dofile("..\\include\\lua\\arrow.lua")		--箭头组件
dofile("..\\include\\lua\\unit.lua")		--角色单位