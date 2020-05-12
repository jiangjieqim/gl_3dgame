---@class Button : Base
Button = {
	

};

Button.__index = Button;

setmetatable(Button, Base);

function Button:new(w,h,url)
	w = w or 80;
	h = h or 18;

	local self = Base:new();
	
	--***********************************************

	--***********************************************
	setmetatable(self, Button);

	self.img = nil;--Image
	self.label = nil;--NLabel
	self.callBack = nil;--回调函数
	self.param = nil;--回调函数的参数
	self.effect = nil;--是否设置有按下去的效果
	self.oldw = w;
	self.oldh = h;
	self.oldx = nil;
	self.oldy = nil;


	local img = Image:new(w,h);
	img:seticon(url or "smallbtn.png");
	img:mouseEnable(true);
	self:settype(16);
	self.img = img;
	self:btn_effect(true);--设置按钮的缩放效果
	return self;
end

function Button:set_label(str,x,y)
	if(self.label==nil) then
		local label = NLabel:new();
		self.label = label;
		self.img:addChild(label:get_container(),x,y);
	end
	local _l = self.label;
	_l:set_text(str);
end

function Button:get_container()
	return self.img:get_container();
end
--设置Z轴的旋转值
function Button:setRotateZ(v)
	self.img:setRotateZ(v);
end

--设置按钮效果
function Button:btn_effect(v)
	-- local container = self.img:get_container();
	-- if(v) then
	-- 	setv(container,FLAGS_BUTTON_EFFECT);
	-- else
	-- 	resetv(container,FLAGS_BUTTON_EFFECT);
	-- end

	
	--e = (a > b and c) or d

	self.effect = (v == true and 1) or 0;
	-- print(self.effect);
end

local function f_btnScaleEnd(self)
	self.img:set_size(self.oldw,self.oldh);
	self.img:set_pos(self.oldx,self.oldy);
end

local function f_click(name,self)
	if(self.callBack) then
		self.callBack(self.param);
	end

	--实现按钮点击下去的缩放效果
	if(self.effect == 1) then
		local w,h=self.img:get_size();
		local scale = 0.95;
		self.img:set_size(self.oldw*scale,self.oldh*scale);
		self.img:set_pos(self.oldx+w*(1-scale)/2,self.oldy+h*(1-scale)/2);
		core.setTimeout(50,f_btnScaleEnd,self);
	end
end

function Button:bind_click(callBack,param)
	self.param = param;
	self.callBack = callBack;
	self.img:on(EVENT_ENGINE_SPRITE_CLICK,f_click,self);
end

function Button:dispose()
	self.img:off(EVENT_ENGINE_SPRITE_CLICK,f_click);	
	self.img:dispose();
	if(self.label)then
		self.label:dispose();
	end
	func_clearTableItem(self);
end
--设置按钮的坐标
function Button:set_pos(x,y)
	self.oldx = x;
	self.oldy = y;
	self.img:set_pos(x,y);
end

function Button:visible(v)
	self.img:visible(v);
	if(self.label) then
		self.label:visible(v);
	end
end