--设置缓冲区大小
local function ftext_set_buffer(txt, v)
    return ftext(txt, "set_buffer",v);
end
-- 创建文本对象
local function ftext_create(w, h, fw, fh,cam)
    cam = cam or 0;
    local name = func_create_name();
    w = w or 64;
    h = h or 64;

    -- 12,11
    fw = fw or 13;
    fh = fh or 12;
--    fw = fw or 16;fh = fh or 15;
    
	
    local p = ftext(nil, "create", name, string.format("%s,%s,%s,%s,%s", w, h, fw, fh,cam));
    ftext_set_buffer(p,64);--设置预制缓冲区
    return p;
end
-- 设置文本对象坐标
local function ftext_setpos(txt, x, y)
    ftext(txt, "setpos", string.format("%s,%s", x or 0, y or 0));
end
----设置文本对象中的文本坐标
-- function func_ftext_setchar(txt,s,x,y)
--    x = x or 0;
--    y = y or 0;
--    return ftext(txt,"setchar",s,string.format("%s,%s",x,y));
-- end
-- 解析多个字符
local function ftext_parse(txt, s)
    return ftext(txt, "parse", s);
end
-- 显示隐藏
local function ftext_vis(txt, v)
    return ftext(txt, "vis", tostring(v));
end     

-- 擦除
local function fext_clear(txt)
    ftext(txt, "clear");
end
--获取文本
local function ftext_str(txt)
    return ftext(txt, "str");
end
local function ftext_get_wordpos(txt)
--function ftext_getsize(txt)
	--func_error(1);
    return ftext(txt, "wordpos");
end
--获取文本的w,h
local function ftext_get_wh(txt)
	return ftext(txt,"size");
end

--获取ftext的容器,此容器是sprite
local function ftext_get_container(txt)
	return ftext(txt, "get_container");
end
-- 重新设置文本对象
local function ftext_reset(txt, s)
--    print(string.len(s));
    fext_clear(txt);
    ftext_parse(txt, s);
end

-- 销毁
local function fext_dispose(txt)
    if(txt) then
        ftext(txt, "dispose");
    else
        func_error("txt = nil");
    end
end

----分割成中文和英文存储在table中
-- function func_split_cn(s)
----    local b = string.byte(s);
--    local len = string.len(s);
--    print("len=",len);
--    for i=1,len,1 do
--        print( "["..string.sub(s,i-1,1).."]")
--    end
-- end




-- 显示单行连续的文本
-- a,b   ==> a和b
-- function func_ftext_set_much_chars(ft,s,x,y,hGap)
--    hGap = hGap or 16
--    x = x or 0;
--    y = y or 0;
--    local arr =   func_split(s,",");
----    local i = 0;
--    local w,h;
--    for key, value in pairs(arr) do		
----		i = i + 1

--        w,h = func_ftext_setchar(ft,value,x,y);
----        print(w,h);
--        x=x+w;
-- end


----    local len = string.len(s);
----    print(len);
----    for i=1,len,1 do
----        print( string.sub(s,i-1,i))
----    end

----    "[\\0-\127\194-\244][\128-\191]*"
----      "[\0-\x7F\xC2-\xF4][\x80-\xBF]*"
----    for ch in string.gmatch(s, "[\\0-\127\194-\244][\128-\191]*") do
----	    print(ch,#ch~=1)
----    end
-- end


-- FText = {
--    str,--文本数据
--    txtWidth,--文本对象的宽度
-- };
-- FText.__index = FText;
--********************************************************
local function label_create(w,h,cam)
	local label = {
		name = nil,
		--组件类型
		type = 1,
		tf = nil;
	};
	
	label.tf = ftext_create(w,h,13,12,cam);
	return label;
end

local function label_dispose(label)
	fext_dispose(label.tf);
	func_clearTableItem(label);
end

local function label_set_text(label,s)
	ftext_reset(label.tf,s);
end

local function label_get_text(label,s)
	return	ftext_str(label.tf);
end

local function label_set_pos(label,x,y)
	ftext_setpos(label.tf,x,y);
end

local function label_get_wordpos(label)
	return ftext_get_wordpos(label.tf);
end

local function label_set_visible(label,v)
	if(type(v) == "boolean") then
		if(v) then 
			v  = 1;
		else
			v = 0;
		end
	end
	
	ftext_vis(label.tf,v);
end
--***************************************************
NLabel = {
	tf,
};
NLabel.__index = NLabel;
setmetatable(NLabel, Base);

function NLabel:new(w,h,cam)
	local self = Base:new();
	setmetatable(self, NLabel);
	self:settype(14);
	self.tf = ftext_create(w,h,13,12,cam);
	return self;
end

function NLabel:set_text(s)
	ftext_reset(self.tf,s);
end

function NLabel:set_pos(x,y)
	ftext_setpos(self.tf,x,y);
end

function NLabel:get_size()
	return ftext_get_wh(self.tf);
end

function NLabel:get_text()
	return ftext_str(self.tf);
end

function NLabel:visible(v)
	if(v) then
		v = 1;
	else
		v = 0;
	end
	ftext_vis(self.tf,v);
end

function NLabel:dispose()
	--func_error();
	fext_dispose(self.tf);
	func_clearTableItem(self);
end

function NLabel:get_container()
	return ftext_get_container(self.tf);
end