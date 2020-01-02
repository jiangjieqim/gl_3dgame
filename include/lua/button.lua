Button = {
	img, -- Image
	label,-- NLabel
	
	callBack,--回调函数
	param,--回调函数的参数
};

Button.__index = Button;

setmetatable(Button, Base);

function Button:new(w,h,url)
	local self = Base:new();
	setmetatable(self, Button);
	local img = Image:new(w or 80,h or 18);
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

--设置按钮效果
function Button:btn_effect(v)
	local container = self.img:get_container();
	if(v) then
		setv(container,FLAGS_BUTTON_EFFECT);
	else
		resetv(container,FLAGS_BUTTON_EFFECT);
	end
end

local function f_click(name,self)
	if(self.callBack) then
		self.callBack(self.param);
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

function Button:set_pos(x,y)
	self.img:set_pos(x,y);
end

function Button:visible(v)
	self.img:visible(v);
	if(self.label) then
		self.label:visible(v);
	end
end