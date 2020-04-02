dofile("..\\include\\lua\\core.lua");
--node_fbo();
--example_srollView();

function f_onkey(data)
	local key = tonumber(data);
	--print("key = "..key);
	if(key == KEY_ESC) then
         engine_exit();
	elseif(key == 49) then
		func_ex_info();
	end
end

evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);

--------------------------------------------------------------

--engine_refreshCam3d();

local fbo = FboRender:new(128,128);
fbo:set_pos(128,128);
--[[
local n = node_new();
node_loadModel(n);
node_loadMaterial(n,"//resource//material//triangle.mat");--bauul.mat
setv(node_getNode(n),FLAGS_DRAW_PLOYGON_LINE);
--setv(node_getNode(n),FLAGS_REVERSE_FACE);--反转三角面

engine_addNode(n.node);
ex_set_cam(node_getNode(n));
--set_cam(node_getNode(n),fbo:get_cam3d());--]]

local cam2d,cam3d = JEngine:getIns():get_cam();

local function addbtn()

	local x = 10;
	local y = 10;
	local w = 90;
	local h = 20;
	local url = "smallbtn.png";

	local sprite = sprite_create_typical(nil,x,y,w,h,fbo:get_cam2d());--string.format("sv%d",cnt)
	--set_cam(sprite,fbo:get_cam2d());

	engine_addNode(sprite);
	loadtexs(url,func_texloadend, { sprite=sprite;url=url});
	--sprite_set_hit_rect(sprite,x,y,w,h);
	--set_cam(sprite,cam2d);

	--print(fbo:get_cam2d());
end

local function addmodel()
	local n =func_ex_loadVBO(func_create_name(),"\\resource\\obj\\arrow.obj");
	--"\\resource\\md2\\bauul.md2");


	local material = func_load("//resource//material//triangle.mat");
	setMaterial(n,material);
	setv(n,FLAGS_VISIBLE);
	engine_addNode(n);
	setv(n,FLAGS_DRAW_PLOYGON_LINE);
	func_set_position(n,0,0,-5.5);
	
	
	set_cam(n,fbo:get_cam3d());
	--set_cam(n,cam3d);
end

local function addmd2()
	local n =func_ex_loadVBO(func_create_name(),"\\resource\\md2\\bauul.md2");
	local material = func_load("//resource//material//bauul.mat");
	setMaterial(n,material);
	setv(n,FLAGS_VISIBLE);
	engine_addNode(n);
	--setv(n,FLAGS_DRAW_PLOYGON_LINE);
	func_set_position(n,0,0,-50.5);
	
	
	set_cam(n,fbo:get_cam3d());
	--set_cam(n,cam3d);
end




addbtn();
addmd2();
--addmodel();