require("IPlugin")	--����ӿڶ���
local EVENT = require("event")	--����ӿڶ���

--print("�����������ʼ��");
PluginMan = {
	--list,--����б�
};
PluginMan.__index = PluginMan;
function PluginMan:new()
	local self = {};
	setmetatable(self, PluginMan);
	self.insList = {}--�����б�
	--self.list = stack_new();
	
	return self;
end
function PluginMan:getInfo(p)
	return p:getName().." "..tostring(p);
end

--���ز��

function PluginMan:load(plugin)
	-- local url = "../include/lua/"..plugin.."";
	local p = require(plugin):new();
	func_print(">>>>�����ʼ�����"..self:getInfo(p).." url="..plugin);
	return p;
end

-- local function f_cpmleteHandler(view)
-- 	print(1111);
-- end

--�л��򿪼̳���IPluginView��ģ��
--mode :
--0 �л�����	-1	�ر� 1��
function PluginMan:toggle(plugin,mode,data)
	mode = mode or 0;

	---@type IPluginView
	local view  = self.insList[plugin];
	
	if(view)then
		-- print("��ȡ���ǵ���"..tostring(view));
		if( view.nskin:isSkinLoaded() == false) then
			view:setData(data,mode);
			core.warning("����û�м������!!!!!");
			return;
		end
		view:showByMode(mode);
	else
		self.insList[plugin] = self:load(plugin);
		view = self.insList[plugin];
		view:setData(data,mode);

		-- local f = view.nskin:isSkinLoaded();
		-- print("isSkinLoaded = ",f);

		-- view:hide();
		-- print("================================> 222  mode:",mode,view:is_visible());
	end


	-- view:show();

	

	--print("mode=",mode,"is_visible = ",view:is_visible());


--[[
	if(mode == 0) then
		if(view:is_visible()) then
			view:hide();
		else
			view:show();
			view:setData(data);
		end
	elseif(mode == -1) then
		view:hide();
	elseif(mode == 1) then
		view:show();
		view:setData(data);
	end
--]]

	return view;
end

--ж�ز��
--�ڴ�й©,����֮
--[[
function PluginMan:unload(p)
	p:unload();
	func_print("<<<<ж�ز��"..f_getInfo(p));	
	
	--setmetatable(getmetatable(p),nil);
	--setmetatable(p, nil);
	
	--print(getmetatable(p));
end
--]]

