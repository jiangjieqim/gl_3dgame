
ListBox = {

};

ListBox.__index = ListBox;
--callBack	index
function ListBox:new(x, y, callBack)
    local s = {
        --    bg,--背景

        b_drop = false,
        -- 是否是下拉着的

        tflist = { },
        -- textFiled列表

        str = "";


        bg = nil,
        -- spirte

        callBack = nil,
        -- 选择回调

        x = 0,
        y = 0,

        --        index = 0,-- 选择的索引0开始,默认初始化为0

        --        tf,-- 文本

        g_gap,-- (一行的高度间隔)

        g_width,-- (一行的宽度)
		
		isSetTitle=nil,	--是否自动设置标题栏文本
    }
    setmetatable(s, ListBox);
    s.callBack = callBack;
    s:init(x, y);
    --    self:init();
    --    print(self);  print(s);
    return s;
end
-- 获取索引(-1开始)
local function f_get_index(list)
    local x, y = func_get_sprite_mouse_xy(list.bg);
    -- print(string.format("%s\t%s\t%d",tostring(f),string.format('%#.0f',f),math.floor(f)))
    -- return tonumber( string.format('%#.0f',f)) - 1
    return math.floor(y / list.g_gap) -1;
end

--设置文本
local function f_set_label(list, label)
    if (list.tf == nil) then
        local tf = ftext_create();
        -- tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
        ftext_setpos(tf, list.x, list.y);
        list.tf = tf;
    end
    ftext_reset(list.tf, label);
end
--设置标题
function ListBox:setTitle(str)
    local list = self;
	f_set_label(self, str);--设置标题文本
end

function ListBox:isSetTitle(v)
	self.isSetTitle = v;
end
-- 根据b_drop的值显示隐藏文本
function ListBox:tf_vis_switch()
    local arr = func_split(self.str, ",");

    local list = self;

    local v = list.b_drop

    local _stat = v == true and 1 or 0;
    --    for key, value in pairs(list.tflist) do
    --        ftext_vis(value, _stat);
    --    end
    for key, value in pairs(list.tflist) do
        fext_dispose(value);
    end
    list.tflist = { };

    local c = 0;
    if (_stat == 1) then
        for key, value in pairs(arr) do
            --            fext_dispose(value);
            if (string.len(value) > 0) then
                -- print(value, string.len(value));
                self:build(value);
                c = c + 1;
            end
        end
        --        print(c);
    end
	local w = self.g_width;
	local h = (c + 1) * self.g_gap;
    func_set_sprite_size(self.bg, w,h);
	sprite_set_hit_rect(self.bg,0,0,w,h);
	-- 重绘背景宽高
    --    print(f_get_index(self));
    ----    func_gc();
    --    func_ex_info();

    if (self.b_drop == false and self.callBack) then
        local i = f_get_index(self);
        if (i >= 0) then
            --f_set_label(self, arr[i + 1]);--设置标题文本
			if(self.isSetTitle == true) then
				self:setTitle(arr[i+1]);
			end
			self.callBack(i);
        end
    end
end

local function f_click(name)
    -- print(self,name);
    --    print(self.b_drop);
    --    self.b_drop = not self.b_drop
    -- local height = self.g_gap


    local list = func_getTable(name);
    -- print(list.x);

    list.b_drop = not list.b_drop

    list:tf_vis_switch();
end
function ListBox:init(_x, _y)
    --    print(self);
    _x = _x or 0;
    _y = _y or 0;
    self.g_gap = 15;
    self.g_width = 60;
    self.x = _x;
    self.y = _y;
    self.b_drop = false;
    local name = func_tableSave(self);
    self.bg = sprite_create(name, _x, _y, self.g_width, self.g_gap);
    func_setIcon(self.bg, "gundi.png");

    evt_on(self.bg, EVENT_ENGINE_SPRITE_CLICK, f_click);

    --    setv(self.bg,FLAGS_VISIBLE);
end
-- function ListBox:get_tfList()
--    return self.tflist;
-- end

function ListBox:build(str)
    local c = func_get_table_count(self.tflist);
    --    print(c,self.tflist,self);
    local tf = ftext_create();
    -- tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
    ftext_setpos(tf, self.x, self.y + self.g_gap *(c + 1));
    -- ftext_vis(tf,0);
    ftext_reset(tf, str);
    self.tflist[c] = tf;
end

function ListBox:add(str)
    --    local c = func_get_table_count(self.tflist) + 1;
    --    --    print(c,self.tflist,self);
    --    local tf = ftext_create();
    --    -- tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
    --    ftext_setpos(tf, self.x, self.y + self.g_gap * c);
    --    -- ftext_vis(tf,0);
    --    ftext_reset(tf, str);
    --    self.tflist[c - 1] = tf;
    if (string.len(self.str) > 0) then
        self.str = string.format("%s,%s", self.str, str);
    else
        self.str = str;
    end
    -- print(self.str);
end
function ListBox:dispose()
    local list = self;
    for key, value in pairs(list.tflist) do
        fext_dispose(value);
    end
    list.tflist = { };
    if(self.tf) then
        fext_dispose(self.tf);
    end
    ptr_remove(list.bg);
end