local m_name = "triangle"
m_name = "bauul"
m_name = "gobin"

--初始化动作(处理成动态加载配置文件,可以做一个编辑器编辑这些缩放和偏移有问题的md2文件)
local function f_split_init(md2)
	
	if(m_name == "bauul" or m_name == "gobin")  then
		func_set_scale(md2,1/50);--这里设置一个缩放价值让模型在单位1以内
		--func_set_y(md2,0.5);	 --设置地板y轴向偏移
		func_setRotateX(md2,PI/2)--旋转一个轴向
	end
	
	
	--print(md2);
	func_anim_push(md2,"stand",0,39);
	func_anim_push(md2,"run",40,45);
	func_anim_push(md2,"jump",66,71);
	func_set_anim(md2,"stand");
	func_play(md2);
end

--加载一个box
local function f_load_box(vbo)

	--vbo = false;
	
	local obj
	local url = "\\resource\\obj\\box.obj";
	if(vbo) then
		obj=load_VBO_model(func_create_name(),url);--box	arrow
		setMaterial(obj,func_load("//resource//material//triangle.mat"));		
	else
		obj =load_model(func_create_name(), "\\resource\\obj\\torus.obj");
		setMaterial(obj,func_load("//resource//material//diffuse.mat"));
	end
	
	setv(obj,FLAGS_DRAW_PLOYGON_LINE)--线框
	--setv(obj,FLAGS_DISABLE_CULL_FACE);
	setv(obj,FLAGS_REVERSE_FACE);
	setv(obj,FLAGS_VISIBLE);
	return obj
end

Unit = {}
--print(Unit)
local function f_removeEvt(obj)
	--print(obj,obj.p);
	evt_off(obj.p,EVENT_ENGINE_BASE_UPDATE,obj.update);
	evt_off(obj.p,EVENT_ENGINE_BASE_END,obj.endCall);
end;

--	_key = "box"的时候显示一个红色的box
function Unit:create(_key)
	
	--重载tostring方法,输出相关信息
	
	self.__tostring = function(t)
		return string.format("元表 = %s p = %0x speed = %.3f",tostring(self),t.p,t.speed);
	end;
	
	local new_sharp = { 
		p = nil;--角色句柄
		
		speed;--移动速度
		
		update =function(data)
					--print(data,self,self.p);
					func_update_mat4x4(self.p);
					
					--func_update_mat4x4(self.box);
					--print("p = "+ p);
				end;
		
		endCall=function(data)
					
					--print("end %d:%d"..func_get_longTime().."\t"..self)
					f_removeEvt(self);
					
					func_set_anim(self.p,"stand");
					--print("移动结束!!!");
				end;
				
		
		--创建一个基本单位,默认在1个单位区间内
		init =  function ()
					--local u = {
					--	p = nil;--资源对象
					--}
					--drfreak
					--bauul
					--triangle
					
					---[[
					
					if(_key ~= "box") then
						local url = m_name;
						local modelURL = string.format("\\resource\\md2\\%s.md2",url);
						local md2=load_VBO_model(func_create_name(),modelURL);
						--local md2 = load_model(func_create_name(),modelURL,0,0,0,1.0);
						setMaterial(md2,func_load(string.format("//resource//material//%s.mat",url)));
						setv(md2,FLAGS_VISIBLE);
						
						
						f_split_init(md2);
						--setv(md2,FLAGS_RAY);
						--setv(md2,FLAGS_DRAW_RAY_COLLISION);
						setv(md2,FLAGS_DISABLE_CULL_FACE)--取消双面渲染
						----------------------------
						self.p = md2;
						
						
					else
						self.p = f_load_box(true);
					end
					
					func_set_y(self.p,0.5);--	y轴偏移0.5
					--self.offset_y = offset_y;
					
					self.speed = 1000;
				end;
	}
    self.__index = self  --②，self == Sharp
    setmetatable(new_sharp, self)  --③
	
	--初始化
	
	new_sharp.init();
	
    return new_sharp;
end
--移动一格需要的毫秒数
function Unit:set_speed(value)
	self.speed = value;
end

function Unit:move(x,y,z)
	local o = self.p;
	
	local px,py,pz = func_get_xyz(o);
	
	y = py;
	--print(self.offset_y);
	local distance = vec_distance(px,py,pz,x,y,z);--求其水平距离
	
	--print(string.format("@@@ %.3f %.3f %.3f=>%.3f %.3f %.3f",px,py,pz,x,y,z));
	--local func = self.update;
	--print(self,func,o);
	--self.update("aaaaa");
	func_look_at(o,x,y,z);--转向目标坐标
	--func_look_at(self.box,x,y,z)
	
--	evt_off(o,EVENT_ENGINE_BASE_UPDATE,self.update);
--	evt_off(o,EVENT_ENGINE_BASE_END,self.endCall);
	
	
	func_set_anim(self.p,"run");
	
	--self.removeEvt();
	f_removeEvt(self);
	evt_on(o,EVENT_ENGINE_BASE_UPDATE,self.update);
	evt_on(o,EVENT_ENGINE_BASE_END,self.endCall);
	
	--print(string.format("需要行走的距离 = %.3f",distance));
	
	--func_move(o,distance * 1000,x,y,z);--(一个单位距离用1秒的速度);
	func_move(o,distance * self.speed,x,y,z);
	--func_move(self.box,distance * self.speed,x,y,z);
end

function Unit:set_anim(ani)
	func_set_anim(self.p,ani);
end