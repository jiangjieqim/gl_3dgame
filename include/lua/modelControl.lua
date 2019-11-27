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
	label_set_text(label,v);--设置文本
	
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
local function f_model_fps_sc_handle(sc)
	local self = scrollBar_get_param(sc);
		
	local cur = sc.value * 60;
	
	change_attr(unit_get_model(self.obj), "fps", tostring(cur));
	local label = skin_find(self.skin,"model_fps_label");
	
	local obj =	unit_get_model(self.obj)
	func_changeFlags(obj, FLAGS_DISABLE_CULL_FACE);--设置显示双面
end

local function f_list_box_select(list,index,label,param)
	--local index =listbox_g	label_set_text(label,math.floor(cur));
end
local function onCk(_status,p)
	---print("CheckBox call onCk",_status,p);
	local self = p;
	local obj =	unit_get_model(self.obj)
	func_changeFlags(obj, FLAGS_DRAW_PLOYGON_LINE);--线框切换
end

local function onCullCk(_status,p)
	local self = p;
et_index(list);
	print("f_list_box_select===>",list,index,label,param);
	--listbox_set_title(list,label);
end



local function f_camx_handle(sc)
	local cur = sc.value * 1;
	--print(cur);
	cam_setPosition(cur*5,0,-10);
	engine_refreshCam3d();
end

local function fc(progress,p)
	--print(progress);
end
local function f_nl(self,index,param)
	print(self,index,param);
end
local function f_callBack(skin,self)
	
	

--local img = Image:new(50,50);
--img:seticon("smallbtn.png");	

	
	--local shape = Shape:new(true);
	--shape:dispose();
	
	
	
	
	local p = skin_get_param(skin);
	--func_print(string.format("****************[%s]",tostring(p)));
	
	--local btn = skin_find(skin,"3");
	--btn_bindClick(btn,btnClick,"abc");

	local sc = skin_find(skin,"rx_sc");
	local rxlabel = skin_find(skin,"rx_label");
	self.rxlabel = rxlabel;
	scrollBar_bind(sc,f_rx_handle,self);
	
	--skin_dispose(skin);
	
	--print(self,self.x,self.y);
	scrollBar_bind(skin_find(skin,"ry_sc"),f_ry_handle,self);
	
	scrollBar_bind(skin_find(skin,"rz_sc"),f_rz_handle,self);

	scrollBar_bind(skin_find(skin,"scale_sc"),f_scale_handle,self);

	scrollBar_bind(skin_find(skin,"model_fps_sc"),f_model_fps_sc_handle,self);
	
	
	
	local ck = skin_find(skin,"ck0");
	--print(ck);
	ck:bind(onCk,self);
	
	local cullck = skin_find(skin,"cullck");
	cullck:bind(onCullCk,self);
	
	
	--local pb = ProgressBar:new(100);
	--pb:progress(0.5);
	--pb:dispose();
	
	local pb1 = skin_find(skin,"pb1");
	pb1:progress(0.6);
	
	local lb1 = skin_find(skin,"lb1");
	listbox_bind(lb1,f_list_box_select,"my_param");
	--listbox_set_title(lb1,"infowin");
	
	local camx_sc = skin_find(skin,"camx_sc");
	scrollBar_bind(camx_sc,f_camx_handle,self);
	

	--设置界面的初始坐标
	skin_set_pos(skin,self.x,self.y);

	
	local sc1 = skin_find(skin,"sc1");	
	sc1:bindCallback(fc,1);
	--sc:dispose();	
	
	local nl = skin_find(skin,"nb1");--NListBox:new();
	nl:addItem("a");
	nl:addItem("b");
	nl:bind(f_nl,9);
	nl:selectIndex(1);

--*******************
	--local a = NLabel:new();
	--a:set_text("k我amkoa");
	--a:set_pos(10,10);
	--print(a:get_text());
	--local w,h = a:get_size();
	--print(w,h);
	--a:dispose();
	
	
	
	--销毁skin组件
	--skin_dispose(skin);
end

ModelControl.__index = ModelControl;
function ModelControl:new(x,y)
	local self = {} 
	setmetatable(self, ModelControl) 
	self.x = x;  
	self.y = y;
	self.rxlabel = nil;
	self.obj = nil;
	self.skin = nil;
	
--	self.bind = bind;
	--print(self);
	self.skin=skin_load("crl.xml",f_callBack,self);

	return self;
end

function ModelControl:bind(obj)
	self.obj = obj;
end
--[[
<ui name="3" type="Button" x="150" y="50" parent="1" label="aaa"/>
<ui name="5" type="Button" x="50" y="100" parent="1" label="按钮文本"/>
<ui name="6" type="Skin" x="150" y="100" url="scrollabel.xml" parent="1" />
--]]