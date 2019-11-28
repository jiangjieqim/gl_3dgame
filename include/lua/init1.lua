dofile("..\\include\\lua\\core.lua");

dofile("..\\include\\lua\\modelControl.lua");

func_enable_debug(false);--关闭调试日志

--example_stack();

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
	--print("key = "..key);
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
	--alert("aa");
	--dofile("..\\include\\lua\\1.lua");--md2动作控制
	dofile("..\\include\\lua\\5.lua");--材质测试
end
--********************************************


--input_dispose(_in);
--func_fixed_load("\\resource\\obj\\torus.obj");
--dofile("..\\include\\lua\\quaternion.lua");--

--arrow();

infowin(150,50);

--crl_init();

--		uiinit();

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

--***************************************************



--[[



<ui name="1" type="Panel" drag="1" center="1"/>
<ui name="2" type="Label" x="10" y="20" parent="1"/>
<ui name="3" type="Button" x="0" y="50" parent="1"/>


--]]

local function loadbox()
	
	
	local obj2 = Unit:new();
	obj2:load("\\resource\\obj\\quad.obj",--triangle,bauul
	"//resource//material//particle.mat");
--unit_scale(obj2,20);
	
		--setv(obj2.p,FLAGS_REVERSE_FACE);
		setv(obj2.m,FLAGS_DRAW_PLOYGON_LINE);
		
		
		--[[local obj2 = unit_load("\\resource\\md2\\bauul.md2",--triangle,bauul
		"//resource//material//bauul.mat");
	unit_scale(obj2,0.02);
	unit_rx(obj2,PI/2);
	unit_split_anim(obj2);--]]
	
	return obj2;
end
--[[

local obj = Unit:new();
obj:load("\\resource\\obj\\arrow.obj",--triangle,bauul
	"//resource//material//triangle.mat");
setv(obj.m,FLAGS_DRAW_PLOYGON_LINE);
--]]
--obj:set_pos(1,1,1)



cam_setPosition(0,0,-10);



--添加fps显示视图
--local f = FpsView:new();
--f:show();


--print(f.p);

--[[
local p = NPanel:new();
p:enable_center(true);
p:setDrag(true);
p:center();
--p:dispose();
--]]


--[[
local p1 = NPanel:new();
p1:enable_center(true);
p1:setDrag(true);
p1:center();--]]

local mc = ModelControl:new(30,30);local u = loadbox();mc:bind(u);




--[[local function f_tex_complete()
	
	local pb = ProgressBar:new(100);
	
	pb:progress(0.1);
	
	--func_print("##############################");
	
	--pb:dispose();
end
loadtexs("checkbox.png;gundi.png",f_tex_complete);--]]


--[[
local x = 2;

if(	   x == 1 
	or x == 2 
	or x == 3 
	or x == 4) then
	print("x = "..x);
end

--]]


--local str = "abc";
--local s = string.sub(str,0,#str - 1);
----


--example_stack();


engine_refreshCam3d();

--print(_VERSION) ;