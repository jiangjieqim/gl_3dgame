dofile("..\\include\\lua\\core.lua");
dofile("..\\include\\lua\\modelControl.lua");
dofile("..\\include\\lua\\editor.lua");

--local e = JEngine:getIns();

--print(JEngine:getIns():get_atals());

--e:dispose();

--func_enable_debug(false);--关闭调试日志

--	example_stack();

--########################################
--example_srollView(200,0);

--btn_create(200,50,60,20,"smallbtn.png");
--btn_create(200,70,120,40,"smallbtn.png");

--[[
local function f_callback2(data,obj)
	print("******************************加载结束2",data,obj);	
	local spr = sprite_create_9grid(data,100,50,200,50,0,3,3,3,3);
	JEngine:getIns():add(spr);
end--]]

--print(string.format("res = %d",res));

--[[local btn = btn_create(0,50);
btn_bindClick(btn,f_onClick);--]]
--**********************************************************

function f_onkey(data)
	local key = tonumber(data);
	--print("key = "..key);
	if(key == KEY_ESC) then
        JEngine:getIns():exit();
	elseif(key == 13) then
	--回车
	--	func_ex_info();
	elseif(key == 49) then
		func_ex_info();
	
		--setv(btnspr,FLAGS_DRAW_PLOYGON_LINE );
		--setv(btnspr,FLAGS_DISABLE_CULL_FACE );
		
		
		--local dt = JEngine:getIns():get_delayTime();
		--print(dt);
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

JEngine:getIns():setbg(0.3,0.3,0.3);
--********************************************
local function uiinit()
	fps();
	btn_create(150,0,100,45);
	example_input(0,80);
	infowin(150,50);
	example_srollView();
	--dofile("..\\include\\lua\\1.lua");--md2动作控制
	dofile("..\\include\\lua\\5.lua");--材质测试
end
--********************************************


--input_dispose(_in);
--func_fixed_load("\\resource\\obj\\torus.obj");
--dofile("..\\include\\lua\\quaternion.lua");--

--arrow();

infowin(150,0);


--		uiinit();

--dofile("..\\include\\lua\\3.lua");


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
	
	----[[
	local obj2 = Unit:new();
	obj2:load("\\resource\\obj\\tri.obj",--triangle,bauul,box,o1
	"//resource//material//bauul.mat","obj2");
	--unit_scale(obj2,20);
	
		--setv(obj2.m,FLAGS_REVERSE_FACE);
		setv(obj2.m,FLAGS_DISABLE_CULL_FACE);--双面渲染
		--setv(obj2.m,FLAGS_DRAW_PLOYGON_LINE);
		func_bind_collide(obj2.m);
		--setv(obj2.m,FLAGS_DRAW_RAY_COLLISION);
		setv(obj2.m,FLAGS_RAY);

		
		--]]
	--obj2:move(0,2,0,2000);
		
	--[[
	local obj2 = unit_load("\\resource\\md2\\bauul.md2",--triangle,bauul
		"//resource//material//bauul.mat");
	unit_scale(obj2,0.02);
	unit_rx(obj2,PI/2);
	unit_split_anim(obj2);
	--]]
	return obj2;
end



local function loadmd5(name)
	local model
	--x = x or 0
	--scale = scale or 0.015
	
	--[[
	if(name == 'wolf') then
		model = func_loadmd5('wolf',scale,"\\resource\\texture\\wolf.tga")		
		func_set_rayRadius(model,30)--设置静态碰撞检测盒
	else
		model  = func_loadmd5('cube',1.0,"\\resource\\texture\\wolf.tga","pCube.md5mesh","pCube_a7.md5anim")
	end
	--]]
	model=load_model(name or "md5test","\\resource\\md5\\wolf\\body.md5mesh");	
	setMaterial(model,func_load("//resource//material//wolf.mat"));
	md5_loadAnim(model, "\\resource\\md5\\wolf\\walk.md5anim","walk");

	setv(model,FLAGS_VISIBLE);
	--func_set_x(model,x)
	func_set_scale(model,0.02)--设置模型的缩放

	--f_init(model)
	JEngine:getIns():bind_3dcam(model);
	
	func_changeFlags(model,FLAGS_ANIM_ADAPTIVE)			--设置固定的fps模式,另一种自适应的有BUG
	
	
	func_changeFlags(model,FLAGS_DRAW_RAY_COLLISION);
	func_changeFlags(model,FLAGS_RAY);

	func_set_ptr_fps(model,260);--设置fps
	
	--change_attr(model, "setframe", 10);--设置当前的关键帧
	func_set_x(model,2);
	

	func_setRotateX(model,PI/2);


	return model
