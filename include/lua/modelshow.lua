--[[

<ui type="NListBox" x="0" y="180" parent="1"/>

--]]


--模型预览
ModleShow = {
	nskin,
	fbo,
	
	u,--UnitBase
	editor,
	
	floorm,

};
ModleShow.__index = ModleShow;


--增加一个md2渲染对象到fbo对象引用中
local function addmd2_fbo(fbo)
	local cam = fbo:get_cam3d();
	cam = nil;
	
	----bauul,triangle
	
	
--[[
	local n = Md5Unit:new();
	n:load(cam);
	n:set_position(0,0,-2);
	n:scale(0.02);
	--n:drawPloygonLine(true);
	n:drawSkeleton(true);
	--print(n:frame_count());
--]]
	
--[[
	local n = UnitBase:new();--md2简模
	n:loadvbo("\\resource\\md2\\triangle.md2","//resource//material//bauul.mat",cam);
	n:set_position(0,0,-2);
	local anim = n:get_anim();
	anim:push("jump",0,3);
	anim:play("jump");
	n:load_collide(nil,true);
	
	--anim:set_fps(1);	
--]]

--[[
	local n = UnitBase:new();
	n:loadvbo("\\resource\\md2\\bauul.md2","//resource//material//bauul.mat",cam);
	n:set_position(0,0,-100);
	--n:scale(0.02);
	local anim = n:get_anim();
	anim:push("jump",0,37);
	anim:play("jump");
	n:load_collide(nil,true);
	--anim:set_fps(1);
--]]
	
	--n:set_fps(30);
	
----[[
	--加载一个vbo类型的OBJ
	local n = UnitBase:new();
	n:loadvbo("\\resource\\obj\\arrow.obj","//resource//material//horse.mat",cam);
	--n:set_position(0,0,-5);
	n:load_collide(nil,true);
	
--]]
	
	return n;
end

local function f_scale_handle(progress,self)
	self.nskin:find("scale_label"):set_text(progress);
	self.u:scale(progress);
	--print(progress,self);
end

local function f_setfps_handle(progress,self)
	--print(progress);
	local v = math.floor(progress*260);
	self.nskin:find("fpslabel"):set_text(v);
	self.u:set_fps(v);
end

local function f_set_anim(progress,self)
	local n = self.u;
	--n:set_frame(progress*20);
	local anim = n:get_anim();
	if(anim) then
		local v = math.floor(anim:total()*progress)-1;
		if(v < 0) then
			v = 0;
		end
		self.nskin:find("anim_label"):set_text(v);
		
		anim:play_to(v,v);
	
	end
end

local function btnClick(self)
	print("dasdasd");
	
	local n = self.u;
	print("it`s name = "..n:get_name());
		
----[[	
	local anim = n:get_anim();
	if(anim) then
		print(
			--JEngine:getIns():get_type_str(self.u:get_p())
			"type:"..n:get_type(),
			anim:cur_frame().."/"..anim:total(),
			"lua总内存数: "..collectgarbage("count").." kb"
			
		);
		
		if(anim:isPlaying()) then
			anim:pause();
		else
			anim:play("jump");
			--anim:play_to(2,3);
		end
	end
--]]
	
	
	----[[
	
	
	if(n:is_visible()) then
		n:visible(false);
		
		--JEngine:getIns():get_cam():rx(PI * 1.8);	
	else
		n:visible(true);
	end
	--]]
	
	
	
	
	--m_dispose(self);
	--self:dispose();
	--u:dispose();
	

end

local function f_set_rotate(progress,self)
----[[
	if(self.u)then
		self.u:rx(PI * progress);
	end
--]]


	
	--test code
--[[
	local skin = self.nskin;
	local tri2 = skin:find("tri2");	
	tri2:setRotateZ(math.pi * progress);
--]]

	--print(math.pi*progress);
end

local function ef(data,self)
	local skin = self.nskin;
	local label = skin:find("info_label");

	local a = self.u:get_anim();
	if(a~=nil)then
		label:set_text(a:cur_frame());
	end
end

local function f_cpmlete(self)
	local skin = self.nskin;
	local x,y = skin:get_pos();
	local namemap = skin.namemap;
	
	local fbo = FboRender:new(128,128);
	fbo:set_pos(x,y);
	
	self.fbo = fbo;
	
	self.u = addmd2_fbo(fbo);--增加一个vbo模型
	
	local btn0 =skin:find("btn0");
	btn0:bind_click(btnClick,self);
    

	local scale = namemap["scale"];--skin:find("scale");
	
	scale:bindCallback(f_scale_handle,self);

	local anim = skin:find("anim");
	anim:bindCallback(f_set_anim,self);
	
	local setfps = skin:find("setfps");
	setfps:bindCallback(f_setfps_handle,self);

	
	--self.fbo:dispose();
	--print(x,y);
	--self:dispose();
	--skin:dispose();

	--self.nskin:dispose();
	
	local timer = timelater_new(1);
	
	evt_on(timer,EVENT_TIMER,ef,self);
	
	
	local rotate = skin:find("rotate");
	rotate:bindCallback(f_set_rotate,self);

	--构建一个测试使用的线段
	local line = LineNode:new(2);
	line:push(0,0,0);
	line:push(0,0,3);
	line:setcolor(1,1,0);
	line:graphics_end();

	local function f_bindRayClick(p)
		local x,y,z = JEngine:getIns():get_hit();--获取射线的交点坐标

		print('射线的交点 f_bindRayClick:',x,y,z);
		
		line:mod(1,x,y,z);
		
		--self.u:move(x,y,z);
		self.u:look_at(x,y,z,0);
	end
	local function loadfloor()
		local n = UnitBase:new();
		
		n:loadvbo("\\resource\\obj\\plane.obj",
			"//resource//material//horse.mat");
		--n:reverse_face(true);
		n:scale(10);
		n:load_collide("\\resource\\obj\\plane.obj",true);
		n:bindRayPick(f_bindRayClick);
		return n;
	end
	self.floorm = loadfloor();

--	local tri2 = skin:find("tri2");	
--	tri2:setRotateZ(PI/4);
	

	--self.editor:dispose();
end

function ModleShow:new(editor)
	local self = {};
	setmetatable(self, ModleShow);
	
	local function f_cpmleteHandler(skin)
		f_cpmlete(self);
	end
	self.editor = editor;
	
	local nskin = NSkin:new();
	self.nskin = nskin;
	evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmleteHandler);
	nskin:load("\\resource\\modelshow.xml","gundi.png;checkbox.png;smallbtn.png");	
	
	return self;
end
--销毁ModelShow
function ModleShow:dispose()
	--print("ModleShow:show");
	if(self.fbo) then
		self.fbo:dispose();
	end
	if(self.nskin) then
		self.nskin:dispose();
	end
	
	if(self.u) then
		self.u:dispose();
	end
	if(self.floorm) then
		self.floorm:dispose();
	end
	func_clearTableItem(self);
end
