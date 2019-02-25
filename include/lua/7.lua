dofile("..\\include\\lua\\core.lua");

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
	setMaterial(_floor,func_load("//resource//material//floor.mat"));	
	setv(_floor,FLAGS_VISIBLE);
	--local _floorRadius = 30--地板半径
	--func_set_scale(_floor,_floorRadius*2);
	--func_set_y(_floor,-_floorRadius);

	func_set_scale(_floor,scale);
	
	setv(_floor,FLAGS_RAY)
	setv(_floor,FLAGS_DRAW_RAY_COLLISION)
	setv(_floor,FLAGS_DRAW_PLOYGON_LINE)--线框
	setv(_floor,FLAGS_DISABLE_CULL_FACE)

	--scale = 1.0;
	
	func_set_glsl_parms(_floor,'uvScale',scale)--设置diffuse.vs (uniform float _uvScale)uv重复值
	return _floor;
end

local function f_create_clickBox()
	local url = "\\resource\\obj\\box.obj";
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
local function f_btnClick()
	cam:rotate(5000);
	--print(tostring(unit))
end
local function f_btnClick1()
--	cam:rx(0);
end
local btn = btn_create(0,40);
btn_bindClick(btn,f_btnClick);

--local btn1 = btn_create(0,60);
--btn_bindClick(btn1,f_btnClick1);

--]]
--***********************************************************************************************



--创建一个角色
unit =  Unit:create("box")--unit_create();
unit:set_speed(600);


_selectBox = f_create_clickBox();

local function f_on_click_floor_handle(data)
	--print('##',data);
	local xml = xml_load_str(data);
	local node = xml_get_node_by_index(xml,0);
	local x = xml_get_float(node,"x")
	local y = xml_get_float(node,"y")
	local z = xml_get_float(node,"z")
	xml_del(xml);
	
	func_set_position(_selectBox,x,y,z);
	
	unit:move(x,y,z);
end


local function f_resize()
	cam:rx(PI * 1.8);
end

---[[--创建一个地板
local floor_ptr = f_create_floor(50);
evt_on(floor_ptr,EVENT_RAY_PICK,f_on_click_floor_handle);


evt_on(cam,EVENT_ENGINE_RESIZE,f_resize);


cam:position(0,-10,-10);
cam:rx(PI * 1.8);

print(unit);

--local a1 = Unit:create("box");
--a1:set_speed(10);
--print(a1);
----------------------------------


