dofile("..\\include\\lua\\core.lua");
print("**************dasds ********");
infowin_show(0,20);


local _floor = load_model(func_create_name(),"\\resource\\obj\\plane.obj")		-- func_loadobj('quad',nil,'myObj1',false)--quad
setMaterial(_floor,func_load("//resource//material//floor.mat"));	
setv(_floor,FLAGS_VISIBLE);
setv(_floor,FLAGS_RAY)
setv(_floor,FLAGS_DRAW_RAY_COLLISION)

local r = 0;
function f_onkey(data)
	local key = tonumber(data);
--	print(key);
	print(key);
	if(key == KEY_A) then
		
		
	
	elseif(key == KEY_B)then
	
	elseif(key == KEY_C) then
		r = r + PI/4
		func_setRotateZ(_floor,r)
		
	end
	
	
end

evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);


cam:position(0,0,-2);
func_setRotateX(_floor,PI/2)
setv(_floor,FLAGS_DISABLE_CULL_FACE);

--setv(_floor,FLAGS_DRAW_PLOYGON_LINE)--Ïß¿ò