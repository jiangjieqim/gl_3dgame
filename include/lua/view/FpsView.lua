--Fps��ͼ��
local FpsView = {
	
};

FpsView.__index = FpsView;
setmetatable(FpsView, IPlugin);--�̳��Բ���ӿ�

--�������
function FpsView:load()
	local self = {
		label,
		timer,
		formatstr,
	};
	setmetatable(self, FpsView);
	return self;
end

local function f_fps_timer(data,param)
	local self = param;
	local _fps = JEngine:getIns():get_fps();
	local str = _fps;
	if(self.formatstr) then
		str = string.format(self.formatstr,_fps);
	end
	if(self.label) then
		self.label:set_text(str);
	end
end

--ж�ز��,�����ٲ��
function FpsView:unload()
	if(self.timer) then
		timelater_remove(self.timer);
	end
	self:hide();
	if(self.label) then
		--print("dispose self.label!");
		self.label:dispose();
	end
	--setmetatable(self, nil);
	--func_clearTableItem(self);
end

function FpsView:show(x,y,formatstr)
----[[
	if(self.label==nil) then
		self.label = NLabel:new(64,64);
		self.label:set_text("fps");
	end
--]]
	
	if(self.label) then
		self.label:set_pos(x or 0,y or 0);
		self.label:visible(true);
	end
		
	self.formatstr = formatstr;
	
----[[
	if(self.timer==nil) then
		self.timer = timelater_new(1000);
		evt_on(self.timer,EVENT_TIMER,f_fps_timer,self);
	end	
--]]
end

function FpsView:getName()
	return "FpsView";
end

function FpsView:hide()
	if(self.label) then
		self.label:visible(false);
	end
	if(self.timer) then
		evt_off(self.timer,EVENT_TIMER,f_fps_timer);
		self.timer = nil;
	end
end

return FpsView;