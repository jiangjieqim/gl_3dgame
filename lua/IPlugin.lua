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
---@param centerFlag 是否居中显示
---@param nskin 皮肤句柄
---@param data 数据
---@param mode 打开的方式
IPluginView = {
	
};

IPluginView.__index = IPluginView;
setmetatable(IPluginView, IPlugin);

--显示/隐藏插件
-- function IPluginView:visible(v)
--     self.nskin:visible(v);
-- end

--界面初始化加载完成之后会调用此接口,这时候视图已经全部构建完成
--可以在这里添加界面组件的各种监听和交互代码
function IPluginView:initialize()
	func_error("未实现initialize接口!!!,请在IPluginView的子类实现之.");
end

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
	if(self.nskin) then
		self.nskin:visible(false);
	else 
		func_error("self.nskin is nil!");
	end
	evt_dispatch(self,EVENT.UNDISPLAY,self);
end

--插件是否显示着
function IPluginView:is_visible()
	if(self.nskin == nil) then
		func_error("self.nskin = nil");
	end
 	return self.nskin:is_visible();
end

--居中
function IPluginView:center()
	self.nskin:center();
end

function IPluginView:setCenter(v)
	self.centerFlag = v;
end

--保存数据
function IPluginView:setData(data,mode)
	self.data = data;
	self.mode = mode;
end

-- --设置数据,刷新视图
-- function IPluginView:setData(data)
	
-- end

--刷新视图
function IPluginView:refreshView()
end

--- 根据mode显示界面
function IPluginView:showByMode(mode)
	if(mode == 0) then
		if(self:is_visible()==true) then
			self:hide();
		else
			self:show();
			self:refreshView();
		end
	elseif(mode == -1) then
		self:hide();
	elseif(mode == 1) then
		self:show();
		self:refreshView();
	end
end
---@param xml string XML配置
---@param tex string 贴图配置
function IPluginView:load(xml,tex)
	local nskin = NSkin:new();
    -- print("AvatarView:new====>",nskin:isSkinLoaded());
	self.nskin = nskin;
	local function f_cpmleteHandler(skin)
		-- init(self);
		-- self:showByMode();
		self:initialize();
		self:refreshView();
    end
	evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmleteHandler,self);
	nskin:load(xml,tex);
end