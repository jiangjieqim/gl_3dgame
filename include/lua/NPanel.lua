NPanel = {
	bg,--Shape
	isDrag,		--是否可以拖拽

};

NPanel.__index = NPanel;
setmetatable(NPanel, Base);

function NPanel:new(w,h)
	w = w or 300;
	h = h or 200;
	local self = Base:new();
	setmetatable(self, NPanel);
	self.bg = Shape:new(true,w,h);
	self:settype(15);
	return self;
end
local function f_resize(evtData,self)
	--print(evtData,self);
	local sw,sh = func_screenSize();
	local bg = self.bg;
	local sx,sy = bg:get_pos();
	local bgw,bgh = bg:get_size();
		
	--print(sx,sy,bgw,bgh);
	if(self.isDrag and sw > bgw and sh > bgh) then
		--print(-sx,-sy,sw-sx,sh-sy);
		bg:set_drag_rect(-sx,-sy,sw-sx,sh-sy);
	end
	
	self:center();
end
--居中
function NPanel:center()
	local bg = self.bg;

	local x,y;
	local sx,sy = func_screenSize();
	local bgw,bgh = bg:get_size();
	--print(bgw,bgh );
	x = (sx - bgw)/2;
	y = (sy - bgh)/2;
	
	self.bg:set_pos(x,y);
end
--设置可以居中
function NPanel:enable_center(v)
	local bg = self.bg;
	if(v) then
		bg:on(EVENT_ENGINE_RESIZE,f_resize,self);
	else
		bg:off(EVENT_ENGINE_RESIZE,f_resize);
	end
end

function NPanel:setDrag(v)
	self.isDrag = v;
	local bg = self.bg;
	bg:mouseEnable(v);
end

function NPanel:dispose()
	local bg = self.bg;
	self:enable_center(false);
	--func_error(0);
	bg:dispose();
	--local bg = self.bg;
end

--[[

案例:

local p = NPanel:new();
p:enable_center(true);
p:setDrag(true);
p:center();

--]]