-------------------------------------------------
JEngine ={
	atals,
	cam2d,
	cam3d,
};

JEngine.__index = JEngine;
setmetatable(JEngine, Instance);--¼Ì³Ð×Ôµ¥Àý

function JEngine:new()
	local self =  Instance:new();
	setmetatable(self, JEngine);
	local uiAtals=atals_load("//resource//texture//","1");
    self.atals = uiAtals;
	
	
	local cam2d,cam3d = getEngine();
	self.cam2d = cam2d;
	self.cam3d = cam3d;
	return self;
end
function JEngine:get_cam()
    return  self.cam2d,self.cam3d;
end
function JEngine:get_atals()
    return self.atals;
end

function JEngine:dispose()
	atals_dispose(self.atals);
	func_clearTableItem(self);
end

function JEngine:new_z()
	 return get_attr(nil,"newPosZ");
end

function JEngine:rename(o,value)
	change_attr(o,"rename",tostring(value));
end

function JEngine:refresh_cam3d()
	change_attr(nil,"cam_refreshModel",self.cam3d);
end
function JEngine:get_fps()
    return get_attr(nil,"fps");
end

function JEngine:get_delayTime()
	return get_attr(nil,"delayTime");
end

function JEngine:exit()
	change_attr(nil,"exit");
end

function JEngine:setbg(r,g,b)
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end

function JEngine:bind_3dcam(o)
	set_cam(o,self.cam3d);
end

function JEngine:add(n)
	change_attr(nil,"ex_add",n);
end
