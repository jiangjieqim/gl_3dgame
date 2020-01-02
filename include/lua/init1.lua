dofile("..\\include\\lua\\core.lua");
dofile("..\\include\\lua\\editor.lua");

--func_enable_debug(false);--关闭调试日志
function f_onkey(data)
	local key = tonumber(data);
	if(key == KEY_ESC) then
        JEngine:getIns():exit();
	elseif(key == 13) then
		--回车
	elseif(key == 49) then
		JEngine:getIns():print_info();	
	end
end
evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);
JEngine:getIns():setbg(0.3,0.3,0.3);
JEngine:getIns():get_cam():set_pos(0,-4,-12);






local function f_tex_complete(p)
	print(p);
	local sc = example_srollView(100,100);	
end
loadtexs("checkbox.png;smallbtn.png",f_tex_complete);




--[[
infowin(150,0);
local e1 = Editor:new();
dofile("..\\include\\lua\\5.lua");
--]]

JEngine:getIns():get_cam():refresh();
--print(_VERSION) ;
