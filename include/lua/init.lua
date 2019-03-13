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
infowin(0,20);

--local list = ListBox:new();list:add("a");
--local list1;
--local function f_select(index)
--    print(index);
--    if(index == 1) then
--        list1:dispose();
--    end
--end
--list1 = ListBox:new(100,0,f_select);
--list1:add("a1");
--list1:add("a2");

--dofile("..\\include\\lua\\8.lua");

--print("lua script is already init finished !");

--local function f_onkey(data)
--	local key = tonumber(data);
--    if(key == KEY_ESC) then
--        ex:exit();	
--	end
--end

--evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);

--local btn = btn_create(0,0,80,18,"gundi.png");
--local ft = func_ftext_create(16,15);

--func_ftext_parse(ft,"aƒ„B");