--[[
--参考范例


local s= LineNode:new(3);
s:push(-0.5,0.4,0.5);
s:push( 0.5, 0.0, 0.5);
s:push( -0.5, -0.49, -0.5);
s:setcolor(1,0,0);
s:graphics_end();
--s:dispose();--销毁

]]

--绘制自定义的线段
LineNode = {
	
};
LineNode.__index = LineNode;

--isPoint = true 使用点模式渲染
function LineNode:new(cnt,isPoint,name)
	local self = {
		p,--C层的linenode指针
	};
	setmetatable(self, LineNode);
	local _type = 0;
	if(isPoint) then
		_type = 1;
	end
	
	self.p=linenode("create",name or func_create_name(),cnt,_type);
	
	if(isPoint) then
		self:setcolor(1,1,0);
	else
		self:setcolor(1,0,0);
	end
	return self;
end

--[[
	修改对应索引的坐标
	index 从0开始
--]]
function LineNode:mod(index,x,y,z)
	linenode("mod",self.p,index,x,y,z);
end

function LineNode:push(x,y,z)
	linenode("push",self.p,x,y,z);
end

--绘制结束
function LineNode:graphics_end()
	linenode("end",self.p);
end

function LineNode:setcolor(r,g,b)
	linenode("setcolor",self.p,r,g,b);
end

function LineNode:dispose()
	linenode("dispose",self.p);
end

