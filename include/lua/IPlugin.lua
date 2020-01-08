--插件接口定义
IPlugin = {
	
};

IPlugin.__index = IPlugin;

--插件加载
function IPlugin:load()
	func_error("IPlugin:load()接口未实现");
end

--插件卸载,此接口会卸载并销毁该插件所有的资源
function IPlugin:unload()
	func_error("IPlugin:unload()接口未实现");
end
