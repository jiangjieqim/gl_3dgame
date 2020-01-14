--------------------------------------------------------------------
-- 信息调试面板
local function f_select(list,index,p)
	local self = p;
	local _stat = self._stat;
	--local index =listbox_get_index(list);
	---[[
    if (index == 0) then
		print("windows test!");
    elseif (index == 1) then
        func_gc();
    elseif (index == 2) then
        self:loadfps();
    elseif (index == 3) then
        _stat = not _stat;
--        print(_stat);

        if(_stat) then
            JEngine:getIns():setbg(0.1,0.1,0.1);
        else
            JEngine:getIns():setbg(0.4,0.4,0.4);
        end
	elseif (index == 4) then
		--lua gc
		func_lua_gc();
    elseif(index == 5) then
        local cam3d = JEngine:getIns():get_cam();
        cam3d:reset();
    end
	--]]
end

local InfoWin = {
	list,
	_stat,
	fps,
};
InfoWin.__index= InfoWin;
setmetatable(InfoWin, IPlugin);--继承自插件接口
function InfoWin:load()
	local self = {};
	setmetatable(self, InfoWin);
	self:init();
	return self;
end
--切换显示fps
function InfoWin:loadfps()
	if(self.fps==nil) then
		self.fps = JEngine:getIns():get_plugin():load("view/FpsView");--加载插件
		self.fps:show();
	else
		JEngine:getIns():get_plugin():unload(self.fps);
		self.fps = nil;
	end
end

function InfoWin:set_pos(x,y)
	self.list:set_pos(x,y);
end

function InfoWin:init()
	
	--[[local list  = listbox_new(x or 0, y or 0);
	listbox_bind(list,f_select);
	
	
	listbox_add(list,"引擎信息");
	listbox_add(list,"gc");
	listbox_add(list,"fps");
	listbox_add(list,"背景颜色");
	listbox_add(list,"创建mesh");
	listbox_add(list,"重置cam");
	listbox_set_title(list,"infowin");--]]
	
	local list = NListBox:new(0,0,128);
	list:addItem("引擎信息");
	list:addItem("gc");
	list:addItem("fps");
	list:addItem("背景颜色");
	list:addItem("LuaGC");
	list:addItem("重置cam");
	list:bind(f_select,self);

	
	--listbox_del(list);
	--return list;
	
	
	--list:dispose();
	
	self.list = list;
end

function InfoWin:getName()
	return "InfoWin";
end

function InfoWin:unload()
	self.list:dispose();
	
	if(self.fps) then
		JEngine:getIns():get_plugin():unload(self.fps);
	end
	
	func_clearTableItem(self);
end

return InfoWin;