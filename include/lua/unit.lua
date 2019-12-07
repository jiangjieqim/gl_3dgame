UnitBaseEvent = 10000;
UnitBaseEndMsg = "UnitBaseEndMsg";--移动结束

local function f_init()
	local self = {
		p,--模型引用
		material,--材质引用
		type,--p的类型
		url,
	};
	return self;	
end

function unit_get_model(self)
	return self.p;
end

--初始化动作(处理成动态加载配置文件,可以做一个编辑器编辑这些缩放和偏移有问题的md2文件)
function unit_split_anim(self)
	local md2 = self.p;
	func_anim_push(md2,"stand",0,39);
	func_anim_push(md2,"run",40,45);
	func_anim_push(md2,"jump",66,71);
	func_set_anim(md2,"stand");
	func_play(md2);
end

local function get_suffix(url)
	local len = string.len(url);
	local str=string.sub(url,len-2,-1);
	--print("["..str.."]");
	return str;
end

function unit_load(modelURL,maturl)
	local self = f_init();
	local name = func_create_name();--f_createName(self);
    self.url = modelURL;
	--local s,e = string.find(modelURL,"md2");
	--print(string.sub(modelURL,a,b)	);
	local _suffix =	get_suffix(modelURL);
	
	local md2;
    --print("**创建角色",modelURL,name);
	if(_suffix == "md2") then
		md2=load_VBO_model(name,modelURL);
	elseif(_suffix == "obj") then
		md2=load_model(name,modelURL);
	else
		print("未实现该格式的文件解析"..modelURL);
		return;
	end
	self.type = func_get_type(md2);
    self.material = func_load(maturl);
    setMaterial(md2,self.material);
	setv(md2,FLAGS_VISIBLE);
	
	--f_split_init(md2);--切割动作
    
	--setv(md2,FLAGS_DISABLE_CULL_FACE)--设置双面都能渲染
    --local ss =new_sharp;
	
    self.p = md2;
	--f_set_cam(self);
	engine_bind_default_3dcam(self.p);--设置默认的cam矩阵空间
	return self;
end
function unit_scale(self,value)
    func_set_scale(self.p,value);
end
function unit_rx(self,v)
    func_setRotateX(self.p,v)
end
function unit_ry(self,v)
    func_setRotateY(self.p,v)
end


local function f_endCall(data)
    local p = func_find_obj(data);
--    local u = allUnits[data];
    func_set_anim(p,"stand");
    evt_off(p,EVENT_ENGINE_BASE_END,f_endCall);

    evt_dispatch(p,UnitBaseEvent,UnitBaseEndMsg);
    --print(data);
end

function unit_move(self,x,y,z,speed,moveAnim)
	moveAnim = moveAnim or "run";
    local o = self.p;    
	if(o==nil) then
        func_error("unit.p = nil");
        return;
    end
	speed = speed or 1000;
	local px,py,pz = func_get_xyz(o);
	
	local distance = vec_distance(px,py,pz,x,y,z);--求其平面距离
	
	func_look_at(o,x,y,z);--转向目标坐标
	
	func_set_anim(self.p,moveAnim);
	
	evt_off(o,EVENT_ENGINE_BASE_END,f_endCall);
	evt_on(o,EVENT_ENGINE_BASE_END,f_endCall);

	func_move(o,distance * speed,x,y,z);

end

--绑定一个点击事件
function unit_bind_ray_pick(self,func)
    --print(self:getv(FLAGS_RAY));
    setv(self.p,FLAGS_RAY);
    evt_on(self.p,EVENT_RAY_PICK,func);
end

--创建一个单位点击地板移动
function example_unit()
	local function loadfloor()
		local obj2 = unit_load("\\resource\\obj\\plane.obj",--triangle,bauul
		"//resource//material//triangle.mat");
		unit_scale(obj2,20);
		
			setv(obj2.p,FLAGS_REVERSE_FACE);
			setv(obj2.p,FLAGS_DRAW_PLOYGON_LINE);
		return obj2;
	end
	cam_setRotateX(PI * 1.8);
	cam_setPosition(0,0,-6);
	
	local obj2 = unit_load("\\resource\\md2\\bauul.md2",--triangle,bauul
		"//resource//material//bauul.mat");
	unit_scale(obj2,0.02);
	unit_rx(obj2,PI/2);
	unit_split_anim(obj2);
	--setv(obj2.p,FLAGS_DRAW_PLOYGON_LINE);
	
	--print("info=>",obj2.type,obj2.url);
	
	local floor = loadfloor();
		--添加一个滚动条组件
	local animsc = scrollBar_new();
	
	local function f_animscHandle(sc)
		local v = sc.value * PI * 2;
		scrollBar_label(animsc,v);	
		if(obj2) then
			unit_ry(obj2,v);
		end	
	end

	scrollBar_setRange(animsc,0,1);
	scrollBar_bind(animsc,f_animscHandle);
	
	local function f_on_click_floor_handle(data)
		local pos = func_split(data,",");
		if(obj2) then
			unit_move(obj2,pos[1],pos[2],pos[3]);
		end
	--	print("请求移动");
		end
	unit_bind_ray_pick(floor,f_on_click_floor_handle);
	
	local function f_onkey(data)
		local key = tonumber(data);
		--print('my key = '..key);
		if(key == KEY_A) then
			--print("get curFocus point = "..func_get_curFocus());	
		elseif(key == KEY_B)then
			func_changeFlags(floor.p,FLAGS_DRAW_PLOYGON_LINE);
		elseif(key == KEY_ESC) then
		--	engine_exit();
		end
	end
--if(obj2) then
    evt_on(obj2,EVENT_ENGINE_KEYBOARD,f_onkey);
end
--*****************************************************

Unit = {
	material,--材质引用
	p,--模型引用
};

Unit.__index = Unit;

function Unit:new()
	local self = {};
	setmetatable(self, Unit);
	return self;
end

function Unit:load(url,maturl,mname)
	local name = mname or func_create_name();
	--print(name);
	
	local m =load_VBO_model(name,url);--load_model(name,url);
	self.m = m;
	self.material = func_load(maturl);
    setMaterial(m,self.material);
	setv(m,FLAGS_VISIBLE);
	engine_bind_default_3dcam(m);
--	print(m);
end

function Unit:set_pos(x,y,z)
	func_set_x(self.m,x);
	func_set_y(self.m,y);
	func_set_z(self.m,z);
end

function Unit:set_rx(value)
	func_setRotateX(self.m,value)
end
function Unit:set_ry(value)
	func_setRotateY(self.m,value)
end
function Unit:set_rz(value)
	func_setRotateZ(self.m,value)
end

local function f_endCall(data)
    local p = func_find_obj(data);
	print("移动结束",p,data);
--    local u = allUnits[data];
--    func_set_anim(p,"stand");
	
    --evt_dispatch(p,UnitBaseEvent,UnitBaseEndMsg);
    --print(data);
end
function Unit:move(x,y,z,usetime)
	--evt_off(self.m,EVENT_ENGINE_BASE_END,f_endCall);
	evt_once(self.m,EVENT_ENGINE_BASE_END,f_endCall);
	func_move(self.m,usetime,x,y,z);
end