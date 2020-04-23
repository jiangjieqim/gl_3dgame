print(string.format("version = [%s]",_VERSION));

package.path=";c:\\gl_3dgame\\lua\\?.lua";--核心组件

--增加业务模块
package.path=package.path..";c:\\gl_3dgame\\lua\\src\\?.lua";
print('package.path = '..package.path);

local core = require("core");
local e = JEngine:getIns();
local plug = e:get_plugin();

JEngine:getIns():setbg(0.4,0.4,0.4);
require("editor");

local function f_fps_timer(data,param)

end
local setting;
--func_enable_debug(false);--关闭调试日志
function f_onkey(data)
	local key = tonumber(data);
	if(key == KEY_ESC) then
        JEngine:getIns():exit();
	elseif(key == 13) then
		--回车
		print("delayMs = "..eg:get_delayTime());
	elseif(key == 49) then
		--1
		-- JEngine:getIns():print_info();	
		--e:setfps(1);
		if(setting==nil)then
			setting = plug:load("view/SettingView");
		else
			if(setting:is_visible()) then
				setting:hide();
			else
				setting:show(true);
			end

		end
	elseif(key == 50) then
		--2
		-- func_lua_gc("2");
		e:setfps(24);
	elseif(key == 51) then
		--3
		func_lua_gc("3_1");
----[[
		local f = JEngine:getIns():get_plugin():load("view/FpsView");--加载插件
		f:show();
		f:dispose();
--[[
		evt_on(2,EVENT_TIMER,f_fps_timer,"a");
		evt_off(2,EVENT_TIMER,f_fps_timer);
--]]
		func_lua_gc("3_2");
	elseif(key == 52) then
		--4
		func_lua_gc("4_1");
----[[
		local tf = JEngine:getIns():get_plugin():load("view/tf");
		tf:init();
		tf:show("message");
        tf:unload();
--]]
		func_lua_gc("4_2");
	end
end

local function init()
	--print(core.now());
	--print(cam);	
	JEngine:getIns():setbg(0.3,0.3,0.3);
	--JEngine:getIns():get_cam():set_pos(0,-4,-15);
	JEngine:getIns():get_cam():set_pos(0,-1,-4);


	local e1 = Editor:new();
	JEngine:getIns():get_cam():refresh();

	-- evt_on(nil,EVENT_ENGINE_KEYBOARD,f_onkey);

end
-- init();
----[[


local function texCallBack()
	-- print("加载预制贴图完成...");

	local btn = Button:new();
	btn:set_pos(100,100);
	btn:bind_click(function()
		init();
		-- core.gc();
		
		
	end
	);
	e:setfps(30);--24
	--15 nice
	--	>15 will be bad 
	evt_on(btn,EVENT_ENGINE_KEYBOARD,f_onkey);


	local fps = plug:load("view/FpsView");
	fps:show();
end


loadtexs("gundi.png;checkbox.png;smallbtn.png",texCallBack);













--]]

-- local win = JEngine:getIns():get_plugin():load("view/InfoView");
--win:set_pos(150,0);
--core.setTimeout(5000,init);
--*******************************************
--func_lua_gc();

--###############################################################

----[[
-- func_lua_gc();
-- local win = plug:load("view/InfoView");
-- win:set_pos(0,0);

-- win:dispose();
-- win = nil;
--func_lua_gc();
--]]

--]]
--###############################################################
--require("5.lua");
--require("quaternion");
--]]

--[[
local s= LineNode:new(3);
s:push(-0.5,0.4,0.5);
s:push( 0.5, 0.0, 0.5);
s:push( -0.5, -0.49, -0.5);
s:setcolor(1,0,0);
s:pushend();

s:mod(0,1,0,0);
s:mod(2,0,0,0);
--s:dispose();--销毁
--]]





-- local a = {key = 0};
-- print(#a);
