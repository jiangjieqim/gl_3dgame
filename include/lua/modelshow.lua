--模型预览
ModleShow = {
	nskin,
	fbo,
	
	u,--UnitBase
};
ModleShow.__index = ModleShow;


--增加一个md2渲染对象到fbo对象引用中
local function addmd2_fbo(fbo)

	local cam = fbo:get_cam3d();
	--cam = nil;
	
	----bauul,triangle
	
	local n = UnitBase:new();
	
----[[
	--md2简模
	n:loadvbo("\\resource\\md2\\triangle.md2","//resource//material//bauul.mat",cam);
	n:set_position(0,0,-2);
	local anim = n:get_anim();
	anim:push("jump",0,3);
	anim:play("jump");
	n:load_collide(nil,true);
	
	--anim:set_fps(1);
	
	--]]
--[[
	n:loadvbo("\\resource\\md2\\bauul.md2","//resource//material//bauul.mat",cam);
	n:set_position(0,0,-100);
	local anim = n:get_anim();
	anim:push("jump",0,37);
	anim:play("jump");
	n:load_collide(nil,true);
	--anim:set_fps(1);
--]]
	
	return n;
end

local function f_scale_handle(progress,self)
	--local self = scrollBar_get_param(sc);
	--func_rotate(unit_get_model(self.obj), "ry", sc.value*PI);
	self.u:scale(progress);
	--print(progress,self);
end

local function f_set_anim(progress,self)
	local n = self.u;
	local anim = n:get_anim();
	local v = math.floor(anim:total()*progress)-1;
	if(v < 0) then
		v = 0;
		
	end
	anim:play_to(v,v);
end

local function btnClick(self)
	local n = self.u;
	local anim = n:get_anim();
	print("u name ="..n:get_name(),
		--JEngine:getIns():get_type_str(self.u:get_p())
		"type:"..n:get_type(),
		anim:cur_frame().."/"..anim:total(),
		"lua总内存数: "..collectgarbage("count").." kb"
		
	);
	
	----[[
	if(n:is_visible()) then
		n:visible(false);
	else
		n:visible(true);
	end
	--]]
	
	local anim = n:get_anim();
	if(anim:isPlaying()) then
		anim:pause();
	else
		anim:play("jump");
		--anim:play_to(2,3);
	end
	--m_dispose(self);
	--self:dispose();
	--u:dispose();
end

local function ef(data,self)
	local skin = self.nskin;
	local label = skin_find(skin,"label1");

	local a = self.u:get_anim();
	
	label:set_text(a:cur_frame());
end

local function f_cpmlete(self)
	local skin = self.nskin;
	local x,y = skin:get_pos();
	
	local fbo = FboRender:new(128,128);
	fbo:set_pos(x,y);
	self.fbo = fbo;
	
	self.u = addmd2_fbo(fbo);--增加一个vbo模型
	
	local scale = skin:find("scale");
	scale:bindCallback(f_scale_handle,self);

	local anim = skin:find("anim");
	anim:bindCallback(f_set_anim,self);

	
	--self.fbo:dispose();
	--print(x,y);
	--self:dispose();
	--skin:dispose();

	--self.nskin:dispose();
	
	
	local btn0 = skin_find(skin,"btn0");
	btn0:bind_click(btnClick,self);
	
	local timer = timelater_new(1);
	
	evt_on(timer,EVENT_TIMER,ef,self);

	
end

function ModleShow:new()
	local self = {};
	setmetatable(self, ModleShow);
	
	local function f_cpmleteHandler(skin)
		f_cpmlete(self);
		--m_dispose(self);
	end
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
	
	self.nskin:dispose();
	
	func_clearTableItem(self);
end
