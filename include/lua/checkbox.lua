CheckBox = {
	selected = false,
	btn,
	container,
	callback,
	param
};
CheckBox.__index = CheckBox;

local function ckfunc(btnName,p)
	local self= p;
	self.selected = not self.selected;
	
	local s = btn_get_container(self.btn);
	
	if(self.selected)then
		func_setIcon(s,"dagou.png");
	else
		func_setIcon(s,"checkbox.png");
	end
	self.callback(self.selected,self.param);
end

function CheckBox:new()
	local self = {};
	setmetatable(self, CheckBox);
	self.btn = btn_create(0,0,20,20,"checkbox.png",0,false);
	--self.container = --btn_get_container(self.btn);
	self.container = self.btn;
	
	return self;
end

function CheckBox:bind(callback,param)
	self.callback = callback;
	if(callback) then
		self.param = param;
		btn_bindClick(self.btn,ckfunc,self);
	end
end

function CheckBox:dispose()
	btn_dispose(self.btn);
	func_clearTableItem(self);
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