-- dofile("..\\include\\lua\\core.lua")

func_print('#######################\t材质测试\t#######################', 0xff0000)
--local _model_ptr;

-- 初始化标示
local function f_set_obj_flags(o)
    setv(o, FLAGS_VISIBLE);
    -- setv(o,FLAGS_GLSL_OUTLINE);
    --  setv(o,FLAGS_DRAW_PLOYGON_LINE);--线框渲染
    -- setv(o,FLAGS_DRAW_NORMAL);


    -- 射线拾取
    --- [[
    setv(o, FLAGS_RAY);
    setv(o, FLAGS_DRAW_RAY_COLLISION);
    setv(o, FLAGS_RENDER_BOUND_BOX);
    -- ]]

end

---- 创建茶壶列表
-- local function createObjList(cnt)
--    cnt = cnt or 1
--    local offset = 3

--    local seedNumber = - offset

--    for i = 0, cnt - 1, 1 do

--        local time = func_get_longTime()
--        seedNumber = seedNumber + offset;
--        local _m = func_loadobj("torus")
--        -- load_objVBO("torus");	--"teapot"
--        setv(_m, FLAGS_DRAW_PLOYGON_LINE);

--        func_set_x(_m, seedNumber)

--        func_print(string.format('load_objVBO 消耗%d ms', func_get_longTime() - time))

--    end

-- end
-- 加载一个带透明通道的树叶模型
local function
    load_alpha_model(objName)
    local time = func_get_longTime()
    objName = objName or "quad"
    local o = func_loadobj(objName)
    -- load_VBO_model(name,"\\resource\\obj\\"..objName..".obj");
    local m = createMaterial("vboDiffuse", "\\resource\\texture\\leaf.tga");
    setMaterial(o, m);
    f_set_obj_flags(o);
    func_print(string.format('load_alpha_model 消耗 %d ms', func_get_longTime() - time))
    return o;
end
local function f_setLabel(label, obj)
    -- local label = listbox_get_label(_l)

    -- listbox_set_label(_l, string.format('%s vbo=%s', label, tostring(func_is_vbo(obj))))

    if (label == 'diffuse') then

    elseif (label == 'ploygonLine') then

        setv(obj, FLAGS_DRAW_PLOYGON_LINE)

    elseif (label == 'outline') then

        -- setv(obj,FLAGS_DISABLE_CULL_FACE)--先设置双面
        if (func_is_vbo(obj)) then
            func_error("vbo不支持!!!")
        end
        setv(obj, FLAGS_OUTLINE)
    elseif (label == 'point') then

        setv(obj, FLAGS_DRAW_PLOYGON_POINT)

    elseif (label == 'glslOutline') then
        -- setv(obj,FLAGS_DRAW_PLOYGON_LINE)

        -- 有一些情况着色器中没有这些变量就会设置无效

        func_set_glsl_parms(obj, 'lineWidth', 0.05)
        func_set_glsl_parms(obj, 'alpha', 0.5)

        setv(obj, FLAGS_GLSL_OUTLINE)
        -- print('***')
    elseif (label == 'normal') then
        if (func_is_vbo(obj)) then
            func_error("vbo不支持!!!")
        end
        setv(obj, FLAGS_DRAW_NORMAL)
        -- 绘制法线

    elseif (label == 'drawCollison') then
        if (func_is_vbo(obj)) then
            func_error("vbo不支持!!!")
        end

        setv(obj, FLAGS_DRAW_RAY_COLLISION)
    end

end

-- ###############################################################
-- 初始化listbox,用来测试不同的材质
local function f_shader_init()
    local g_model;
    local config = "diffuse,ploygonLine,outline,point,drawCollison,glslOutline,normal";

    local listbox = ListBox:new(0, 0,
    function(index)

        local _l = listbox;
        if (g_model) then
            func_ptr_remove(g_model)
        end


        -- local obj =func_loadobj("torus")
        local t = func_get_longTime();
        local obj = func_fixed_load()

        -- func_fixed_load()--func_loadmd5('wolf',0.01)--func_loadobj("torus")

        if (obj) then

            g_model = obj

            --            _model_ptr = g_model
            local arr = func_split(config);
            local s = string.format('index = %d\tlabel = [%s]\t	vbo:%s',
            index,
            arr[index + 1],
            tostring(func_is_vbo(obj))
            )

            print(s..","..(func_get_longTime() - t));

            f_setLabel(arr[index + 1], g_model);
        end

    end )

    listbox:add(config)
    --######################################################
    -- 渲染回调
    local _ticket = 0;
    local tt = 0;
--    evt_on(listbox, EVENT_ENGINE_RENDER_3D, function()

--        -- print(func_fps())
--        -- drawCall回调
--        if (func_get_longTime() - tt <= 20) then
--            -- 相关毫秒一次
--            return
--        end

--        tt = func_get_longTime();

--        if (g_model) then
--            -- _temp += PI--func_fps()*PI
--            local fps = ex:fps();
--            local f = func_get_longTime() / fps
--            -- *PI

--            _ticket = _ticket +(1000 / fps) * 0.1
--            -- print(string.format("f = %.3f fps = %d ticket = %.3f",f, func_fps(),_ticket))

--            func_setRotateX(g_model, _ticket);
--        end
--    end
--    );

    return listbox
end

-- #########################################################

-- cam:position(0,0,-2)
-- load_alpha_model()

cam:position(0, 0, -10)
local list = f_shader_init();
-- listbox_select(list,0)--默认选择一个
