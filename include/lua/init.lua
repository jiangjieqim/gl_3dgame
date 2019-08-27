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


--fps(0,15);
--infowin(0,0);

local sprite =sprite_create("bg11",100,50,128,14);
func_setIcon(sprite,"gundi.png");
setv(sprite,FLAGS_DRAW_PLOYGON_LINE);


alert("ÎÄ±¾");

local _in = input_create();
input_set_pos(_in,100,50);
local function f_onkey(data)
	local key = tonumber(data);
	--print('my key = '..key);
	if(key == 13) then
		--print(input_isFocusIn(_in));
		if(input_isFocusIn(_in)) then
			print("inout str = ["..input_get_str(_in).."]");
			input_clear(_in);
		end
	elseif(key == KEY_B)then
       
    elseif(key == KEY_ESC) then
        
	end
end
evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);

--input_dispose(_in);

--func_fixed_load("\\resource\\obj\\torus.obj");

--dofile("..\\include\\lua\\quaternion.lua");--


local function loadFloor()
	cam:position(0,-5,-5);
	cam:rx(PI * 1.8);
	--µØ°å
	local plane = UnitBase:new();
	--plane:load_model(nil,"//resource//material//floor.mat");
	plane:load_model();
	plane:setv(FLAGS_REVERSE_FACE);
	plane:setv(FLAGS_DRAW_PLOYGON_LINE);
plane:scale(10);
end

--loadFloor();
dofile("..\\include\\lua\\7.lua");



--local t = func_get_longTime();
--for i=1,1000000,1 do

--    math.random();
--end
--print(func_get_longTime()-t);
