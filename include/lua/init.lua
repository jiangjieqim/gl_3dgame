dofile("..\\include\\lua\\core.lua");
function f_onkey(data)
	local key = tonumber(data);
	if(key == KEY_ESC) then
        ex:exit();	
	end
end
evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);

--dofile("..\\include\\lua\\7.lua");


--[[

A = {x = 0,y = 1};
A.__index = A

function A:new(x)
    local s = {};
    setmetatable(s,A);
    s.x = x or 0;
    return s;
end

function A:test()
    print(self.x);
end


local obj = A:new();
obj:test();

local obj1 = A:new(2);
obj1:test();


obj:test();
--]]



--单位基础类
UnitBase  = {
    p,--角色句柄
    speed = 1000,--角色移动的速度
    name,--角色的名字
};

function UnitBase:new()
    local s = {};
    setmetatable(s,UnitBase);
    --s.x = x or 0;
    return s;
end


