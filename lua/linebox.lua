LineBox = {
    
};
LineBox.__index = LineBox;

local function f_draw(line,r)
    line:push(-r,0,r);
    line:push(r,0,r);
    line:push(r,0,-r);
    line:push(-r,0,-r);
    line:push(-r,0,r);

	line:setcolor(1,0,0);
	line:graphics_end();
end

--设置坐标
function LineBox:setPos(x,y,z)    
    local r = self.radius;
    
    local line = self.p;
    x = x or 0;
    y = y or 0;
    z = z or 0;
    self.x = x;
    self.y = y;
    self.z = z;

    line:mod(0,x-r,y,z+r);
    line:mod(1,x+r,y,z+r);
    line:mod(2,x+r,y,z-r);
    line:mod(3,x-r,y,z-r);
    line:mod(4,x-r,y,z+r);
end
--设置半径
function LineBox:setRadius(r)
    r = r or 0.5;
    self.radius = r;
    self:setPos(self.z, self.y, self.z);
end
--  r:线框立方体的半径
function LineBox:new(r)
    local self = {
        p,--LineNode引用
        radius = 0.5,--半径
        x = 0,
        y = 0,
        y = 0,
    };
    setmetatable(self, LineBox);
    local line = LineNode:new(5);
    self.p = line;
    self.radius = r;
    f_draw(line,r);
    return self;
end
--销毁
function LineBox:dispose()
	self.p:dispose();
end
return LineBox;
