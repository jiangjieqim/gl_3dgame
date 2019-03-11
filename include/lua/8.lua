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



local function ftextTest()
    
    local w = 60;--42,20
    local h = w;

    --local btn = btn_create(200,100,w,h/2,"gundi.png");

    local ft = func_ftext_create(w,h);--18,16,16,15
    func_ftext_setpos(ft,50,50);
    local _time = func_get_longTime();


    --      func_ftext_parse(ft,func_loadfile("\\resource\\obj\\sphere.obj"));  --teapot

   

    func_fext_clear(ft);
    local tw,th=func_ftext_parse(ft,'输出引擎信息看看');
    print("文本宽高",tw,th);
--    func_ftext_reset(ft,math.random(0,1000));
    print('耗时\t'..(func_get_longTime()-_time));

    func_ftext_vis(ft,1);
    -- func_fext_dispose(ft);
end

--cam:position(0,0,-3.5);

--ftextTest();

--btn_create();

--local obj = UnitBase:new();
--obj:loadvbo("\\resource\\md2\\bauul.md2","//resource//material//bauul.mat");

--local sprite = sprite_create('name',0,0,100,20);
--func_setIcon(sprite,'smallbtn');



function loadmd2()
    local obj = UnitBase:new();
    obj:loadvbo("\\resource\\md2\\bauul.md2","//resource//material//bauul.mat");
    obj:scale(1/50);
    obj:setv(FLAGS_DRAW_PLOYGON_LINE);
    --obj:setv(FLAGS_RAY);
    --obj:load_collide("\\resource\\md2\\bauul.md2");
    --obj:setv(FLAGS_DRAW_RAY_COLLISION);
    obj:rx(PI/2);
    obj:y(0.5);
    --print("obj.name = ",obj:get_name());
    return obj;
end
function createFloor()
--地板
    local plane = UnitBase:new();
    --plane:load_model(nil,"//resource//material//floor.mat");
    plane:load_model();
    plane:setv(FLAGS_REVERSE_FACE);
    plane:setv(FLAGS_DRAW_PLOYGON_LINE);
    plane:scale(10);
    glsl_set(plane.material,string.format("_lineColor,%s","0.5,0.5,0.5"));
    glsl_set(plane.material,string.format('uvScale,%s',tostring(plane:get_scale())));--设置diffuse.vs (uniform float _uvScale)uv重复值
    return plane;
end

--local obj = loadmd2();
local function f_on_click_floor_handle(data)
    local pos = func_split(data,",");--func_xml_to_tb(data);
    if(obj) then
        obj:move(pos[1],pos[2],pos[3]);
    end
end

--createFloor():bindRayPick(f_on_click_floor_handle);

cam:position(0,-5,-5);
cam:rx(PI * 1.8);