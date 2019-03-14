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
    crl.nameTf = func_ftext_create();
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

--    local btn = btn_create(crl.x, oy);

--    btn_bindClick(btn,
--    function(b)
--        crl:dispose();
--        --            btn_dispose(func_getTable(b));
--    end
--    )

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

    func_ftext_reset(self.nameTf, "str了");
end

function CrlUI:dispose()
    if (self.sclist) then
        for key, value in pairs(self.sclist) do
            scrollBar_del(value);
        end
    end
    self.sclist = nil;
end
