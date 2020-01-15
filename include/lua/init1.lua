dofile("..\\include\\lua\\core.lua");
dofile("..\\include\\lua\\editor.lua");


local function f_fps_timer(data,param)


end



--func_enable_debug(false);--关闭调试日志
function f_onkey(data)
	local key = tonumber(data);
	if(key == KEY_ESC) then
        JEngine:getIns():exit();
	elseif(key == 13) then
		--回车
	elseif(key == 49) then
		--1
		JEngine:getIns():print_info();	
	elseif(key == 50) then
		--2
		func_lua_gc("2");
		
	elseif(key == 51) then
		--3
		func_lua_gc("3_1");

----[[
		local f = JEngine:getIns():get_plugin():load("view/FpsView");--加载插件
		f:show();
		f:unload();
		--f = nil;
		--]]
		
--[[
		evt_on(2,EVENT_TIMER,f_fps_timer,"a");
		evt_off(2,EVENT_TIMER,f_fps_timer);
--]]
	
		
	


		func_lua_gc("3_2");

		
		
		
		
		
	elseif(key == 52) then
		--4

		
		func_lua_gc("4_1");

		
--[[
		local tf = JEngine:getIns():get_plugin():load("view/tf");
		tf:init();
		tf:show("message");
        tf:unload();
--]]

		
		
		func_lua_gc("4_2");

	end
end
evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);
JEngine:getIns():setbg(0.3,0.3,0.3);
JEngine:getIns():get_cam():set_pos(0,-4,-15);


--*******************************************

--func_lua_gc();

--###############################################################


--[[
local win = JEngine:getIns():get_plugin():load("view/InfoView");
win:set_pos(150,0);
win:unload():
win = nil;
func_lua_gc();
--]]





--]]
--###############################################################



--local e1 = Editor:new();

--dofile("..\\include\\lua\\5.lua");
--dofile("..\\include\\lua\\quaternion.lua");

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






JEngine:getIns():get_cam():refresh();
--print(_VERSION) ;
