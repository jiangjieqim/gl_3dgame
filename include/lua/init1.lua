dofile("..\\include\\lua\\core.lua");

--########################################
--node_fbo();

--btn_create(200,50,60,20,"smallbtn.png");
--btn_create(200,70,120,40,"smallbtn.png");

--[[
local function f_callback2(data,obj)
	print("******************************加载结束2",data,obj);	
	local spr = sprite_create_9grid(data,100,50,200,50,0,3,3,3,3);
	engine_addNode(spr);
end--]]

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
	--回车
	--	func_ex_info();
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
--********************************************
local function uiinit()
	fps();
	btn_create(150,0,100,45);
	example_input(0,80);
	infowin(150,50);
	example_srollView();
	alert("aa");
	dofile("..\\include\\lua\\1.lua");--md2动作控制
	dofile("..\\include\\lua\\5.lua");--材质测试
end
--********************************************


--input_dispose(_in);
--func_fixed_load("\\resource\\obj\\torus.obj");
--dofile("..\\include\\lua\\quaternion.lua");--

--arrow();

--infowin(150,50);

crl_init();


--uiinit();
--dofile("..\\include\\lua\\3.lua");

--alert("aa");

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



--init();

--btn_create();
--btn_create(100);


--[[local function loadCallBack(a)
	print("***************************************",a);
	alert_dispose(a);
end

alert("aa",loadCallBack);--]]
--function evt_once(obj,id,func,params)
	








--[[
local function f_loadend(p)
	
	print(p);
end
local function f_loadend2(p)
	
	print(p);
end

loadtexs("gundi.png;checkbox.png",f_loadend,1);
loadtexs("arrow3.png;smallbtn.png",f_loadend2,2);--]]