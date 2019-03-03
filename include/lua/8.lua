dofile("..\\include\\lua\\core.lua");
--print("**************dasds ********");
--infowin_show(0,20);


function test()
	local btn = btn_create(0,0);

	---[[
	local _floor = load_model(func_create_name(),"\\resource\\obj\\plane.obj")		-- func_loadobj('quad',nil,'myObj1',false)--quad
	setMaterial(_floor,func_load("//resource//material//floor.mat"));	
	setv(_floor,FLAGS_VISIBLE);
	--setv(_floor,FLAGS_RAY)
	--setv(_floor,FLAGS_DRAW_RAY_COLLISION)		--BUG 启动的时候会有空指针异常
	--setv(_floor,FLAGS_DISABLE_CULL_FACE);

	local r = 0;
	function f_onkey(data)
		local key = tonumber(data);
		
	--	print(key);

	--	print(key,r);

		r = r + PI/8;
		
		if(key == KEY_A) then
			func_setRotateX(_floor,r)
		elseif(key == KEY_B)then
			func_setRotateY(_floor,r)
		elseif(key == KEY_C) then
			func_setRotateZ(_floor,r)
		elseif(key == KEY_D) then
			local rx,ry,rz = func_get_rotate(_floor)
			print(string.format("%.3f %.3f %.3f",rx%PI,ry%PI,rz%PI));
			func_ex_info();
			
			--func_changeFlags(_floor,FLAGS_DISABLE_CULL_FACE);
		end
		
		
	end

	evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);


	cam:position(0,0,-5);

	func_setRotateX(_floor,PI)
	func_setRotateY(_floor,PI/2)
	func_setRotateZ(_floor,PI/2)
	--]]

	--scxzxAetv(_floor,FLAGS_DRAW_PLOYGON_LINE)--线框

end


	function f_onkey(data)
		local key = tonumber(data);
		
	--	print(key);

	--	print(key,r);

		if(key == KEY_A) then
			func_ex_info();
		elseif(key == KEY_B)then
			
		elseif(key == KEY_C) then
			
		elseif(key == KEY_D) then
			
		end
		
		
	end

	evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);
	
	
--test();
--local btn = btn_create(10,20,65,65);
--[[
local bg = sprite_create("aa",10,20,65,65);
func_setIcon(bg,"gundi.png");
]]


--change_attr(nil,"ft_test");
cam:position(0,0,-5);


--[[
local _floor = load_model(func_create_name(),"\\resource\\obj\\torus.obj")		-- func_loadobj('quad',nil,'myObj1',false)--quad
setMaterial(_floor,func_load("//resource//material//floor.mat"));
setv(_floor,FLAGS_VISIBLE);	

--]]


--unit =  Unit:create("box1")--unit_create();





