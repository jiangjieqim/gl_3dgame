--print("**************dasds ********");
--infowin_show(0,20);


function test()
	

	---[[
	local _floor = load_model(func_create_name(),"\\resource\\obj\\plane.obj")		-- func_loadobj('quad',nil,'myObj1',false)--quad
	setMaterial(_floor,func_load("//resource//material//floor.mat"));	
	--setv(_floor,FLAGS_VISIBLE);
	setv(_floor,FLAGS_RAY)
	--setv(_floor,FLAGS_DRAW_RAY_COLLISION)		--BUG 启动的时候会有空指针异常
	setv(_floor,FLAGS_DISABLE_CULL_FACE);

	local r = 0;
	


	cam:position(0,0,-5);

	func_setRotateX(_floor,PI)
	func_setRotateY(_floor,PI/2)
	func_setRotateZ(_floor,PI/2)
	--]]

	--scxzxAetv(_floor,FLAGS_DRAW_PLOYGON_LINE)--线框
    cam:position(0,0,-5);
end


-------------------------------------------------------------

local btn = btn_create(50,50,64,64,"gundi.png");
local ft = func_ftext_create();--18,16,16,15
func_ftext_setpos(ft,50,50);
func_ftext_parse(ft,"按钮说明大");
