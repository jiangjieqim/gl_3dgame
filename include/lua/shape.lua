Shape = {
	name = nil,
	type = nil,
	w,h,
	container,--Sprite对象

	material,
}

Shape.__index = Shape;

function Shape:new(issuper,w,h)
	w = w or 32;
	h = h or 32;
	local self = {};
	setmetatable(self, Shape);
	
	self.w = w;
	self.h = h;
	if(issuper) then
		self:init();
	end
	
	return self;
end

function Shape:init()
	self:settype(11);
	local sprite = sprite_create_typical(nil,0,0,self.w,self.h);
	self.container = sprite;
	engine_addNode(sprite);	
	local material = func_load("//resource//material//shape.mat");
	setMaterial(self.container,material);
	self:mouseEnable(false);--默认关闭鼠标事件
	self.material = material;
	
	--self:setcolor(1.0);
end

function Shape:setname(name)
	self.name = name;
end

--设置颜色
function Shape:setcolor(r,g,b)
	r = r or 0;
	g = g or 0;
	b = b or 0;
	local str = string.format("_lineColor,%f,%f,%f",r,g,b);
	glsl_set(self.material,str);
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

--销毁
function Shape:dispose()
	ptr_remove(self.container);
	func_clearTableItem(self);
end
