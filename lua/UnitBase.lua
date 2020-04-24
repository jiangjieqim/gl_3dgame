UnitBaseEvent = 10000;
UnitBaseEndMsg = "UnitBaseEndMsg";--�ƶ�����
require("animator")

--*************************************************************
--ֻʵ�� p = TYPE_OBJ_VBO_FILE ���͵Ķ���
UnitBase = {

    p,--��ɫ���,�������е�md2,obj,md5���õ�ֵ
    speed,--�ƶ��ٶ�	
	anim,--�������������
};
UnitBase.__index = UnitBase;

setmetatable(UnitBase, NUnit);

function UnitBase:new()
    local s = NUnit:new();
    setmetatable(s,UnitBase);
   --s.speed = 1000;
    return s;
end

function UnitBase:dispose()
	if(self.anim) then
		self.anim:dispose();
	end
	ptr_remove(self.p);
	func_clearTableItem(self);
end

--���ö����cam�ľ���ռ�
local function f_set_cam(self)
	JEngine:getIns():bind_3dcam(self.p);
end


--�������������
function UnitBase:get_anim()
	return self.anim;
end

--����VBOģ��
function UnitBase:loadvbo(modelURL,maturl,cam)
            
    --print("**������ɫ",modelURL,name);

    local m=JEngine:getIns():load(modelURL);
	
    local material = self:load_material(maturl);
    setMaterial(m,material);
	--setv(m,FLAGS_VISIBLE);--��ʾģ�Ͷ���
	
	--f_split_init(md2);
	
    --setv(md2,FLAGS_DISABLE_CULL_FACE)--����˫�涼����Ⱦ
    --local ss =new_sharp;
   
    self.p = m;
	
	JEngine:getIns():add(self.p);
	self:set_cam(cam);
	--f_set_cam(self);
	
	self:visible(true);
	local suffix = get_suffix(modelURL);
	--print(suffix);
	if(suffix~= "obj") then
		self.anim = Animator:new(self.p);
	else
		--func_error(modelURL);
    end
    -- print("is loadvbo"..modelURL);
    -- print(self);
end
--����һ������������
function UnitBase:loadbox()
    local url = "\\resource\\obj\\box.obj";--tri
    local maturl = maturl or "//resource//material//triangle.mat"
    local name = func_create_name();--f_createName(self);
    local obj=load_VBO_model(name,url);--box	arrow
   
    local mat = self:load_material(maturl);
    --self.material = mat;
	setMaterial(obj,mat);
    setv(obj,FLAGS_DRAW_PLOYGON_LINE)--�߿�
    --setv(obj,FLAGS_DISABLE_CULL_FACE);--����˫�涼����Ⱦ
    setv(obj,FLAGS_REVERSE_FACE);
    setv(obj,FLAGS_VISIBLE);
    self.p = obj;
	f_set_cam(self);
end
function UnitBase:set_position(x,y,z)
    change_attr(self.p,"set_position",string.format("%f,%f,%f",x,y,z));
end
function UnitBase:load_model(url,maturl)
    url =  url or "\\resource\\obj\\plane.obj";
    maturl = maturl or "//resource//material//triangle.mat"
    local _floor = load_model(func_create_name(),url);
	local mat = self:load_material(maturl);
    --self.material = mat;
    setMaterial(_floor,mat);
    --glsl_set(mat,string.format("_lineColor,%s","0.5,0.5,0.5"));
	
	setv(_floor,FLAGS_VISIBLE);
	

--	setv(_floor,FLAGS_RAY)
--	--setv(_floor,FLAGS_DRAW_RAY_COLLISION)
--	setv(_floor,FLAGS_DRAW_PLOYGON_LINE)--�߿�
--	setv(_floor,FLAGS_DISABLE_CULL_FACE)
    self.p = _floor;
	f_set_cam(self);
end


function UnitBase:set_speed(v)
    self.speed = v;
end

--��һ������¼�
function UnitBase:bindRayPick(func)
    --print(self:getv(FLAGS_RAY));
    self:setv(FLAGS_RAY);
    evt_on(self.p,EVENT_RAY_PICK,func);
end
local function f_endCall(data)
    local p = JEngine:getIns():find(data);
--    local u = allUnits[data];
    func_set_anim(p,"stand");
    evt_off(p,EVENT_ENGINE_BASE_END,f_endCall);

    evt_dispatch(p,UnitBaseEvent,UnitBaseEndMsg);
    --print(data);
end
--[[
	������ײ��ָ��һ��.obj ������.md2��Ϊ������ײ���ӵĻ���ģ��
	drawbox : �Ƿ���ʾ��ײ��
	
	�˽ӿ���fboģʽ����Ⱦ���������ʰȡ����Ч��
--]]
function UnitBase:load_collide(model,drawbox)
	change_attr(self.p,"collide_load",model or "\\resource\\obj\\box.obj",frame or 0);--torus
	setv(self.p,FLAGS_RAY);
	if(drawbox) then
		setv(self.p,FLAGS_DRAW_RAY_COLLISION);
	end
end
--�ý�ɫ����ĳ������
local function func_move(o,ms,x,y,z)
	change_attr(o,"move",string.format("%d,%f,%f,%f",ms,x,y,z));
end

--�ý�ɫ����time����ת��ĳ������
local function f_look_at(o,x,y,z,time)
    time = time or 0;
	change_attr(o,"lookat",string.format("%f,%f,%f,%f",x,y,z,time));
end

function UnitBase:look_at(x,y,z,time)
	f_look_at(self.p,x,y,z,time);
end

function UnitBase:move(x,y,z)
    --print(x,y,z);
    x = tonumber(x);
    y = tonumber(y);
    z = tonumber(z);
    local o = self.p;
    
	if(o==nil) then
        func_error("unit.p = nil");
        return;
    end
--    if(self.speed==nil) then
--        func_error("unit.speed = nil");%
--        return;
--    end
    self.speed = self.speed or 1000;--Ĭ��ʹ��1000�����ƶ�һ��������λ������

	local px,py,pz = self:get_pos();--func_get_xyz(o);
	
	y = py;
	--print(self.offset_y);
	local distance = vec_distance(px,py,pz,x,y,z);--����ƽ�����
	
	f_look_at(o,x,y,z);--ת��Ŀ������
	--print("look at:",x,y,z);
	func_set_anim(self.p,"run");
	
    evt_off(o,EVENT_ENGINE_BASE_END,f_endCall);
	evt_on(o,EVENT_ENGINE_BASE_END,f_endCall);

	func_move(o,distance * self.speed,x,y,z);
end