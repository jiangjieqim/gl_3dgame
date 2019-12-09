--[[
--参考范例


local s= LineNode:new(3);
s:push(-0.5,0.4,0.5);
s:push( 0.5, 0.0, 0.5);
s:push( -0.5, -0.49, -0.5);
s:pushend();
--s:dispose();--销毁

]]

--绘制自定义的线段
LineNode = {
	p,
}
LineNode.__index = LineNode;

function LineNode:new(cnt,name)
	local self = {};
	setmetatable(self, LineNode);
	self.p=linenode("create",name or func_create_name(),cnt);
	return self;
end

function LineNode:push(x,y,z)
	linenode("push",self.p,x,y,z);
end

function LineNode:pushend()
	linenode("end",self.p);
end

function LineNode:dispose()
	linenode("dispose",self.p);
end

