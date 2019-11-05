local TYPE_LABEL = 1;
local TYPE_SCROLL_BAR = 2;

local function f_create_node(t,node)
	local n = {
		node = node;
		t = t;--类型
	};
	--print(n.node,n.t);
	return n;
end

local function f_delAll(n,index,p)
		local container = p;
		local _type = n.t;
		--print(n.t,n.node);		
		
		if(_type == TYPE_LABEL) then
			func_sprite_removechild(container,ftext_get_container(n.node));
			fext_dispose(n.node);
		elseif(_type == TYPE_SCROLL_BAR) then
			scrollBar_del(n.node);
		end
		
		
		--[[if(n.t == TYPE_LABEL) then
			func_sprite_removechild(container,ftext_get_container(n));
			fext_dispose(n);
		end--]]
	end	

local function f_add_rotate(key, x, y,container)
    local sc = scrollBar_new(x, y,container);
    local function f_animscHandle(sc)
       -- func_rotate(crl.o, key, sc.value);
		--print(sc.value);
    end

    scrollBar_bind(sc, f_animscHandle);
    --scrollBar_setRange(sc, 0, PI)
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
	
	alert_set_drag(self,true);
	alert_enable_resize(self,true);
	
	--alert_add_closebtn(self);
	
	
	--alert_set_label(self,param.str);
	
	--alert_dispose(self);--]]
	
	--###################################
	local nameTf =  ftext_create(150, 150);
	ftext_reset(nameTf, "信息");
	func_addchild_label(container,nameTf);
	stack_push(labelStack,f_create_node(TYPE_LABEL,nameTf));
	
	local sc =f_add_rotate("rx",10,20,container);
	--func_addchild_scrollBar(container,sc,10,20);
	--scrollBar_pos(sc,10,10);
	stack_push(labelStack,f_create_node(TYPE_SCROLL_BAR,sc));
	
	--清理label列表
	--stack_foreach(labelStack,f_delAll,container);
	
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

