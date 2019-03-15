cam:position(0, -5, -5);
cam:rx(PI * 1.7);

-- ex:bgColor(0.9,0.9,0.9);
-- 地板
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

local function func_quat(x0, y0, x1, y1, v)
    return quat("Quat_slerp", string.format("%f,%f,%f,%f,%f", x0, y0, x1, y1, v));
end



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
--    print(string.format("%.2f\t%.2f\t%.2f", x, y, w));
    box1:set_position(x,y,0);
end )
scrollBar_setRange(sc, 0, 1)