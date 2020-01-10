dofile("..\\include\\lua\\IPlugin.lua")	--插件接口定义

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

--加载插件
function PluginMan:load(plugin)
	local url = "../include/lua/"..plugin..".lua";
	local p = dofile(url);
	--stack_push(self.list,p);
	func_print(">>>>加载插件"..p:getName()..url);
	return p;
end

--卸载插件
function PluginMan:unload(p)
	func_print("<<<<卸载插件"..p:getName());
	p:unload();
	p = nil;
end