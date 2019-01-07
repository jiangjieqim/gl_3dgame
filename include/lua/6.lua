dofile("..\\include\\lua\\core.lua");
func_print('四元数旋转测试',0xff0000)

local name = 'torus0'
local name1 = 'torus1'


local animsc = scrollBar_new(0,0)
scrollBar_setRange(animsc,0,1)

scrollBar_bind(animsc,
	function(sc)
		test_unit_01(name,sc.value,name1)
		--print(sc.value)
	end
)

local _scale = 1

local obj1 = func_loadobj('quad','wolf.tga','center')
setv(obj1,FLAGS_DRAW_PLOYGON_LINE)
func_set_scale(obj1,_scale)
--[[
local obj = func_loadobj('quad','wolf.tga',name)
setv(obj,FLAGS_DRAW_PLOYGON_LINE)
func_set_scale(obj,_scale)

local obj1 = func_loadobj('quad','wolf.tga',name1)
setv(obj1,FLAGS_DRAW_PLOYGON_LINE)
func_set_scale(obj1,_scale)
--]]
func_set_camera_pos(0,0,-5)


