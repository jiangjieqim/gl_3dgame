--加载两个角色模型,点击地面实现移动行走

--设置cam
cam:position(0,-5,-5);
cam:rx(PI * 1.8);
----------------------------------------------------------------------------
--local obj = UnitBase:new();
--obj:loadvbo("\\resource\\md2\\bauul.md2","//resource//material//bauul.mat");
--obj:scale(1/50);
--obj:rx(PI/2);
--obj:y(0.5);






local obj2 = UnitBase:new();
obj2:loadvbo("\\resource\\md2\\bauul.md2","//resource//material//bauul.mat");--gobin
obj2:scale(1/50);
--obj2:setv(FLAGS_DRAW_PLOYGON_LINE);
obj2:rx(PI/2);
obj2:y(0.5);

local animsc = scrollBar_new(100,20);
local function f_animscHandle(sc)
    local v = sc.value * PI * 2;
	--print("### "..tostring(v));
    --func_setRotateY(unit:get_ptr(),v);
    obj2:ry(v);
end
scrollBar_setRange(animsc,0,1)
----animscTf = scrollBar_add_text(animsc,'animsc')
scrollBar_bind(animsc,f_animscHandle)




--选择box
local select = UnitBase:new();
select:loadbox();

--##########################################################
--地板
local plane = UnitBase:new();
--plane:load_model(nil,"//resource//material//floor.mat");
plane:load_model();
plane:setv(FLAGS_REVERSE_FACE);
plane:setv(FLAGS_DRAW_PLOYGON_LINE);
plane:scale(10);
glsl_set(plane.material,string.format("_lineColor,%s","0.5,0.5,0.5"));
glsl_set(plane.material,string.format('uvScale,%s',tostring(plane:get_scale())));--设置diffuse.vs (uniform float _uvScale)uv重复值

local function f_on_click_floor_handle(data)
    local pos = func_split(data,",");--func_xml_to_tb(data);
    obj2:move(pos[1],pos[2],pos[3]);
end
local function f_endCall(msg)
    if(msg == UnitBaseEndMsg) then
--        print(obj2:get_name().."移动结束");
    end
end
if(plane) then
   plane:bindRayPick(f_on_click_floor_handle);
end
evt_on(obj2:get_p(),UnitBaseEvent,f_endCall);


local function f_onkey(data)
	local key = tonumber(data);
	--print('my key = '..key);
	if(key == KEY_A) then
	    --alert(tostring(math.random(0,100)));
        --print("get curFocus point = "..func_get_curFocus());	
	elseif(key == KEY_B)then
        if(plane) then
		    func_changeFlags(plane:get_p(),FLAGS_DRAW_PLOYGON_LINE);
        end
    elseif(key == KEY_ESC) then
        ex:exit();
	end
end
evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);