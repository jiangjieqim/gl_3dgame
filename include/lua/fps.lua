--[[local bytes ,emptycnt,allbytes= memory_get_info();
		local s = string.format("fps=%d—”≥Ÿ%d-%d-%d-[%d,%d,(_%.0f_kb)]",curFbs,engine_getDelayTime(),maxValue,_time/1000,
					bytes,emptycnt,allbytes / 1024);
--]]
--##################################################################

--Fps ”Õº¿‡
FpsView = {label,timer,formatstr};

FpsView.__index = FpsView;
function FpsView:new()
	local self = {};
	setmetatable(self, FpsView);
	
	--tf = ftext_create(128,128);
    --ftext_set_buffer(tf,128);
	--self.tf = tf;
	self.label = label_create(64,64);
	return self;
end
local function f_fps_timer(data,param)
	local self = param;
	local _fps = engine_get_fps();
	local str = _fps;
	if(self.formatstr) then
		str = string.format(self.formatstr,_fps);
	end
	
	label_set_text(self.label,str);
end

function FpsView:dispose()
	if(self.timer) then
		timelater_remove(self.timer);
	end
	self:hide();
	label_dispose(self.label);	
end
function FpsView:show(x,y,formatstr)
	label_set_pos(self.label,x or 0,y or 0);
	label_set_visible(self.label,1);
	--print(self.x);
	self.formatstr = formatstr;
	if(self.timer==nil) then
		local timer = timelater_new(1000);
		self.timer = timer;
		evt_on(timer,EVENT_TIMER,f_fps_timer,self);
	end
end

function FpsView:hide()
	--ftext_vis(self.tf,0);
	label_set_visible(self.label,0);
	local timer = self.timer;
	evt_off(timer,EVENT_TIMER,f_fps_timer);
end

--Fbsµƒ»›∆˜
function FpsView:container()
	return self.label;
end

--local mc = FpsView:new();
--mc:show();
--mc:hide();
--mc:dispose();

--fps();