CrlUI = {

};
CrlUI.__index = CrlUI;

---- 显示一个文本
-- local function f_showTF(x, y, label)
--    local tf = ftext_create(x, y);
--    ftext_reset(tf, label);
--    return tf
-- end

-- local function f_addTf(sc, label)
--    local x, y, sw, sh = scrollBar_getInfo(sc)
--    return f_showTF(x + sw, y, label)
-- end
local function f_add_rotate(crl, key, x, y)
    local sc = scrollBar_new(x, y);
    local function f_animscHandle(sc)
        func_rotate(crl.o, key, sc.value);
    end

    scrollBar_bind(sc, f_animscHandle);
    scrollBar_setRange(sc, 0, PI)
    scrollBar_label(sc, key);
    crl.sclist[key] = sc;
    return scrollBar_get_rect(sc);
end

local function f_init_skeleton_btn(crl, btn_skeleon)
    btn_label(btn_skeleon, "skeleton")
    btn_bindClick(btn_skeleon, function(b)
        local o = crl.o;
        local suffix = func_get_suffix(o)
        if (suffix == SUFFIX_MD5MESH) then
            -- print(func_get_suffix(o))
            btn_label(b, "skeleton:" .. tostring(func_changeFlags(o, FLAGS_RENDER_DRAWSKELETON)))
        else
            func_error(string.format("%s type = %s", "类型有误", suffix))
        end
    end )
end

local function f_init_scale(crl, scale_sc)

    scrollBar_bind(scale_sc,
    function(sc)
        local v = sc.value
        local o = crl.o;
        func_set_scale(o, v)
        -- tf_setText(scaleTF,'scale '..string.format('%.2f',v))
        scrollBar_label(scale_sc, string.format('scale:%.2f', v));
    end );
    local key = "scale";
    scrollBar_setRange(scale_sc, 0.0, 1 / 50)
    scrollBar_label(scale_sc, key);
    crl.sclist[key] = scale_sc;
end
local function f_init_fps_sc(crl, sc)
    scrollBar_label(sc, "fps");
    scrollBar_bind(sc, function(sc)
        local v = sc.value
        -- print("设置模型("..string.format("%#x",tonumber(model))..') FPS = '..	string.format("%0.2f", v))

        -- local frame = '[frame = '..get_attr(model,"getCurFrame")..']';
        -- tf_setText(fpsTF,"fps:" .. string.format("%0.0f", v));
        scrollBar_label(sc, string.format("fps=%0.0f", v));
        change_attr(crl.o, "fps", tostring(v))
    end )
    scrollBar_setRange(sc, 0, 600)
    crl.sclist["fps"] = sc;
end

local function f_init_set_frame(crl, sc)
    scrollBar_label(sc, 'setframe');
    scrollBar_setRange(sc, -1, 47);
    scrollBar_bind(sc, function(sc)
        local value = sc.value
        local o = crl.o
        local v = math.ceil(value)
        if (v < 0) then
            -- func_error("超出滑动区间",modelUI_bind)
            -- assert(nil,'超出了滑动区间')
            -- return
        end
        change_attr(o, "setframe", v)
        -- tf_setText(setframe_tf,"setframe:" .. string.format("%0.0f", v)  ..frame);
        -- local frame = get_attr(model,"getCurFrame")
        -- local str =  string.format()
        scrollBar_label(sc, '' .. v .. '-' .. tostring(get_attr(o, "getCurFrame")) .. '-' .. get_attr(o, "frameCount"))
    end );
    crl.sclist["setframe"] = sc;
end

local function f_init_lineWidth(crl, sc)
    scrollBar_label(sc, '描边渲染');
    scrollBar_bind(sc,
    function(sc)

        local v = sc.value
        local o = crl.o;
        -- func_set_scale(o,v)
        func_changeFlags(o, FLAGS_GLSL_OUTLINE)
        -- 支持边缘渲染
        func_set_glsl_parms(o, 'lineWidth', v)
--        tf_setText(lineTF, 'line ' .. string.format('%.2f', v))
    end
    )
    crl.sclist["lineWidth"] = sc;
