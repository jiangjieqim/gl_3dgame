--cam:position(0, 0, -5);
-- cam:rx(PI * 1.7);
JEngine:getIns():get_cam():set_pos(0,0,-5);



local function func_quat(x0, y0, z0, x1, y1,z1, v)
    return quat("Quat_slerp", x0, y0,z0, x1, y1,z1, v);
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
    local tf = NLabel:new(256,256); 	--ftext_create(128, 128);
    -- tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
    --ftext_setpos(tf, 0, 12);
	tf:set_pos(0,12);

	
    local sc = NScrollBar:new(0,100)
    
	
	local function scHandler(progress)
		local v = progress;
		
        -- print("设置模型("..string.format("%#x",tonumber(model))..') FPS = '..	string.format("%0.2f", v))

        -- local frame = '[frame = '..get_attr(model,"getCurFrame")..']';
        -- tf_setText(fpsTF,"fps:" .. string.format("%0.0f", v));
        --        scrollBar_label(sc, string.format("fps=%0.0f", v));
        --        change_attr(crl.o, "fps", tostring(v))
        --    print(v);

        --    local x, y, w = func_quat(1, 0, 0, -1, v);
        local x, y, z,w = func_quat(-1, 0, 0, 
									 -1, -1,0, v);

        --ftext_reset(tf, string.format("%.2f,%.2f,w=%.2f", x, y, w));
        --tf:set_text(string.format("%.2f,%.2f,w=%.2f", x, y, w));
		
		--print(x, y, z,w);
		
		box1:set_position(x, y, z);
	end
	
	sc:bindCallback(scHandler);
    --scrollBar_setRange(sc, 0, 1)
end


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
