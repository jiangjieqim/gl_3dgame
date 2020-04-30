local EVENT = require("event");

--插件接口定义
IPlugin = {
	
};

IPlugin.__index = IPlugin;

--插件加载
function IPlugin:new()
	func_error("IPlugin:new()接口未实现");
end

--插件卸载,此接口会卸载并销毁该插件所有的资源
function IPlugin:dispose()
	func_error("IPlugin:dispose()接口未实现");
end

function IPlugin:getName()
	func_error("IPlugin:getName()接口未实现");	
end
--[[
	视图类型的插件继承此类
]]
IPluginView = {
	nskin = nil,
	centerFlag,--是否居中显示
};

IPluginView.__index = IPluginView;
setmetatable(IPluginView, IPlugin);

--显示/隐藏插件
-- function IPluginView:visible(v)
--     self.nskin:visible(v);
-- end

--v = true居中
function IPluginView:show()
	if(self.centerFlag) then
		self.nskin:center();
	end
	-- self:onDisplay();
	self.nskin:visible(true);
	evt_dispatch(self,EVENT.DISPLAY,self);
end

function IPluginView:hide()
	-- self:unDisplay();
	self.nskin:visible(false);
	evt_dispatch(self,EVENT.UNDISPLAY,self);
end

--插件是否显示着
function IPluginView:is_visible()
 	return self.nskin:is_visible();
end

--居中
function IPluginView:center()
	self.nskin:center();
end


function IPluginView:setCenter(v)
	self.centerFlag = v;
end
--设置数据
function IPluginView:setData(data)
end