dofile("..\\include\\lua\\core.lua");
function f_onkey(data)
	local key = tonumber(data);
	if(key == KEY_ESC) then
        ex:exit();	
	end
end
evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);
fps();
infowin(0,20);
--dofile("..\\include\\lua\\7.lua");

cam:position(0,-5,-5);
cam:rx(PI * 1.8);
----------------------------------------------------------------------------
local obj = UnitBase:new();
obj:loadvbo("\\resource\\md2\\bauul.md2","//resource//material//bauul.mat");
obj:scale(1/50);
obj:rx(PI/2);
obj:y(0.5);

local obj2 = UnitBase:new();
obj2:loadvbo("\\resource\\md2\\gobin.md2","//resource//material//gobin.mat");
obj2:scale(1/50);
obj2:rx(PI/2);
obj2:y(0.5);

local select = UnitBase:new();
select:loadbox();

--µØ°å
local plane = UnitBase:new();
plane:load_model();
plane:setv(FLAGS_REVERSE_FACE);
plane:setv(FLAGS_DRAW_PLOYGON_LINE);
plane:scale(10);

local function f_on_click_floor_handle(data)
    local pos = func_xml_to_tb(data);
    obj2:move(pos.x,pos.y,pos.z);
end
local function f_endCall(msg)
    
    if(msg == UnitBaseEndMsg) then
        print(obj2:get_name().."ÒÆ¶¯½áÊø");
    end
end

plane:bindRayPick(f_on_click_floor_handle);

evt_on(obj2:get_p(),UnitBaseEvent,f_endCall);