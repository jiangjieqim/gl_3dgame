--cam:position(0, 0, -5);
-- cam:rx(PI * 1.7);
JEngine:getIns():get_cam():set_pos(0,0,-5);

local function quat_slerp(x0, y0, z0, x1, y1,z1, v)
    return quat("Quat_slerp", x0, y0,z0, x1, y1,z1, v);
end

-- 求垂直向量
local function f_load_c()
	
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

		
		
		--顺时针
		--0 <= angle <=90			90
        --local x, y, z,w = quat_slerp(	0,1,0,	 1,0,0, v);
															
			
		--45												
		--local x, y, z,w = quat_slerp(	0,1,0,	 -1,1,0, v);
	
		
		-- 90<angle<=180		逆时针 135
		local x, y, z,w = quat_slerp(	1,	0,	0, 		-1,	1,	0, v);
									
			
		--		顺时针 135
		local x, y, z,w = quat_slerp(-1,	1,	0, 		1,	0,	0, v);
									
		--		180
		local x, y, z,w = quat_slerp(1,0,0, 		-1,	0,	0, v);
		
		
		--s 45
		local x, y, z,w = quat_slerp(1,0,0, 		1,	-1,	0, v);
		--s 90
		local x, y, z,w = quat_slerp(1,0,0, 		0,	-1,	0, v);


        --ftext_reset(tf, string.format("%.2f,%.2f,w=%.2f", x, y, w));
        --tf:set_text(string.format("%.2f,%.2f,w=%.2f", x, y, w));
		
		--print(x, y, z,w);
		
		box1:set_position(x, y, z);
	end
	
	sc:bindCallback(scHandler);
	
	scHandler(1);
    --scrollBar_setRange(sc, 0, 1)
	
	--local m = math.sqrt(0.707*0.707 + 0.707*0.707);	-- 1
	--print(m);
end


--[[


(-1,1)                       (0,1)y						(1,1)
                            ^
                            *
                            *
                            *
                            *
(-1,0)* * * * * * * * * * * * * * * * * * * * * * * *>x  (1,0)
                            *0
                            *
                            *
                            *
                            *
                            *
 (-1,-1)                    *(0.-1)						(1,-1)			


--]]


--local x, y, z = vec3_cross(0.72, -0.69, 0, 0, 0, 1);
--loadObj();

-- print(x, y, z);--求+90之后的向量
f_load_c();

-- print(vec3_dot(0.72,-0.69, 0,       0,-1,0));
