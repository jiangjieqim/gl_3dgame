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
--退出引擎
function EX:exit()
    change_attr(nil,"exit");
end


--[[
--获取fps
function func_fps()
    func_error("fps",func_fps);
    return 1;
end

--获取delayTime
function func_delayTime()
	return get_attr(nil,"delayTime");
end
function func_delayTime()
	return get_attr(nil,"delayTime");
end
--]]




--当前的fps
function EX:fps()
	return get_attr(nil,"fps");
end
--每一帧需要的毫秒数
function EX:delayTime()
    return get_attr(nil,"delayTime");
end
--设置背景色
function EX:bgColor(r,g,b)
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end
ex = EX:new();