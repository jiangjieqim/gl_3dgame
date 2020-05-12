package.cpath = package.cpath .. ";c:/Users/Administrator.B124KVZ6GK0IWF6/.vscode/extensions/tangzx.emmylua-0.3.49/debugger/emmy/windows/x86/?.dll"
dbg = require("emmy_core")
dbg.tcpListen("localhost", 9966)

print(string.format("version = [%s]",_VERSION));

package.path=";c:\\gl_3dgame\\lua\\?.lua";--核心组件

--增加业务模块
package.path=package.path..";c:\\gl_3dgame\\lua\\src\\?.lua";
print('package.path = '..package.path);

local core = require("core");
local LineBox = require("linebox");
local e = core.e;
local plug = core.p;
core.debug(1);

-- local color = {a = "aa",b = "nn"}
-- print("######################",table.getn(color),core.getLen(color));







JEngine:getIns():setbg(0.4,0.4,0.4);
require("editor");

local function f_fps_timer(data,param)

end

local function loadAvatarView()
	local obj = {
		url="\\resource\\md2\\triangle.md2",--bauul
		mat = "//resource//material//bauul.mat",
		scale = 0.1, --0.003,
		x = 0,
		y = 0,
		z = -0.5,
	};
	plug:toggle("view/AvatarView",0,obj);
end

function f_onkey(data)
	local key = tonumber(data);
	if(key == KEY_ESC) then
        JEngine:getIns():exit();
	elseif(key == 13) then
		--回车
		-- print("delayMs = "..eg:get_delayTime());
		e:print_info();
		
	elseif(key == 49) then
		--1
		loadAvatarView();
		-- loadAvatarView();
	elseif(key == 50) then
		--2
		-- func_lua_gc("2");
		-- e:setfps(24);
		local label = plug:load("view/FlyLabel");
		local sx,sy = core.e:screen_size();

		label:set_label(math.random(),1000,sy*0.25);

	elseif(key == 51) then
		--3
		plug:toggle("view/SettingView");

	elseif(key == 52) then
		--4
		-- func_lua_gc("4_1");

		

--[[
		local tf = JEngine:getIns():get_plugin():load("view/tf");
		tf:init();
		tf:show("message");
        tf:unload();
		func_lua_gc("4_2");
--]]

	end
end

local function init()
	--print(core.now());
	--print(cam);
	JEngine:getIns():setbg(0.3,0.3,0.3);
	--JEngine:getIns():get_cam():set_pos(0,-4,-15);
	-- 0.000 -1.000 -5.500

	local e1 = Editor:new();
	JEngine:getIns():get_cam():set_pos(0,-1,-5.5);	--#	0, -1 -5.5



	-- evt_on(nil,EVENT_ENGINE_KEYBOARD,f_onkey);

end
-- init();
----[[
	local function f_bindRayClick(p)
		local x,y,z = JEngine:getIns():get_hit();--获取射线的交点坐标

		print('##### you call function: f_bindRayClick:',x,y,z);
		
		-- line:mod(1,x,y,z);
		
		-- --self.u:move(x,y,z);
		-- self.u:look_at(x,y,z,0);
	end
local function tc()
	local data = {
		url="\\resource\\md2\\triangle.md2",--	triangle	bauul
		mat = "//resource//material//bauul.mat",
	}	
	local cam = nil;

	-- local fbo = FboRender:new(128,128);
	-- cam = fbo:get_cam3d();
	local url = data.url;
	local mat = data.mat;
	local m = UnitBase:new();
	m:loadvbo(url,mat,cam);
	-- m:scale(0.003);
	m:scale(0.1);
	m:set_position(0,0,-1.5);
	m:load_collide("\\resource\\obj\\box.obj",true);
	m:bindRayPick(f_bindRayClick);


	--e:get_cam():set_pos(0,0,-0.5);

end
local function texCallBack()
	

	-- print("加载预制贴图完成...");

	--init();
--[[
	local btn = Button:new();
	btn:set_pos(50,20);
	btn:bind_click(function () 
			print(999);
		end
	);
]]
	
	-- loadAvatarView();
	
	

	


	-- local lb = LineBox:new(0.5);
	-- lb:setPos(0,0.2,0);
	-- lb:setRadius(1);


	-- local fps = plug:load("view/FpsView");
	-- fps:show();
end
e:setfps(24);--24
-- loadtexs("gundi.png;checkbox.png;smallbtn.png",texCallBack);
evt_on(0,EVENT_ENGINE_KEYBOARD,f_onkey);

-- local n = UnitBase:new();
-- n:loadvbo("\\resource\\md2\\bauul.md2","//resource//material//bauul.mat",0);
-- -- n:set_position(0,0,-100);
-- n:scale(0.01);

-- tc();




--  local shape = Shape:new(true,100,100);
--  shape:visible(true);



-- local function fc()
-- 	 print("fc..."..func_get_longTime());
-- 	shape:visible(false);
-- end
-- core.setTimeout(1000,fc);

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

