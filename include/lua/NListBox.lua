--下拉列表组件
NListBox = {
	bg,--Shape
};

NListBox.__index = NListBox;
setmetatable(NListBox, Base);

local function f_click(name,self)
--	f_select_call(list);

	print(name,self);
end

function NListBox:new()
	
	local self = Base:new();
	setmetatable(self, NListBox);
	self:settype(13);
	
	local bg = Shape:new(true,60,12);
	self.bg = bg;
	bg:mouseEnable(true);
	bg:on(EVENT_ENGINE_SPRITE_CLICK,f_click,self);
	
	return self;
end

function NListBox:dispose()
	local bg = self.bg;
	bg:dispose();
	func_clearTableItem(self);
end