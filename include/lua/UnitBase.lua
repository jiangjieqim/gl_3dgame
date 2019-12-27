UnitBaseEvent = 10000;
UnitBaseEndMsg = "UnitBaseEndMsg";--移动结束
dofile("..\\include\\lua\\animator.lua")

--*************************************************************
--只实现 p = TYPE_OBJ_VBO_FILE 类型的对象
UnitBase = {

    p,--角色句柄,是引擎中的md2,obj,md5引用的值
    speed,--移动速度	
	anim,--动作管理器句柄
};
UnitBase.__index = UnitBase;
function UnitBase:new()
    local s = {};
    setmetatable(s,UnitBase);
   --s.speed = 1000;
    return s;
end

function UnitBase:dispose()
	self.anim:dispose();
	ptr_remove(self.p);
	func_clearTableItem(self);
end

--设置对象的cam的矩阵空间
local function f_set_cam(self)
	JEngine:getIns():bind_3dcam(self.p);
end
--获取句柄
function UnitBase:get_p()
    return self.p;
end

--动作管理器句柄
function UnitBase:get_anim()
	return self.anim;
end

--获取模型的类型
function UnitBase:get_type()
	return JEngine:getIns():get_type(self.p);
end

--设置当前的对象的cam
function UnitBase:set_cam(cam)
	if(cam) then
		set_cam(self.p,cam);--使用指定的cam
	else
		JEngine:getIns():bind_3dcam(self.p);--使用默认的cam
	end
end

function UnitBase:visible(v)
	if(v) then
		setv(self.p,FLAGS_VISIBLE);--显示
	else	
		resetv(self.p,FLAGS_VISIBLE);--隐藏
	end
end

function UnitBase:is_visible()
	return getv(self.p,FLAGS_VISIBLE) == 1;
end


--加载VBO模型
function UnitBase:loadvbo(modelURL,maturl,cam)
            
    --print("**创建角色",modelURL,name);

    local m=JEngine:getIns():load(modelURL);
	
	--load_VBO_model(name,modelURL);
    local material = func_load(maturl);
    setMaterial(m,material);
	--setv(m,FLAGS_VISIBLE);--显示模型对象
    
	
	
	--f_split_init(md2);
	
	
    --setv(md2,FLAGS_DISABLE_CULL_FACE)--设置双面都能渲染
    --local ss =new_sharp;
   

    self.p = m;
	
	JEngine:getIns():add(self.p);
	self:set_cam(cam);
	--f_set_cam(self);
	
	self:visible(true);
	
	self.anim = Animator:new(self.p);
end
--加载一个测试立方体
function UnitBase:loadbox()
    local url = "\\resource\\obj\\box.obj";--tri
    local maturl = maturl or "//resource//material//triangle.mat"
    local name = func_create_name();--f_createName(self);
    local obj=load_VBO_model(name,url);--box	arrow
   
    local mat = func_load(maturl);
    --self.material = mat;
	setMaterial(obj,mat);
    setv(obj,FLAGS_DRAW_PLOYGON_LINE)--线框
    --setv(obj,FLAGS_DISABLE_CULL_FACE);--设置双面都能渲染
    setv(obj,FLAGS_REVERSE_FACE);
    setv(obj,FLAGS_VISIBLE);
    self.p = obj;
	f_set_cam(self);
end

function UnitBase:load_model(url,maturl)
    url =  url or "\\resource\\obj\\plane.obj";
    maturl = maturl or "//resource//material//triangle.mat"
    local _floor = load_model(func_create_name(),url)		-- func_loadobj('quad',nil,'myObj1',false)--quad
	local mat = func_load(maturl);
    --self.material = mat;
    setMaterial(_floor,mat);
    --glsl_set(mat,string.format("_lineColor,%s","0.5,0.5,0.5"));
	
	setv(_floor,FLAGS_VISIBLE);
	

--	setv(_floor,FLAGS_RAY)
--	--setv(_floor,FLAGS_DRAW_RAY_COLLISION)
--	setv(_floor,FLAGS_DRAW_PLOYGON_LINE)--线框
--	setv(_floor,FLAGS_DISABLE_CULL_FACE)
    self.p = _floor;
	f_set_cam(self);
end

function UnitBase:setv(v)
    setv(self.p,v)
end
function UnitBase:getv(v)
    return getv(self.p,v)
end
function UnitBase:scale(value)
    func_set_scale(self.p,value);
end
function UnitBase:get_scale()
    return func_get_scale(self.p);
end

function UnitBase:rx(v)
    func_setRotateX(self.p,v)
end
function UnitBase:ry(v)
    func_setRotateY(self.p,v)
end
function UnitBase:rz(v)
    func_setRotateZ(self.p,v)
end
function UnitBase:x(v)
    func_set_x(self.p,v);
end

function UnitBase:y(v)
    func_set_y(self.p,v);
end
function UnitBase:z(v)
    func_set_z(self.p,v);
end
function UnitBase:set_position(x,y,z)
    func_set_position(self.p,x,y,z);
end
function UnitBase:get_name()
    return func_get_name(self.p);
end

function UnitBase:set_speed(v)
    self.speed = v;
end
--[[
function UnitBase:refresh()
	func_update_mat4x4(self.p);
end
--]]
--绑定一个点击事件
function UnitBase:bindRayPick(func)
    --print(self:getv(FLAGS_RAY));
    self:setv(FLAGS_RAY);
    evt_on(self.p,EVENT_RAY_PICK,func);
end
local function f_endCall(data)
    local p = func_find_obj(data);
--    local u = allUnits[data];
    func_set_anim(p,"stand");
    evt_off(p,EVENT_ENGINE_BASE_END,f_endCall);

    evt_dispatch(p,UnitBaseEvent,UnitBaseEndMsg);
    --print(data);
end
--[[
	加载碰撞盒
	drawbox : 是否显示碰撞盒
	
	bug:fbo拾取无效
--]]
function UnitBase:load_collide(model,drawbox)
	change_attr(self.p,"collide_load",model or "\\resource\\obj\\box.obj",frame or 0);--torus
	setv(self.p,FLAGS_RAY);
	if(drawbox) then
		setv(self.p,FLAGS_DRAW_RAY_COLLISION);
	end
end

function UnitBase:move(x,y,z)
    --print(x,y,z);
    x = tonumber(x);
    y = tonumber(y);
    z = tonumber(z);
    local o = self.p;
    
    --print("==============>name=",func_get_name(o));

	if(o==nil) then
        func_error("unit.p = nil");
        return;
    end
--    if(self.speed==nil) then
--        func_error("unit.speed = nil");%
--        return;
--    end
    self.speed = self.speed or 1000;--默认使用1000毫秒移动一个基本单位的速速

	local px,py,pz = func_get_xyz(o);
	
	y = py;
	--print(self.offset_y);
	local distance = vec_distance(px,py,pz,x,y,z);--求其平面距离
	
	func_look_at(o,x,y,z);--转向目标坐标
	
	func_set_anim(self.p,"run");
	
    evt_off(o,EVENT_ENGINE_BASE_END,f_endCall);
	evt_on(o,EVENT_ENGINE_BASE_END,f_endCall);

	func_move(o,distance * self.speed,x,y,z);
end