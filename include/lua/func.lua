


flags={}

flags.FLAGS_RENDER_BOUND_BOX	=	0x01
flags.FLAGS_DRAW_RAY_COLLISION	=	0x02	--绘制射线盒子	(是否绘制射线静态包围盒,用于测试 查看射线包围盒),即使没有设置FLAGS_RAY,那么也是可以绘制射线盒子的,这样可以用来查看
flags.FLAGS_LINE_RENDER			=	0x04	--是否显示渲染线框
flags.FLAGS_OUTLINE				=	0x08	--是否显示边缘轮廓
flags.FLAGS_RAY					=	0x10	--16是否具有射线拾取功能(此只用于3d拾取,2d拾取不用这个标记)
flags.FLAGS_VISIBLE				=	0x20	--32显示或者隐藏模型,(是否加入渲染队列中)
flags.FLAGS_RENDER_DRAWSKELETON	=	64		--渲染骨骼节点(md5模型才会显示)
flags.FLAGS_ANIM_ADAPTIVE		=	128		--是否根据关键帧的帧率适配关键帧的动画(是否适配fps 1,适配  0,不适配)
flags.FLAGS_GLSL_OUTLINE		=	256		--用GLSL实现的轮廓线
flags.FLAGS_DRAW_PLOYGON_LINE	=	512		--在第一个基本材质上用线框渲染(固定管线模式设置GL_LINE)
flags.FLAGS_BUTTON_EFFECT		=	1024	--设置按钮特效(在有点击事件的前提下才会有该特效)
flags.FLAGS_DRAW_NORMAL 		= 	2048	--法线
flags.FLAGS_DRAG				=	4096	--是否可拖拽
flags.FLAGS_DRAW_PLOYGON_POINT	= 	8192	--点模式


flags.SHADER_FLAG_FLAT_COLOR	=0x01

-------------------------------------------------------

--键值存储所有的对象引用
instance={}


--[[
	表引用转地址
	0EA02968
--]]
function tableToInt(t)
	_l = string.len("table:  ");
	s=string.sub(tostring(t),_l,-1);
	return s;
end
--#################################################################
dofile("..\\include\\lua\\core.lua")
--#################################################################
--------------------------------------------------------
function func_bindInstance(sc)
	local s = tableToInt(sc)
	
	--print("***********************"..s)
	instance[s] = sc;
	return s;
end

--获取对象
function func_getInstance(name)
	return instance[name]
end

--获取一个背景名
function func_convertBgName(s)
	return s.."_bg"
end

function str_cut_decimal(f,c)
	local s =  tostring(f);
	local i = string.find(s,".")+1
	
	return string.sub(s,0,i+c)--保留小数点后面c位
end

--获取整数部分
function tl_getIntegralPart(x)
	if x <= 0 then
	   return math.ceil(x);
	end

	if math.ceil(x) == x then
	   x = math.ceil(x);
	else
	   x = math.ceil(x) - 1;
	end
	return x;
end


xfun = {}
--根据名字找节点
function xfun.FindNode(name)
	return dofunc("FindNode",name);
end
function xfun.RotateX(obj,v)
	change_attr(obj,"rx",v)
end
function xfun.RotateZ(obj,v)
	change_attr(obj,"rz",v)
end

function xfun.Scale(obj,v)
	dofunc("SetScale",obj,v);
end

function xfun.GetName(obj)
	return dofunc("GetName",obj)
end

--设置摄影机的坐标
function
xfun.SetCamera(x,y,z)
		dofunc("SetCameraPosition",x,y,z);
end

