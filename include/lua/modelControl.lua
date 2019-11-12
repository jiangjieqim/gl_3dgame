--print("***");

ModelControl = {x=0,y=0};


local function btnClick(btnName,p)
		print("btnName=",btnName,"p=",p);--p = abc
end

local function f_rx_handle(sc)
   -- func_rotate(crl.o, key, sc.value);
	--print(sc.value);
	local self = scrollBar_get_param(sc);
	local label = self.rxlabel;
    local v = string.format("%.1f",sc.value);
	label_set_text(label,v);--…Ë÷√Œƒ±æ
	
	func_rotate(unit_get_model(self.obj), "rx", sc.value*PI);
	--print(sc.value*PI);
end

local function f_ry_handle(sc)
	local self = scrollBar_get_param(sc);
	func_rotate(unit_get_model(self.obj), "ry", sc.value*PI);
end

local function f_rz_handle(sc)
	local self = scrollBar_get_param(sc);
	func_rotate(unit_get_model(self.obj), "rz", sc.value*PI);
end
local function f_scale_handle(sc)
	local self = scrollBar_get_param(sc);
	func_rotate(unit_get_model(self.obj), "scale", sc.value);
end

local function f_callBack(skin,self)
	local p = skin_get_param(skin);
	--func_print(string.format("****************[%s]",tostring(p)));
	
	local btn = skin_find(skin,"3");
	btn_bindClick(btn,btnClick,"abc");

	local sc = skin_find(skin,"rx_sc");
	local rxlabel = skin_find(skin,"rx_label");
	self.rxlabel = rxlabel;
	scrollBar_bind(sc,f_rx_handle,self);
	
	--skin_dispose(skin);
	
	--print(self,self.x,self.y);
	scrollBar_bind(skin_find(skin,"ry_sc"),f_ry_handle,self);
	
	scrollBar_bind(skin_find(skin,"rz_sc"),f_rz_handle,self);

	scrollBar_bind(skin_find(skin,"scale_sc"),f_scale_handle,self);

	skin_set_pos(skin,self.x,self.y);

end

ModelControl.__index = ModelControl;
function ModelControl:new(x,y)
	local self = {} 
	setmetatable(self, ModelControl) 
	self.x = x;  
	self.y = y;
	self.rxlabel = nil;
	self.obj = nil;
	
--	self.bind = bind;
	--print(self);
	skin_load("crl.xml",f_callBack,self);

	return self;
end

function ModelControl:bind(obj)
	self.obj = obj;
end












