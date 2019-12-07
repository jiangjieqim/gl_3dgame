--设置sprite线框
function func_set_sprite_line(v)
	change_attr(nil,"g_sprite_line",tostring(v));
end


local EX = {}
function EX:new()
	local e = {
        spriteline;
	}
	
	self.__index = self;
    setmetatable(e, self);
    e.spriteline = 0;
    return e;
end

--设置sprite线框渲染
function EX:set_spriteline(v)
    self.spriteline = change_attr(nil,"g_sprite_line",tostring(v));
end
function EX:switch_spriteline()
    self:set_spriteline(self:get_spriteline() == 1 and 0 or 1);
end
function  EX:get_spriteline()
    return self.spriteline;
end






ex = EX:new();











