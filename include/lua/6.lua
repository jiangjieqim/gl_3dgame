dofile("..\\include\\lua\\core.lua");
func_print('四元数旋转测试',0xff0000)

local name = 'torus0'
local name1 = 'torus1'


local animsc = scrollBar_new(0,0)
scrollBar_setRange(animsc,0,1)
local tf = scrollBar_add_text(animsc,'')

scrollBar_bind(animsc,
	function(sc)
		test_unit_01(sc.value)
		--print(sc.value)
		tf_setText(tf,sc.value)
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

--local md5file = func_loadmd5('wolf',0.02,"\\resource\\texture\\wolf.tga")

--加载一个obj模型
local box = func_loadobj('box','wolf.tga','myBox')--'box' 'torus'
--setv(box,FLAGS_DRAW_PLOYGON_LINE)

func_set_camera_pos(0,0,-5)


