


dofile("..\\include\\lua\\core.lua");


local function init()

--########################################
node_fbo();

--btn_create(200,50,60,20,"smallbtn.png");
--btn_create(200,70,120,40,"smallbtn.png");




--[[
local function f_callback2(data,obj)
	print("******************************加载结束2",data,obj);	
	local spr = sprite_create_9grid(data,100,50,200,50,0,3,3,3,3);
	engine_addNode(spr);
end--]]



local function f_callback1(data,obj)
	
	
	print("******************************加载结束",data,obj);
	
	
	
	
	local spr1 = sprite_create_typical(100,100,100,50,0);
	sprite_set_9grid(spr1,"smallbtn.png",3,3,3,3);
	engine_addNode(spr1);
end


--print(string.format("res = %d",res));





--[[local btn = btn_create(0,50);
btn_bindClick(btn,f_onClick);--]]
--**********************************************************

function f_onkey(data)
	local key = tonumber(data);
	print("key = "..key);
	if(key == KEY_ESC) then
         engine_exit();
	elseif(key == 13) then
		func_ex_info();
	elseif(key == 49) then
		func_ex_info();
	
		--setv(btnspr,FLAGS_DRAW_PLOYGON_LINE );
		--setv(btnspr,FLAGS_DISABLE_CULL_FACE );
		if(btnspr) then
			if(getv(btnspr,FLAGS_REVERSE_FACE) == 1) then
				resetv(btnspr,FLAGS_REVERSE_FACE );
			else
				setv(btnspr,FLAGS_REVERSE_FACE );
			end
		end
	elseif(key == 50) then
		--[[if(btnspr) then
			setv(btnspr,FLAGS_DRAW_PLOYGON_LINE );
		end--]]
		if(sv) then
			scrollView_dispose(sv);
		end
	end
end
evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);

engine_setBg(0.3,0.3,0.3);

--fps();
infowin(0,100);

--创建一个输入组件
local function f_init_input()

	--作为输入框的背景
	local sprite =sprite_create("bg11",100,50,128,14);
	func_setIcon(sprite,"gundi.png");
	--setv(sprite,FLAGS_DRAW_PLOYGON_LINE);


	--local sprite1 =func_sprite_createNoHit(100,100,256,256);
	--func_setIcon(sprite1,"gundi.png");
	--setv(sprite1,FLAGS_DRAW_PLOYGON_LINE);

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
	evt_on(_in,EVENT_ENGINE_KEYBOARD,f_onkey);

end

--f_init_input();

--alert("aa");


--input_dispose(_in);
--func_fixed_load("\\resource\\obj\\torus.obj");
--dofile("..\\include\\lua\\quaternion.lua");--

--btn_create(0,0,128,128);

--dofile("..\\include\\lua\\7.lua");

--example_unit();

--//汉字第一个字节的范围
--static int
--f_is_first_code(unsigned char c){
--	return (0x80 <= c) && (0xF7 >= c);
--}
--//汉字第二个字节的范围
--static int
--f_is_second_code(unsigned char c){
--	return (0xA1 <= c) && (0xFE > c);
--}

--print(0x80,0xF7,0xA1,0xFE);


--local t = func_get_longTime();
--for i=1,1000000,1 do

--    math.random();
--end
--print(func_get_longTime()-t);


engine_refreshCam3d();

end


init();

--btn_create();
--btn_create(100);



--alert("aa");
--[[
local function f_loadend(p)
	
	print(p);
end
local function f_loadend2(p)
	
	print(p);
end

loadtexs("gundi.png;checkbox.png",f_loadend,1);
loadtexs("arrow3.png;smallbtn.png",f_loadend2,2);--]]