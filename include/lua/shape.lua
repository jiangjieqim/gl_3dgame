Shape = {
	name = nil,
	type = nil,
	w,h,
	container,--Sprite对象
}

Shape.__index = Shape;

function Shape:new(w,h)
	w = w or 64;
	h = h or 64;
	local self = {};
	setmetatable(self, Shape);
	
	self.w = w;
	self.h = h;
	
	return self;
end

function Shape:init()
	--print(1);
	
	self:mouseEnable(false);--默认关闭鼠标事件
end


function Shape:settype(t)
	--print(t);
	self.type = t;	
end

function Shape:gettype()
	return self.type;
end

function Shape:visible(v)
	if(v) then
		setv(self.container,FLAGS_VISIBLE);
	else
		resetv(self.container,FLAGS_VISIBLE);
	end
end

function Shape:is_visible()
	return getv(self.container,FLAGS_VISIBLE) == 1;
end

function Shape:getsize()
	return self.w,self.h;
end

function Shape:get_container()
	return self.container;
end

function Shape:set_pos(x,y)
	func_set_local_pos(self.container,x,y);
end

function Shape:mouseEnable(v)
	local _status = v == true and 1 or 0;
	--print(status);
	sprite_enable_click(self.container,_status);
end

function Shape:on(id,func,params)
	evt_on(self.container,id,func,params);
end

function Shape:off(id,func)
	evt_off(self.container,func);
end