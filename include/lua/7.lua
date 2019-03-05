dofile("..\\include\\lua\\core.lua");

--func_set_sprite_line(1);




--FLAGS_DRAW_RAY_COLLISION
--[[
function f_load_md2()
	md2=load_VBO_model(func_create_name(),"\\resource\\md2\\horse.md2");
	setMaterial(md2,func_load("//resource//material//horse.mat"));
	setv(md2,FLAGS_VISIBLE);
end
local btn = btn_create(0,40)
btn_label(btn,"load md2")
btn_bindClick(btn,f_load_md2)--切换显示是否要线框渲染
--]]

--创建一个指定半径的地板
local function f_create_floor(scale)
	--创建一个可点击的地板
	--local _floor = func_loadobj('plane','box.tga','_floor',false);
	local _floor = load_model(func_create_name(),"\\resource\\obj\\plane.obj")		-- func_loadobj('quad',nil,'myObj1',false)--quad
	local mat = func_load("//resource//material//triangle.mat");
    setMaterial(_floor,mat);
    glsl_set(mat,string.format("_lineColor,%s","0.5,0.5,0.5"));
	
	setv(_floor,FLAGS_VISIBLE);
	--local _floorRadius = 30--地板半径
	--func_set_scale(_floor,_floorRadius*2);
	--func_set_y(_floor,-_floorRadius);

	func_set_scale(_floor,scale);
	
	setv(_floor,FLAGS_RAY)
	--setv(_floor,FLAGS_DRAW_RAY_COLLISION)
	setv(_floor,FLAGS_DRAW_PLOYGON_LINE)--线框
	setv(_floor,FLAGS_DISABLE_CULL_FACE)

	--scale = 1.0;
	
	func_set_glsl_parms(_floor,'uvScale',scale)--设置diffuse.vs (uniform float _uvScale)uv重复值
	return _floor;
end

local function f_create_clickBox()
	local url = "\\resource\\obj\\box.obj";
	--	func_create_name()
	local obj = load_VBO_model(func_create_name(),url);
	setMaterial(obj,func_load("//resource//material//triangle.mat"));		
	setv(obj,FLAGS_DRAW_PLOYGON_LINE)--线框
	--setv(obj,FLAGS_DISABLE_CULL_FACE);
	setv(obj,FLAGS_REVERSE_FACE);
	setv(obj,FLAGS_VISIBLE);
	return obj;
end

--******************************************************
local unit;
local _selectBox;
fps();
infowin_show(0,20);

---[[
local function f_btnClick(b)
	--cam:rotate(5000);
	--print("==========\t"..b)
	--print(b);
	--alert("AAbb");
    ex:set_spriteline(ex:get_spriteline() == 1 and 0 or 1);
end

local function f_btnDown(b)
	--cam:rotate(5000);
	print(b)
end
local btn = btn_create(100,20);
btn_bindClick(btn,f_btnClick);
--btn_bindDown(btn,f_btnDown);

---[[
local function f_animscHandle(sc)
    local v = sc.value * PI * 2;
	print("### "..tostring(v));
    func_setRotateY(unit:get_ptr(),v);
end

local animsc = scrollBar_new(200,20)

scrollBar_setRange(animsc,0,1)
animscTf = scrollBar_add_text(animsc,'animsc')
scrollBar_bind(animsc,f_animscHandle)
--]]

--local btn1 = btn_create(0,60);
--btn_bindClick(btn1,f_btnClick1);

--]]
--***********************************************************************************************
--创建一个角色
unit =  Unit:create("DEBUG")--unit_create();
unit:set_speed(600);
_selectBox = f_create_clickBox();
--alert("AAbb");

local function f_on_click_floor_handle(data)
	local xml = xml_load_str(data);
	local node = xml_get_node_by_index(xml,0);
	local x = xml_get_float(node,"x")
	local y = xml_get_float(node,"y")
	local z = xml_get_float(node,"z")
	xml_del(xml);
	
	func_set_position(_selectBox,x,y,z);
	
	unit:move(x,y,z);
    --unit:rotateTo(x,y,z,100);
end


local function f_resize()
	cam:rx(PI * 1.8);
end

---[[--创建一个地板
local floor_ptr = f_create_floor(50);

evt_on(floor_ptr,EVENT_RAY_PICK,f_on_click_floor_handle);

evt_on(cam,EVENT_ENGINE_RESIZE,f_resize);

function f_onkey(data)
	local key = tonumber(data);
--	print(key);
	if(key == KEY_A) then
		
		local px,py,pz = func_get_xyz(unit:get_ptr());
		print(string.format("unit %.3f %.3f %.3f",px,py,pz));
	    alert(tostring(math.random(0,100)));	
	elseif(key == KEY_B)then
	
		func_changeFlags(floor_ptr,FLAGS_DRAW_PLOYGON_LINE);
	
	elseif(key == KEY_C) then
		print(key);
		--func_changeFlags(unit:get_ptr(),FLAGS_DISABLE_CULL_FACE);
		--func_changeFlags(unit:get_ptr(),FLAGS_VISIBLE);
		--func_update_mat4x4(unit:get_ptr());
	end
end
evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);
cam:position(0,-10,-10);
cam:rx(PI * 1.8);