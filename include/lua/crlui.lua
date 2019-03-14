CrlUI ={

};
CrlUI.__index = CrlUI;

--显示一个文本
local function f_showTF(x,y,label)
	local tf = func_ftext_create(x,y);
	func_ftext_reset(tf,label);
	return tf
end

local function f_addTf(sc,label)
	local x,y,sw,sh=scrollBar_getInfo(sc)
	return f_showTF(x + sw,y,label)
end
local function init(crl)
    crl.nameTf = func_ftext_create();
    func_ftext_setpos(crl.nameTf,crl.x,crl.y);
    func_ftext_reset(crl.nameTf, "信息");
    local w,h=func_ftext_getsize(crl.nameTf);

    crl.rxsc = scrollBar_new(crl.x,crl.y+h);
    local function f_animscHandle(sc)
        local v = sc.value * PI * 2;
	    print("### "..tostring(v));
        --func_setRotateY(unit:get_ptr(),v);
--        obj2:ry(v);
    end

    scrollBar_bind(crl.rxsc,f_animscHandle);




end

function CrlUI:new(x,y)
    local s = {
        x,y,
        nameTf,
        rxsc,
        o,--控制对象
    }
    setmetatable(s, CrlUI);
    s.x = x or 0;
    s.y = y or 0;
    init(s);   
    return s;
end

function CrlUI:bind(o)
--    self.nameTf
    self.o = o;

    func_ftext_reset(self.nameTf, "str了");
end
