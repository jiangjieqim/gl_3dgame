local function f_dispose(self)
	fext_dispose(self.nameTf);
end

local function loadCallBack(obj,param)
	
	---[[
	local self = param.alert;
	--print("alert初始化完成",obj,self);
	
	local container = alert_get_container(self);
	
	alert_set_drag(self,true);
	alert_enable_resize(self,true);
	
	alert_add_closebtn(self);
	--alert_set_label(self,param.str);
	
	--alert_dispose(self);--]]
	
	--###################################
	local nameTf =  ftext_create(150, 150);
	ftext_reset(nameTf, "信息");
	func_addchild(container,ftext_get_container(nameTf));
	--print(nameTf);
	--
	
	func_sprite_removechild(container,ftext_get_container(nameTf));
	fext_dispose(nameTf);
	--f_dispose(param);
end


function crl_init()
	local ins = {
		alert,
		
		--label
		--nameTf,
		
		
		
		
	}
	
	ins.alert = alert_init();
	
	evt_once(ins.alert,EVENT_ENGINE_COMPLETE,loadCallBack,ins);
	alert_start(ins.alert);
	
	return ins;
end

