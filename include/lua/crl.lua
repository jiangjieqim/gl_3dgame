
local function f_delAll(n)
	func_dispose(n);
end	

local function f_add_rotate(key, x, y,container)
    local sc = scrollBar_new(x, y,container);
    local function f_animscHandle(sc)
       -- func_rotate(crl.o, key, sc.value);
		--print(sc.value);
    end

    scrollBar_bind(sc, f_animscHandle);
    --scrollBar_setRange(sc, 0, PI)
    --scrollBar_label(sc, key);
    --crl.sclist[key] = sc;
    --return scrollBar_get_rect(sc);
	return sc;
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
	local nameTf =  label_create(150, 150);
	label_text(nameTf, "信息");
	func_addchild_label(container,nameTf);
	stack_push(labelStack,nameTf);
	
	local sc =f_add_rotate("rx",10,20,container);
	stack_push(labelStack,sc);
	
	--清理label列表
	stack_foreach(labelStack,f_delAll);
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