end
local function loadObj(model)
	
	local name = func_create_name();
	model = model or 'torus'--'box'--'torus'
	--local obj = func_loadobj(model)--box	'torus'
	local obj = load_model(name,string.format("\\resource\\obj\\%s.obj",model));	
	setMaterial(obj,func_load("//resource//material//wolf.mat"));
	setv(obj,FLAGS_VISIBLE);
	---[[
	setv(obj,FLAGS_RAY)					--设置为可拾取状态
	--setv(obj,FLAGS_DRAW_RAY_COLLISION)	--绘制射线盒子
	setv(obj,FLAGS_DISABLE_CULL_FACE)		--显示双面显示
	--setv(obj,FLAGS_GLSL_OUTLINE)
	--setv(obj,FLAGS_DRAW_PLOYGON_LINE)
	
	setv(obj,FLAGS_RENDER_BOUND_BOX);
	setv(obj,FLAGS_DRAW_NORMAL);

	
	--moduleUI_bind(obj)
	--f_add_model(obj)

	--func_set_pick(obj,"f_pick")
	--]]
	JEngine:getIns():bind_3dcam(obj);
	
	func_set_x(obj,-3);

	return obj
end

--loadmd2();
--loadmd5();
--loadObj();







--[[

local obj = Unit:new();
obj:load("\\resource\\obj\\arrow.obj",--triangle,bauul
	"//resource//material//triangle.mat");
setv(obj.m,FLAGS_DRAW_PLOYGON_LINE);
--]]
--obj:set_pos(1,1,1)


local cam3d = JEngine:getIns():get_cam();
cam3d:set_pos(0,-4,-12);


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

--local mc = ModelControl:new(300,30);

--local u = loadbox();

--mc:bind(u);




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


--[[

local function f_cpmlete(self)
	print("加载结束");
	
	
	
	self:set_pos(10,20);--设置坐标

	local skin1 = self:find("skin1");
	local label = skin1:find("2");
	--print(skin1,label);
	--label_set_text(label, "x*");

	--self:dispose();--销毁
end

local nskin = NSkin:new();
evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmlete);
nskin:load("\\resource\\crl.xml","gundi.png;checkbox.png;smallbtn.png");
--]]



--*************************************************************
local model = u;
local function f_rx_handle(progress,p)
	if(model) then
		model:set_rx(progress*PI);
	end
end


local function f_cpmlete(skin)
	
	skin:set_pos(0,15);
	
	local sc = skin_find(skin,"sc");
	sc:bindCallback(f_rx_handle);
	
	local tbtn = skin_find(skin,"tbtn");
	tbtn:bind_click(function ()
			if(model) then
				model:set_pos(0,0,0);
				model:move(0,2,0,2000);
			end
		end
	);
	
	local tbtn1 = skin_find(skin,"tbtn1");

	tbtn1:bind_click(function ()
			--[[if(model) then
				model:dispose();
			end--]]
			
			
		end
	);
	
	
	
	--local list = f_createLines(10,1);
	--print(stack_length(list));
	
	local e1 = Editor:new();
	--e1:dispose();

end
local nskin = NSkin:new();
evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmlete);
nskin:load("\\resource\\rx.xml","gundi.png;checkbox.png;smallbtn.png");
--#######################################################
local function f_fps_timer(data)
	--print(data);
	if(model) then
		model:set_ry(data*PI/1000);
	end
end
local timer = timelater_new(10);
evt_on(timer,EVENT_TIMER,f_fps_timer);
--增加一个sprite到fbo渲染列表中
local function addbtn_fbo(fbo)
	local x = 0;
	local y = 0;
	local w = 50;
	local h = 20;
	local url = "smallbtn.png";

	local sprite = sprite_create_typical(nil,x,y,w,h,fbo:get_cam2d());--string.format("sv%d",cnt)
	--set_cam(sprite,fbo:get_cam2d());

	JEngine:getIns():add(sprite);
	loadtexs(url,func_texloadend, { sprite=sprite;url=url});
	--sprite_set_hit_rect(sprite,x,y,w,h);
	--set_cam(sprite,cam2d);

	--print(fbo:get_cam2d());
end

--增加一个md2渲染对象到fbo对象引用中
local function addmd2_fbo(fbo)

	local n = JEngine:getIns():load("\\resource\\md2\\bauul.md2");--"\\resource\\md2\\bauul.md2"
	local material = func_load("//resource//material//bauul.mat");
	setMaterial(n,material);
	setv(n,FLAGS_VISIBLE);
	JEngine:getIns():add(n);
	--setv(n,FLAGS_DRAW_PLOYGON_LINE);
	func_set_position(n,0,0,-100);
	
	func_setRotateX(n,PI/2)
	func_setRotateY(n,PI/2)
	set_cam(n,fbo:get_cam3d());
	--set_cam(n,cam3d);
end
--创建一个fbo对象
--local fbo = FboRender:new(128,128);
--fbo:set_pos(0,200);

--addmd2_fbo(fbo);
--addbtn_fbo(fbo);




JEngine:getIns():get_cam():refresh();
--print(_VERSION) ;
