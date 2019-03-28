dofile("..\\include\\lua\\core.lua");
function f_onkey(data)
	local key = tonumber(data);
	if(key == KEY_ESC) then
        ex:exit();	
	end
end

evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);
ex:bgColor(0.3,0.3,0.3);
--ex:bgColor(1,1,1);
--ex:bgColor(0.5,0.5,0.5);


--fps();
infowin(200,0);

dofile("..\\include\\lua\\quaternion.lua");
--dofile("..\\include\\lua\\7.lua");


--local t = func_get_longTime();
--for i=1,1000000,1 do

--    math.random();
--end
--print(func_get_longTime()-t);