end
local function init(crl)
    crl.nameTf = ftext_create(150, 150);
    ftext_setpos(crl.nameTf, crl.x, crl.y);
    ftext_reset(crl.nameTf, "信息");
    local x, y, w, h;
    local oy = 0;
    w, h = ftext_wordpos(crl.nameTf);
    oy = oy + h;

    x, y, w, h = f_add_rotate(crl, "rx", crl.x, crl.y + oy);

    oy = oy + h;

    x, y, w, h = f_add_rotate(crl, "ry", crl.x, crl.y + oy);

    oy = oy + h;

    x, y, w, h = f_add_rotate(crl, "rz", crl.x, crl.y + oy);

    oy = oy + h;

    -- ########################################################
    -- scale滑动条
    local scale_sc = scrollBar_new(crl.x, crl.y + oy)
    f_init_scale(crl, scale_sc);
    x, y, w, h = scrollBar_get_rect(scale_sc)

    oy = oy + h;
    -- ########################################################
    -- 切换显示骨骼
    local btn_skeleon = btn_create(crl.x, crl.y + oy)
    f_init_skeleton_btn(crl, btn_skeleon);

    x, y, w, h = btn_get_rect(btn_skeleon);
    oy = oy + h;
    -- ########################################################
    -- 设置fps

    local sc = scrollBar_new(crl.x, crl.y + oy)
    f_init_fps_sc(crl, sc);
    x, y, w, h = scrollBar_get_rect(sc);
    oy = oy + h;
    -- ########################################################
    -- 设置md5的当前关键帧
    local sc = scrollBar_new(crl.x, crl.y + oy)
    f_init_set_frame(crl, sc);
    x, y, w, h = scrollBar_get_rect(sc);
    oy = oy + h;
    -- ########################################################
    -- 设置描边渲染
    local sc = scrollBar_new(crl.x, crl.y + oy)
    f_init_lineWidth(crl, sc);
    x, y, w, h = scrollBar_get_rect(sc);
    oy = oy + h;
    -- ########################################################
    -- 模式切换
    local list = ListBox:new(crl.x, crl.y + oy, function(index)
        if (index == 0) then
            func_changeFlags(crl.o, FLAGS_DRAW_PLOYGON_LINE);
        elseif (index == 1) then
            func_changeFlags(crl.o,FLAGS_DISABLE_CULL_FACE)
        end
    end )
    list:add("线框渲染,背面剔除");
    oy = oy + h;

    -- ########################################################

    local btn = btn_create(crl.x + 100, 0);

    btn_bindClick(btn, function(b)
        crl:dispose();
        btn_dispose(func_getTable(b));
        btn_dispose(btn_skeleon);
        list:dispose();
    end
    )
    btn_label(btn, "销毁");

end

function CrlUI:new(x, y)
    local s = {
        x,y,
        nameTf,
        o,-- 控制对象
        sclist,
        stat,
    }
    setmetatable(s, CrlUI);
    s.x = x or 0;
    s.y = y or 0;
    -- s.stat = false;
    s.sclist = { };
    init(s);
    return s;
end

function CrlUI:bind(o)
    --    self.nameTf
    if (self.o) then
        func_set_box_color(self.o, 1, 0, 0)
    end

    self.o = o;
    local str = string.format("%#x,%s", tonumber(o), func_get_name(o))
    ftext_reset(self.nameTf, str);
    --    self.stat = not self.stat;
    --    print(self.stat);
    func_set_box_color(o, 1, 1, 0)
    -- 设置选择的模型黄色框显示
end

function CrlUI:dispose()
    if (self.sclist) then
        for key, value in pairs(self.sclist) do
            scrollBar_del(value);
        end
    end
    fext_dispose(self.nameTf);
    self.sclist = nil;
end
