require("IPlugin")	--插件接口定义
local EVENT = require("event")	--插件接口定义

--print("插件管理器初始化");
PluginMan = {
	--list,--插件列表
};
PluginMan.__index = PluginMan;
function PluginMan:new()
	local self = {};
	setmetatable(self, PluginMan);
	self.insList = {}--单例列表
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

-- local function f_cpmleteHandler(view)
-- 	print(1111);
-- end

--切换打开继承自IPluginView的模块
--mode :
--0 切换开启	-1	关闭 1打开
function PluginMan:toggle(plugin,mode)
	mode = mode or 0;
	local view  = self.insList[plugin];
	if(view)then
		-- print("获取的是单例"..tostring(view));
	else
		self.insList[plugin] = self:load(plugin);
		view = self.insList[plugin];
		view:hide();
	end

	if(mode == 0) then
		if(view:is_visible()) then
			view:hide();
		else
			view:show();
		end
	elseif(mode == -1) then
		view:hide();
	elseif(mode == 1) then
		view:show();
	end
	return view;
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

