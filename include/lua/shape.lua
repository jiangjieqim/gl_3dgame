Shape = {
	w,h,
	container,--Sprite对象
}

Shape.__index = Shape;
setmetatable(Shape, Base);

function Shape:new(issuper,w,h)
	w = w or 32;
	h = h or 32;
	local self = Base:new();
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
	--self.material = material;
	
	--self:setcolor(1.0);
end


--设置颜色
function Shape:setcolor(r,g,b)
	--func_error();
	
	r = r or 0;
	g = g or 0;
	b = b or 0;
	local str = string.format("_lineColor,%f,%f,%f",r,g,b);
	local m = sprite_get_material(self.container);
	glsl_set(m,str);
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

--当前的Shape的宽高
function Shape:get_size()
	--func_error("*");
	return self.w,self.h;
end
--获取Shape的坐标
function Shape:get_pos()
	local p = self:get_container();
	
	--func_error(1);

	--print("*"..tostring(p));
	return get_attr(p,"spritePos");
end

--设置为线框渲染
function Shape:drawPloygonLine(v)
	if(v) then
		setv(self:get_container(),FLAGS_DRAW_PLOYGON_LINE);
	else
		resetv(self:get_container(),FLAGS_DRAW_PLOYGON_LINE);
	end
end

--获取Shape当前容器
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
	
	--func_error(string.format("%s,%s",tostring(self),tostring(v)));
end

function Shape:on(id,func,params)
	evt_on(self.container,id,func,params);
end

function Shape:off(id,func)
	evt_off(self.container,id,func);
end

--销毁
function Shape:dispose()
	ptr_remove(self.container);
	func_clearTableItem(self);
end

--设置Shape的宽度
function Shape:set_width(w)
	self.w = w;
	change_attr(self.container,"sprite_set_width",w);
end

--设置Shape的高度
function Shape:set_height(h)
	self.h = h;
	change_attr(self.container,"sprite_set_height",h);	
end

--设置Shape的宽高
function Shape:set_size(w,h)
	self.w = w;
	self.h = h;
	func_set_sprite_size(self.container,w,h);
end

--将一个sprite添加到该shape中,使其作为子对象
function Shape:addChild(sprite,x,y)
	func_addchild(self:get_container(),sprite,x,y);
end

--设置拖拽的方向
function Shape:set_drag_type(v)
	sprite_set_direction(self:get_container(),v);
end

--获取拖拽的方向
function Shape:get_drag_type()
	return get_attr(self:get_container(),"dragDirection");
end

--设置可拖拽范围
function Shape:set_drag_rect(x,y,w,h)
	--func_error();
	sprite_setDragScope(self:get_container(),x,y,w,h);
end

--获取鼠标拾取的sprite相对坐标(局部local坐标)
function Shape:local_mouse_xy()
	return func_get_sprite_mouse_xy(self.container);
end
--设置可点击区域的范围
function Shape:hit_rect(x,y,w,h)
	sprite_set_hit_rect(self:get_container(),x,y,w,h);
end