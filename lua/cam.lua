Camera = {
	p,--camera句柄
};

Camera.__index = Camera;

function Camera:new(p)
	local self = {};
	setmetatable(self,Camera);
	self.p = p;
	return self;
end

--设置camera的坐标
function Camera:set_pos(x,y,z)
	-- print(x,y,z);
	cam(self.p,"set_xyz",x or 0,y or 0,z or 0);
	self:refresh();
end

--重置camera
function Camera:reset()
	cam(self.p,"reset");
end

--获取camera的C层的句柄
function Camera:get_p()
	return self.p;
end

--刷新model矩阵
function Camera:refresh()
	cam(self.p,"refresh");
end

--设置camera的rx的值
function Camera:rx(v)
	cam(self.p,"rx",v);
	self:refresh();
end

--销毁cam
function Camera:dispose()
	func_clearTableItem(self);
end

--绑定一个对象到cam
function Camera:bind(o)
	set_cam(o,self.p);
end
