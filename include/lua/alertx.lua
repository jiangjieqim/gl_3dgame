local function loadCallBack(obj,param)
	
	local self = param.self;
	--print("alert初始化完成",obj,self);
	
	alert_set_drag(self,true);
	alert_enable_resize(self,true);
	
	alert_add_closebtn(self);
	alert_set_label(self,param.str);
	
	--alert_dispose(self);
end

local _alert1;

function alert(str)
	if(_alert1~=nil)then
		alert_set_label(_alert1,str);
		alert_show(_alert1);
	else
		_alert1 = alert_init();
		evt_once(_alert1,EVENT_ENGINE_COMPLETE,loadCallBack,{self=_alert1,str=str});
		alert_start(_alert1);
	end
end