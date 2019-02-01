dofile("..\\include\\lua\\core.lua");
func_print('四元数旋转测试',0xff0000)



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
--小方块
local obj1 = func_loadobj('quad',nil,'myObj1',false)--quad
setv(obj1,FLAGS_RAY)		
setv(obj1,FLAGS_DRAW_RAY_COLLISION)
setv(obj1,FLAGS_DRAW_PLOYGON_LINE)



func_set_scale(obj1,_scale)

--local md5file = func_loadmd5('wolf',0.02,"\\resource\\texture\\wolf.tga")

--加载一个obj模型
--VBO没有渲染出材质
local box = func_loadobj('arrow',nil,'myBox',false)--'box' 'torus' 'teapot' 'arrow'
--setv(box,FLAGS_RAY)					--设置为可拾取状态
setv(box,FLAGS_DRAW_RAY_COLLISION)
setv(box,FLAGS_DRAW_PLOYGON_LINE)




--创建地板
local _floor = func_loadobj('box',nil,'_floor',false);
func_set_scale(_floor,20);
func_set_y(_floor,-10);
setv(_floor,FLAGS_RAY)
setv(_floor,FLAGS_DRAW_RAY_COLLISION)
setv(_floor,FLAGS_DRAW_PLOYGON_LINE)

--加载一个角色模型


local function f_init()
	local horse=func_loadmd2('bauul','bauul.tga','vbo')--'horse'
	func_rename(horse,'_horse');
	
	func_setRotateX(horse,PI/2)--旋转一个轴向
	func_set_scale(horse,0.1)
	func_set_x(horse,-5)
	func_set_z(horse,-5)
	
	change_attr(horse,"animtor_push","stand","0,19");

	change_attr(horse,"animtor_push","run","40,45");

	func_set_anim(horse,"stand")
	
	--func_set_ptr_fps(horse,1)
	
	change_attr(horse,"animtor_play");--播放
end

local horse=f_init()

local _target = func_loadobj('box',nil,'_target',false);
setv(_target,FLAGS_DRAW_PLOYGON_LINE)



func_set_camera_pos(0,-5,-20)


--drawCall回调
function func_drawCall(v)
	
	
	
end
