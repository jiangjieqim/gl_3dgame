cam:position(0, 0, -5);
-- cam:rx(PI * 1.7);

-- ex:bgColor(0.1,0.1,0.1);
-- 地板


local function func_quat(x0, y0, x1, y1, v)
    return quat("Quat_slerp", string.format("%f,%f,%f,%f,%f", x0, y0, x1, y1, v));
end

-- 求垂直向量



local function f_load_c()
    local plane = UnitBase:new();
    -- plane:load_model(nil,"//resource//material//floor.mat");
    plane:load_model();
    plane:setv(FLAGS_REVERSE_FACE);
    plane:setv(FLAGS_DRAW_PLOYGON_LINE);
    plane:scale(10);


    local box = UnitBase:new();
    box:loadbox();
    box:scale(0.25);

    local box1 = UnitBase:new();
    box1:loadbox();
    box1:scale(0.25);

    -- 信息文本
    local tf = ftext_create(128, 128);
    -- tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
    ftext_setpos(tf, 0, 12);



    local sc = scrollBar_new(0, 100)
    scrollBar_bind(sc, function(sc)
        local v = sc.value
        -- print("设置模型("..string.format("%#x",tonumber(model))..') FPS = '..	string.format("%0.2f", v))

        -- local frame = '[frame = '..get_attr(model,"getCurFrame")..']';
        -- tf_setText(fpsTF,"fps:" .. string.format("%0.0f", v));
        --        scrollBar_label(sc, string.format("fps=%0.0f", v));
        --        change_attr(crl.o, "fps", tostring(v))
        --    print(v);

        --    local x, y, w = func_quat(1, 0, 0, -1, v);
        local x, y, w = func_quat(1, 0, 0, -1, v);
        ftext_reset(tf, string.format("%.2f,%.2f,w=%.2f", x, y, w));
        box1:set_position(x, y, 0);
    end )
    scrollBar_setRange(sc, 0, 1)
end

--local function loadObj()
--    local name = func_create_name();
--    -- model = model or 'tri'--'box'--'torus'
--    -- local obj = func_loadobj(model)--box	'torus'
--    local obj = load_model(name, string.format("\\resource\\obj\\torus.obj", model));
--    setMaterial(obj, func_load("//resource//material//wolf.mat"));
--    setv(obj, FLAGS_VISIBLE);
--    setv(obj, FLAGS_DRAW_PLOYGON_LINE);
--end

--[[
                             (1)y
                            ^
                            *
                            *
                            *
                            *
(-1)* * * * * * * * * * * * * * * * * * * * * * * *>x  (1)
                            *0
                            *
                            *
                            *
                            *
                            *
                            *(-1)


--]]


local x, y, z = vec3_cross(0.72, -0.69, 0, 0, 0, 1);
--loadObj();

-- print(x, y, z);--求+90之后的向量
f_load_c();

-- print(vec3_dot(0.72,-0.69, 0,       0,-1,0));
