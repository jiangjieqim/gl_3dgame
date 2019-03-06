--print("**************dasds ********");
--infowin_show(0,20);


function test()
	

	---[[
	local _floor = load_model(func_create_name(),"\\resource\\obj\\plane.obj")		-- func_loadobj('quad',nil,'myObj1',false)--quad
	setMaterial(_floor,func_load("//resource//material//floor.mat"));	
	--setv(_floor,FLAGS_VISIBLE);
	setv(_floor,FLAGS_RAY)
	--setv(_floor,FLAGS_DRAW_RAY_COLLISION)		--BUG 启动的时候会有空指针异常
	setv(_floor,FLAGS_DISABLE_CULL_FACE);

	local r = 0;
	


	cam:position(0,0,-5);

	func_setRotateX(_floor,PI)
	func_setRotateY(_floor,PI/2)
	func_setRotateZ(_floor,PI/2)
	--]]

	--scxzxAetv(_floor,FLAGS_DRAW_PLOYGON_LINE)--线框
    cam:position(0,0,-5);
end
function f_onkey(data)
	local key = tonumber(data);
	print(key);
	

--		r = r + PI/8;

--		if(key == KEY_A) then
--			func_setRotateX(_floor,r)
--		elseif(key == KEY_B)then
--			func_setRotateY(_floor,r)
--		elseif(key == KEY_C) then
--			func_setRotateZ(_floor,r)
--		elseif(key == KEY_D) then
--			local rx,ry,rz = func_get_rotate(_floor)
--			print(string.format("%.3f %.3f %.3f",rx%PI,ry%PI,rz%PI));
--			func_ex_info();
--		elseif(key == KEY_I) then
--			--func_changeFlags(_floor,FLAGS_DISABLE_CULL_FACE);
--        	ex:switch_spriteline();
--		end
	if(key == KEY_I) then
		ex:switch_spriteline();
    end
end

evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);

test();
--local btn = btn_create(10,20,65,65);
--[[
local bg = sprite_create("aa",10,20,65,65);
func_setIcon(bg,"gundi.png");
]]


--change_attr(nil,"ft_test");

-------------------------------------------------------------

local btn = btn_create(50,50,80,18,"gundi.png");
local ft = func_ftext_create(func_create_name(),18,16);
func_ftext_setpos(ft,50,50);
--local x,y;
--x = 0;
--y=0;
--local w,h = func_ftext_setchar(ft,"字");
--x=x+w;
--w,h = func_ftext_setchar(ft,",",x,y);
--x=x+w;
--w,h = func_ftext_setchar(ft,"R",x,y);


--print(ft,w,h);
func_ftext_set_much_chars(ft,"我,a,H,A");
