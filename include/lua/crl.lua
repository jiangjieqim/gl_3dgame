local function f_dispose(self)
	fext_dispose(self.nameTf);
end

local TYPE_LABEL = 1;






local function f_create_node(t,node)
	local n = {
		node = node;
		t = t;
	};
	--print(n.node,n.t);
	return n;
end

local function f_add_rotate(key, x, y)
    local sc = scrollBar_new(x, y);
    local function f_animscHandle(sc)
       -- func_rotate(crl.o, key, sc.value);
		print(sc.value);
    end

    scrollBar_bind(sc, f_animscHandle);
    scrollBar_setRange(sc, 0, PI)
    scrollBar_label(sc, key);
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
	
	local function f_delLabel(n)
--		local n = node.node;
		
		if(n.t == TYPE_LABEL) then
			func_sprite_removechild(container,ftext_get_container(n.node));
			fext_dispose(n.node);
		end
		
		
		--[[if(n.t == TYPE_LABEL) then
			func_sprite_removechild(container,ftext_get_container(n));
			fext_dispose(n);
		end--]]
	end	
	
	alert_set_drag(self,true);
	alert_enable_resize(self,true);
	
	--alert_add_closebtn(self);
	
	
	--alert_set_label(self,param.str);
	
	--alert_dispose(self);--]]
	
	--###################################
	local nameTf =  ftext_create(150, 150);
	ftext_reset(nameTf, "信息");
	func_addchild(container,ftext_get_container(nameTf));
	stack_push(labelStack,f_create_node(TYPE_LABEL,nameTf));
	
	
	--清理label列表
	--stack_foreach(labelStack,f_delLabel);
	
	--f_add_rotate("rx",0,20);
	
	
	--f_dispose(param);
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

