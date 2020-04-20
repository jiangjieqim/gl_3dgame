require("IPlugin")	--插件接口定义

--print("插件管理器初始化");
PluginMan = {
	--list,--插件列表
};
PluginMan.__index = PluginMan;
function PluginMan:new()
	local self = {};
	setmetatable(self, PluginMan);
	--self.list = stack_new();
	return self;
end

function PluginMan:getInfo(p)
	return p:getName().." "..tostring(p);
end

--加载插件

function PluginMan:load(plugin)
	-- local url = "../include/lua/"..plugin.."";
	local p = require(plugin):new();
	func_print(">>>>插件初始化完毕"..self:getInfo(p).." url="..plugin);
	return p;
end

--卸载插件
--内存泄漏,废弃之
--[[
function PluginMan:unload(p)
	p:unload();
	func_print("<<<<卸载插件"..f_getInfo(p));	
	
	--setmetatable(getmetatable(p),nil);
	--setmetatable(p, nil);
	
	--print(getmetatable(p));
end
--]]

