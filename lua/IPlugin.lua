local EVENT = require("event");

--����ӿڶ���
IPlugin = {
	
};

IPlugin.__index = IPlugin;

--�������
function IPlugin:new()
	func_error("IPlugin:new()�ӿ�δʵ��");
end

--���ж��,�˽ӿڻ�ж�ز����ٸò�����е���Դ
function IPlugin:dispose()
	func_error("IPlugin:dispose()�ӿ�δʵ��");
end

function IPlugin:getName()
	func_error("IPlugin:getName()�ӿ�δʵ��");	
end
--[[
	��ͼ���͵Ĳ���̳д���
]]
---@param centerFlag �Ƿ������ʾ
---@param nskin Ƥ�����
---@param data ����
---@param mode �򿪵ķ�ʽ
IPluginView = {
	
};

IPluginView.__index = IPluginView;
setmetatable(IPluginView, IPlugin);

--��ʾ/���ز��
-- function IPluginView:visible(v)
--     self.nskin:visible(v);
-- end

--�����ʼ���������֮�����ô˽ӿ�,��ʱ����ͼ�Ѿ�ȫ���������
--������������ӽ�������ĸ��ּ����ͽ�������
function IPluginView:initialize()
	func_error("δʵ��initialize�ӿ�!!!,����IPluginView������ʵ��֮.");
end

--v = true����
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

--����Ƿ���ʾ��
function IPluginView:is_visible()
	if(self.nskin == nil) then
		func_error("self.nskin = nil");
	end
 	return self.nskin:is_visible();
end

--����
function IPluginView:center()
	self.nskin:center();
end

function IPluginView:setCenter(v)
	self.centerFlag = v;
end

--��������
function IPluginView:setData(data,mode)
	self.data = data;
	self.mode = mode;
end

-- --��������,ˢ����ͼ
-- function IPluginView:setData(data)
	
-- end

--ˢ����ͼ
function IPluginView:refreshView()
end

--- ����mode��ʾ����
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
---@param xml string XML����
---@param tex string ��ͼ����
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