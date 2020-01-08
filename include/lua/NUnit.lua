------------------------------------------------------
--切换状态
--返回true or false
------------------------------------------------------
local function f_changeFlags(_o,_flag)
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
	return getv(_o,_flag) == 1;
end

local function func_set_position(o,x,y,z)
	change_attr(o,"set_position",string.format("%f,%f,%f",x,y,z));
end

local function func_get_scale(o)
	return get_attr(o,"scale");
end

--设置坐标
local function func_set_x(o,x)
	change_attr(o,"x",x)
end
local function func_set_y(o,y)
	change_attr(o,"y",y)
end
local function func_set_z(o,z)
	change_attr(o,"z",z)
end

NUnit = {
	p,
};
NUnit.__index = NUnit;

function NUnit:new()
	local self = {};
	setmetatable(self, NUnit);
	return self;
end

--获取模型的类型
function NUnit:get_type()
	return JEngine:getIns():get_type(self.p);
end

--设置当前的对象的cam
function NUnit:set_cam(cam)
	if(cam) then
		set_cam(self.p,cam);--使用指定的cam
	else
		JEngine:getIns():bind_3dcam(self.p);--使用默认的cam
	end
end

--获取句柄
function NUnit:get_p()
    return self.p;
end

function NUnit:visible(v)
	if(v) then
		setv(self.p,FLAGS_VISIBLE);--显示
	else	
		resetv(self.p,FLAGS_VISIBLE);--隐藏
	end
end

function NUnit:f_set_flag(flag,v)
	if(v) then
		self:setv(flag);
		--print(v);
	else
		self:resetv(flag);
	end
end
--设置为线框渲染
function NUnit:drawPloygonLine(v)
	self:f_set_flag(FLAGS_DRAW_PLOYGON_LINE,v);
end

--反转面
function NUnit:reverse_face(v)
	self:f_set_flag(FLAGS_REVERSE_FACE,v);
end

--[[
--背面剔除
function NUnit:cullFace(v)
	self:f_set_flag(FLAGS_DISABLE_CULL_FACE,v);
end--]]

function NUnit:is_visible()
	return getv(self.p,FLAGS_VISIBLE) == 1;
end
--设置缩放比率
function NUnit:scale(value)
	--print(value);
    change_attr(self.p,"scale",value);
end

function NUnit:setv(v)
    setv(self.p,v)
end
function NUnit:resetv(v)
    resetv(self.p,v)
end
function NUnit:getv(v)
    return getv(self.p,v)
end

function NUnit:get_scale()
    return func_get_scale(self.p);
end

--获取rx,ry,rz的值
function NUnit:get_rotate()
	--return get_attr(self.p,"rotate");
	func_error("未实现!");
end

--获取其x,y,z坐标
function NUnit:get_pos()
	return get_attr(self.p,"xyz");
end


--绕自定义轴旋转r欧拉角
--normal 是否单位化向量
function NUnit:rotate_vec(r,x,y,z)
--[[	if(normal) then
		local x1,y1,z1 = vec3Normalize(x,y,z);
		
		print(x1,y1,z1);
	end--]]
	change_attr(self.p,"rotate_vec",x,y,z,r);
end


function NUnit:rx(v)
    --func_setRotateX(self.p,v)
    self:rotate_vec(v,1,0,0);
end

function NUnit:ry(v)
--    func_setRotateY(self.p,v)
    self:rotate_vec(v,0,1,0);
end
function NUnit:rz(v)
--    func_setRotateZ(self.p,v)
    self:rotate_vec(v,0,0,1);
end
function NUnit:x(v)
    func_set_x(self.p,v);
end

function NUnit:y(v)
    func_set_y(self.p,v);
end
function NUnit:z(v)
    func_set_z(self.p,v);
end

function NUnit:set_position(x,y,z)
    func_set_position(self.p,x,y,z);
end

--获取对象名
function NUnit:get_name()
    return get_attr(self.p,"get_name")
end

----设置对象的关键帧帧率
function NUnit:set_fps(v)
	change_attr(self.p,"fps",tostring(v))
end

--切换flag
function NUnit:changeFlag(flag)
	f_changeFlags(self.p,flag);
end

--加载材质
function NUnit:load_material(url)
	return func_load_material(url);
end