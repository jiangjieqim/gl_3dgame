CheckBox = {
	selected = false,
	btn,--ButtonÒýÓÃ
	callback,
	param,
};
CheckBox.__index = CheckBox;
setmetatable(CheckBox, Base);

local function ckfunc(p)
	local self= p;
	self.selected = not self.selected;
	
	local s =  self.btn:get_container();--btn_get_container(self.btn);
	
	if(self.selected)then
		func_setIcon(s,"dagou.png");
	else
		func_setIcon(s,"checkbox.png");
	end
	if (self.callback~=nil) then
		self.callback(self.selected,self.param);
	end
end

function CheckBox:get_container()
	return self.btn:get_container();
end

function CheckBox:new()
	local self =  Base:new();
	self:settype(6);
	setmetatable(self, CheckBox);
	self.btn =  Button:new(20,20,"checkbox.png");
	self.btn:bind_click(ckfunc,self);
	return self;
end

function CheckBox:setlabel(label)
	self.btn:set_label(label,20,0);
end

function CheckBox:bind(callback,param)
	self.callback = callback;
	self.param = param;
end

function CheckBox:dispose()
	--print("Ïú»ÙCheckBox");
	self.btn:dispose();
	func_clearTableItem(self);
end

function CheckBox:visible(v)
	self.btn:visible(v);
end

--[[

	local function onCk(_status,p)
			print(_status,p);
	end
	local ck = CheckBox:new();
	ck:bind(onCk,12);
	
	func_addnode(container,ck.container,120);
	ck:dispose();

--]]