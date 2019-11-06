
local function f_delAll(n)
	func_dispose(n);
end	
local function f_animscHandle(sc)
   -- func_rotate(crl.o, key, sc.value);
	--print(sc.value);
	
	local label = scrollBar_get_param(sc);
	label_set_text(label,sc.value);
end

local function loadCallBack(obj,param)
	---[[
	local self = param.alert;
	
	local labelStack = param.labelStack;
	--print("alert初始化完成",obj,self);
	
	local container = alert_get_container(self);
	
	alert_set_drag(self,true);
	alert_enable_resize(self,true);
	
	--alert_add_closebtn(self);
	
	
	--alert_set_label(self,param.str);
	
	--alert_dispose(self);--]]
	
	--###################################
	local nameTf =  label_create();
	label_set_text(nameTf, "信息");
	func_addnode(container,nameTf);
	stack_push(labelStack,nameTf);
	
	--local sc =f_add_rotate(0,0,container,nameTf);
	
	local sc = scrollBar_new();
    scrollBar_bind(sc, f_animscHandle,nameTf);
	func_addnode(container,sc,10,20);
	stack_push(labelStack,sc);
	
	local btn = btn_create();
	btn_label(btn,"Button");
	func_addnode(container,btn,0,40);
	stack_push(labelStack,btn);

	
	--清理label列表
	--stack_foreach(labelStack,f_delAll);
end

function crl_init()
	local ins = {
		alert,
		
		--label
		--nameTf,
		
		
		labelStack,
		--spriteStack,
		
	}
	
	ins.alert = alert_init();
	
	evt_once(ins.alert,EVENT_ENGINE_COMPLETE,loadCallBack,ins);
	alert_start(ins.alert);
	
	
	ins.labelStack=stack_new();
	
	return ins;
end

