CheckBox = {
	selected = false,
	btn,
	container,
	callback,
	param,
};
CheckBox.__index = CheckBox;
setmetatable(CheckBox, Base);

local function ckfunc(btnName,p)
	local self= p;
	self.selected = not self.selected;
	
	local s = btn_get_container(self.btn);
	
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
	return  btn_get_container(self.btn);
end

function CheckBox:new()
	local self =  Base:new();
	self:settype(6);
	setmetatable(self, CheckBox);
	self.btn = btn_create(0,0,20,20,"checkbox.png",0,false);
	self.container = btn_get_container(self.btn);
	--self.container = self.btn;
	btn_bindClick(self.btn,ckfunc,self);
	return self;
end

function CheckBox:setlabel(label)
	btn_label(self.btn,label,"rightout");
end
--[[--设置CheckBox的名字
function CheckBox:setname(name)
	self.name = name;
end--]]
function CheckBox:bind(callback,param)
	self.callback = callback;
	self.param = param;
end

function CheckBox:dispose()
	--print("销毁CheckBox");
	btn_dispose(self.btn);
	func_clearTableItem(self);
end

function CheckBox:visible(v)
	btn_visible(self.btn,v);
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