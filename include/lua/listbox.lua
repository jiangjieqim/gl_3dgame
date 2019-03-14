----------------------------------------------------------------------------------------
----下拉列表组件
-- ListBox={}

----显示隐藏文本
-- local function showList(listBox,v)
-- if(listBox) then
-- 	for i = 0, listBox.length-1 do
-- 		--table.insert(arr,1,i)
-- 		local tf=listBox.tflist[i];			

-- 		if(v == 1) then
-- 			setv(tf,flags.FLAGS_VISIBLE)
-- 		else
-- 			--if(i > 0) then
-- 				resetv(tf,flags.FLAGS_VISIBLE);
-- 			--end
-- 		end

-- 	end
-- end
-- end

-- function ListBox.selectByIndex(listBox,index)
-- if(index >=0 and listBox.clickEvtCallBack)	then
-- 	listBox.selectIndex = index;

-- 	--设置文本
-- 	dofunc("tf_copy",listBox.tf,listBox.tflist[index]);
-- 	local str =	dofunc("tf_get_str",listBox.tf);
-- 	listBox.selectLabel = str;
-- 	listBox.clickEvtCallBack(listBox);
-- end
-- end

-- function listBoxCallBack(name)
-- local listBox = instance[name];

-- local size = listBox.width..","..listBox.gap;
-- local isShowList = 0;
-- if(listBox.isOpen) then
-- 	size = listBox.width..","..tostring(listBox.gap*(listBox.length+1));
-- 	isShowList = 1;
-- end
-- showList(listBox,isShowList);
-- change_attr(listBox.sprite,"sprite_resize",size);

-- ------------
-- local localX , localY=get_attr(listBox.sprite,"spriteLocalXY");
-- --print(localX..","..localY);
-- local index = localY/listBox.gap-1;
-- --print(index)--取index的整数部分
-- index = tl_getIntegralPart(index)

-- ListBox.selectByIndex(listBox,index);

-- listBox.isOpen=not listBox.isOpen;
-- end

-- function ListBox.new(_x,_y,callBack)
-- local listBox=
-- {
-- 	length = 0;
-- 	--list={};
-- 	tflist={};
-- 	sprite=nil;
-- 	clickEvtCallBack = nil;
-- 	gap = 15;	--(一行的间隔)
-- 	width = 60; --(宽度)
-- 	x=0;
-- 	y=0;
-- 	isOpen = 0;
-- 	selectIndex=-1;
-- 	selectLabel=nil;
-- 	tf=nil;
-- };
-- listBox.x = _x;
-- listBox.y = _y;
-- local name = tableToInt(listBox);--tostring(sc);
-- instance[name] = listBox;
-- listBox.sprite=sprite_create(name,_x,_y,listBox.width,listBox.gap,"listBoxCallBack","","");
-- listBox.clickEvtCallBack = callBack;

-- func_setIcon(listBox.sprite,"gundi.png")



-- listBox.tf=tf_create(128,_x,_y,1.0,1.0,1.0);

-- return listBox;
-- end

-- function ListBox.push(p,str)
-- --p.list[p.length] = str;
-- local tf=tf_create(128,p.x,p.y+p.gap*(p.length+1),1.0,1.0,1.0);
-- p.tflist[p.length] = tf;
-- tf_setText(tf,str);

-- resetv(tf,flags.FLAGS_VISIBLE);

-- p.length=p.length+1;
-- end

-- local function disposeTflist(listBox,v)
-- if(listBox) then
-- 	for i = 0, listBox.length-1 do
-- 		--table.insert(arr,1,i)
-- 		local tf=listBox.tflist[i];			
-- 		ptr_remove(tf);
-- 	end
-- end
-- end

-- function ListBox.dispose(listBox)
-- disposeTflist(listBox);
-- ptr_remove(listBox.sprite);
-- ptr_remove(listBox.tf);
-- listBox.list = nil;
-- listBox = nil;
-- end


ListBox = {

};

ListBox.__index = ListBox;

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
local function f_set_label(list, label)
    if (list.tf == nil) then
        local tf = func_ftext_create();
        -- tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
        func_ftext_setpos(tf, list.x, list.y);
        list.tf = tf;
    end
    func_ftext_reset(list.tf, label);
end
-- 根据b_drop的值显示隐藏文本
function ListBox:tf_vis_switch()
    local arr = func_split(self.str, ",");

    local list = self;

    local v = list.b_drop

    local _stat = v == true and 1 or 0;
    --    for key, value in pairs(list.tflist) do
    --        func_ftext_vis(value, _stat);
    --    end
    for key, value in pairs(list.tflist) do
        func_fext_dispose(value);
    end
    list.tflist = { };

    local c = 0;
    if (_stat == 1) then
        for key, value in pairs(arr) do
            --            func_fext_dispose(value);
            if (string.len(value) > 0) then
                -- print(value, string.len(value));
                self:build(value);
                c = c + 1;
            end
        end
        --        print(c);
    end

    func_set_sprite_size(self.bg, self.g_width,(c + 1) * self.g_gap);
    -- 重绘背景宽高
    --    print(f_get_index(self));
    ----    func_gc();
    --    func_ex_info();

    if (self.b_drop == false and self.callBack) then
        local i = f_get_index(self);
        if (i >= 0) then
            f_set_label(self, arr[i + 1]);
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
    local tf = func_ftext_create();
    -- tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
    func_ftext_setpos(tf, self.x, self.y + self.g_gap *(c + 1));
    -- func_ftext_vis(tf,0);
    func_ftext_reset(tf, str);
    self.tflist[c] = tf;
end

function ListBox:add(str)
    --    local c = func_get_table_count(self.tflist) + 1;
    --    --    print(c,self.tflist,self);
    --    local tf = func_ftext_create();
    --    -- tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
    --    func_ftext_setpos(tf, self.x, self.y + self.g_gap * c);
    --    -- func_ftext_vis(tf,0);
    --    func_ftext_reset(tf, str);
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
        func_fext_dispose(value);
    end
    list.tflist = { };
    if(self.tf) then
        func_fext_dispose(self.tf);
    end
    ptr_remove(list.bg);
end