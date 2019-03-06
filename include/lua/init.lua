dofile("..\\include\\lua\\core.lua");

--fps();
--infowin(0,20);


--dofile("..\\include\\lua\\8.lua");


local function f_onkey(data)
	local key = tonumber(data);
    if(key == KEY_ESC) then
        ex:exit();	
	end
end

evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);


local ft = func_ftext_create(16,15);

func_ftext_parse(ft,"aƒ„B");