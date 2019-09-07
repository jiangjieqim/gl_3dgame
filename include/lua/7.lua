--加载两个角色模型,点击地面实现移动行走

--设置cam

cam_setRotateX(PI * 1.8);
cam_setPosition(0,0,-6);
--cam:rx(PI * 1.8);
----------------------------------------------------------------------------
--local obj = UnitBase:new();
--obj:loadvbo("\\resource\\md2\\bauul.md2","//resource//material//bauul.mat");
--obj:scale(1/50);
--obj:rx(PI/2);
--obj:y(0.5);



--加载一个角色模型
--[[
local obj2 = UnitBase:new();
obj2:loadvbo("\\resource\\md2\\bauul.md2","//resource//material//bauul.mat");--gobin,bauul


obj2:scale(1/50);
--obj2:setv(FLAGS_DRAW_PLOYGON_LINE);
obj2:rx(PI/2);
obj2:y(0.5);
--]]

local node =  node_test();



--[[
obj2:loadvbo("\\resource\\md2\\triangle.md2","//resource//material//wolf.mat");
--obj2:setv(FLAGS_DRAW_PLOYGON_LINE);
obj2:setv(FLAGS_DISABLE_CULL_FACE);
--]]

--选择box
local select = UnitBase:new();
select:loadbox();
--select:set_position(0,-2,-6);

local animsc = scrollBar_new(100,20);
local function f_animscHandle(sc)
    local v = sc.value * PI * 2;
	--print("### n * PI, n = "..tostring(v/PI));
    --func_setRotateY(unit:get_ptr(),v);
    
	scrollBar_label(animsc,v);
	if(obj2) then
	--	obj2:ry(v);
    end
	
	if(node) then
		node_setRY(node,v);
	end
	
    if(select)then
    --    select:ry(v);
    end

 --  cam_setRotateX(v);
end
scrollBar_setRange(animsc,0,1)
----animscTf = scrollBar_add_text(animsc,'animsc')
scrollBar_bind(animsc,f_animscHandle)







--##########################################################
--地板
local plane = UnitBase:new();
--plane:load_model(nil,"//resource//material//floor.mat");
plane:load_model();
plane:setv(FLAGS_REVERSE_FACE);
plane:setv(FLAGS_DRAW_PLOYGON_LINE);
plane:scale(20);
glsl_set(plane.material,string.format("_lineColor,%s","0.5,0.5,0.5"));
glsl_set(plane.material,string.format('uvScale,%s',tostring(plane:get_scale())));--设置diffuse.vs (uniform float _uvScale)uv重复值
--plane:set_position(0,-2,0);
local function f_on_click_floor_handle(data)
    local pos = func_split(data,",");--func_xml_to_tb(data);
    if(obj2) then
		obj2:move(pos[1],pos[2],pos[3]);
    end
--	print("请求移动");
end
local function f_endCall(msg)
    if(msg == UnitBaseEndMsg) then
--        print(obj2:get_name().."移动结束");
    end
end
if(plane) then
   plane:bindRayPick(f_on_click_floor_handle);
end

if(obj2) then
    evt_on(obj2:get_p(),UnitBaseEvent,f_endCall);
end

local function f_onkey(data)
	local key = tonumber(data);
	--print('my key = '..key);
	if(key == KEY_A) then
	    
        --print("get curFocus point = "..func_get_curFocus());	
	elseif(key == KEY_B)then
        if(plane) then
		    func_changeFlags(plane:get_p(),FLAGS_DRAW_PLOYGON_LINE);
        end
    elseif(key == KEY_ESC) then
        ex:exit();
	end
end
--if(obj2) then
    evt_on(obj2,EVENT_ENGINE_KEYBOARD,f_onkey);
--end