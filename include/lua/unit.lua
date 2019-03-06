--初始化动作(处理成动态加载配置文件,可以做一个编辑器编辑这些缩放和偏移有问题的md2文件)
local function f_split_init(md2)
	func_anim_push(md2,"stand",0,39);
	func_anim_push(md2,"run",40,45);
	func_anim_push(md2,"jump",66,71);
	func_set_anim(md2,"stand");
	func_play(md2);
end
--[[
--加载一个box
local function f_load_box(vbo)

	--vbo = false;--是否使用vbo模式
	
	local obj
	local url = "\\resource\\obj\\o1.obj";--tri
	if(vbo) then
		local name = func_create_name();
		--print("name="..name);
		obj=load_VBO_model(name,url);--box	arrow
        local mat = func_load("//resource//material//triangle.mat");
		setMaterial(obj,mat);		
	else
		obj =load_model(func_create_name(), "\\resource\\obj\\box.obj");
		setMaterial(obj,func_load("//resource//material//diffuse.mat"));
	end
	
	setv(obj,FLAGS_DRAW_PLOYGON_LINE)--线框
	setv(obj,FLAGS_DISABLE_CULL_FACE);--设置双面都能渲染
	setv(obj,FLAGS_REVERSE_FACE);
	setv(obj,FLAGS_VISIBLE);
	return obj
end
--]]
Unit = {}

--所有的单位对象集合
local allUnits = {};

--	_key = "DEBUG"的时候显示一个测试模型

local function f_endCall(data)
    local u = allUnits[data];
    func_set_anim(u.p,"stand");
    evt_off(u.p,EVENT_ENGINE_BASE_END,f_endCall);
end

local function f_createName(t)
    local name = func_create_name();
    --print("===============================",t);
    allUnits[name] =  t;
    t.name = name;
    return name;
end;


function Unit:create(_key)
	
	--重载tostring方法,输出相关信息
	--[[
	self.__tostring = function(t)
		return string.format("p = %0x speed = %.3f",t.p,t.speed);
	end;
	--]]
	local new_sharp = { 
		p;--角色句柄
		name;
		speed;--移动速度
		
		update =function(data)
					--print(data,self,self.p);
					func_update_mat4x4(self.p);
					
					--func_update_mat4x4(self.box);
					--print("p = "+ p);
				end;
		
        --创建一个基本单位,默认在1个单位区间内
        --加载一个vbo模型
        loadvbo = function(modelURL,maturl)
            local name = f_createName(self);
            
            print("创建角色",modelURL,name);

            local md2=load_VBO_model(name,modelURL);
            setMaterial(md2,func_load(maturl));
	        setv(md2,FLAGS_VISIBLE);
            f_split_init(md2);
            setv(md2,FLAGS_DISABLE_CULL_FACE)--取消双面渲染
            --local ss =new_sharp;

            self.p = md2;
        end;

        --加载一个测试的立方体
        loadbox = function(vbo,url)
            vbo = vbo or true;
            local obj
            local name = f_createName(self);
	        url = url or "\\resource\\obj\\o1.obj";--tri
	        if(vbo) then
		        --local name = func_create_name();
		        --print("name="..name);
		        obj=load_VBO_model(name,url);--box	arrow
                local mat = func_load("//resource//material//triangle.mat");
		        setMaterial(obj,mat);		
	        else
		        obj =load_model(name, "\\resource\\obj\\box.obj");
		        setMaterial(obj,func_load("//resource//material//diffuse.mat"));
	        end
	
	        setv(obj,FLAGS_DRAW_PLOYGON_LINE)--线框
	        setv(obj,FLAGS_DISABLE_CULL_FACE);--设置双面都能渲染
	        setv(obj,FLAGS_REVERSE_FACE);
	        setv(obj,FLAGS_VISIBLE);
            self.p = obj;
        end;

        --设置对象的坐标
        position=function(x,y,z)
            func_set_position(self.p,x,y,z);
        end;

        --移动一格需要的毫秒数
        set_speed=function(value)
	        self.speed = value;
        end;
        move=function(x,y,z)
	        local o = self.p;
            --
            --print("==============>name=",func_get_name(o));

	        if(o==nil) then
                func_error("unit.p = nil");
                return;
            end
            if(self.speed==nil) then
                func_error("unit.speed = nil");
                return;
            end
	        local px,py,pz = func_get_xyz(o);
	
	        y = py;
	        --print(self.offset_y);
	        local distance = vec_distance(px,py,pz,x,y,z);--求其水平距离
	
	        func_look_at(o,x,y,z);--转向目标坐标
	
	        func_set_anim(self.p,"run");
	
            evt_off(o,EVENT_ENGINE_BASE_END,f_endCall);
	        evt_on(o,EVENT_ENGINE_BASE_END,f_endCall);

	        func_move(o,distance * self.speed,x,y,z);
        end
	}
    self.__index = self  --②，self == Sharp
    setmetatable(new_sharp, self)  --③
	
	--初始化
	--local f = new_sharp.endCall;
	--new_sharp.init();
	--print(new_sharp);
    print("###",self,tostring(new_sharp));
    return new_sharp;
end





function Unit:scale(value)
    func_set_scale(self.p,value);
end
function Unit:rx(v)
    func_setRotateX(self.p,v)
end
function Unit:x(v)
    func_set_x(self.p,v);
end
function Unit:y(v)
    func_set_y(self.p,v);
end
function Unit:z(v)
    func_set_z(self.p,v);
end

function Unit:get_ptr()
	return self.p;
end	

function Unit:getname()
    return self.name;
end

--以一定速度转向目标
function Unit:rotateTo(x,y,z,time)
    --local s = string.format("%.3f\t%.3f\t%.3f\ttime=%.3f\n",x,y,z,time);
    --print(s);
    func_look_at(self.p,x,y,z,time);
end

function Unit:set_anim(ani)
	func_set_anim(self.p,ani);
end




