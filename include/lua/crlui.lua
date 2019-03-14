CrlUI = {

};
CrlUI.__index = CrlUI;

---- 显示一个文本
--local function f_showTF(x, y, label)
--    local tf = func_ftext_create(x, y);
--    func_ftext_reset(tf, label);
--    return tf
--end

--local function f_addTf(sc, label)
--    local x, y, sw, sh = scrollBar_getInfo(sc)
--    return f_showTF(x + sw, y, label)
--end
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

local function init(crl)
    crl.nameTf = func_ftext_create(128,128);
    func_ftext_setpos(crl.nameTf, crl.x, crl.y);
    func_ftext_reset(crl.nameTf, "信息");
    local x, y, w, h;
    local oy = 0;
    w, h = func_ftext_getsize(crl.nameTf);
    oy = oy + h;

    x, y, w, h = f_add_rotate(crl, "rx", crl.x, crl.y + oy);

    oy = oy + h;

    x, y, w, h = f_add_rotate(crl, "ry", crl.x, crl.y + oy);

    oy = oy + h;

    x, y, w, h = f_add_rotate(crl, "rz", crl.x, crl.y + oy);
    
    oy = oy + h;


    --########################################################
    --scale滑动条
	local scale_sc =  scrollBar_new(crl.x, crl.y + oy)
	scrollBar_bind(scale_sc,
        function (sc)
	        local v = sc.value
	        local o = crl.o;
	        func_set_scale(o,v)
	        --tf_setText(scaleTF,'scale '..string.format('%.2f',v))
            scrollBar_label(scale_sc, string.format('scale:%.2f',v));
        end);
    local key = "scale";
    scrollBar_setRange(scale_sc,0.0,1/50)
    scrollBar_label(scale_sc, key);
    crl.sclist[key] = scale_sc;
    x, y, w, h= scrollBar_get_rect(scale_sc)

    oy = oy + h;
    --------------------------------------------
    --########################################################


    --切换显示骨骼
	local btn_skeleon = btn_create(crl.x, crl.y + oy)
	btn_label(btn_skeleon,"skeleton")
	btn_bindClick(btn_skeleon,function (b)
	    local o = crl.o;
	    local suffix = func_get_suffix(o)
	    if (suffix == SUFFIX_MD5MESH) then
		    --print(func_get_suffix(o))
		    btn_label(b,"skeleton:"..tostring(func_changeFlags( o,FLAGS_RENDER_DRAWSKELETON)))
	    else
		    func_error(string.format("%s type = %s","类型有误",suffix))
	    end
    end)
    x, y, w, h=btn_get_rect(btn_skeleon);
    oy = oy + h;
    --########################################################
    local btn = btn_create(crl.x,crl.y + oy);

    btn_bindClick(btn,
    function(b)
        crl:dispose();
        btn_dispose(func_getTable(b));
    end
    )
    btn_label(btn,"销毁");
end

function CrlUI:new(x, y)
    local s = {
        x,y,
        nameTf,
        o,-- 控制对象
        sclist,
    }
    setmetatable(s, CrlUI);
    s.x = x or 0;
    s.y = y or 0;
    s.sclist = { };
    init(s);
    return s;
end

function CrlUI:bind(o)
    --    self.nameTf
    self.o = o;

    func_ftext_reset(self.nameTf, string.format("模型名:%s",func_get_name(o)));
end

function CrlUI:dispose()
    if (self.sclist) then
        for key, value in pairs(self.sclist) do
            scrollBar_del(value);
        end
    end
    func_fext_dispose(self.nameTf);
    self.sclist = nil;
end
