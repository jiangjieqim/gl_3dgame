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

function ListBox:new(x,y)
    local s = { 
         --    bg,--背景

    b_drop = false,
    -- 是否是下拉着的

    tflist = { },
    -- textFiled列表

    bg = nil,
    -- spirte

    callBack = nil,
    -- 选择回调

    x = 0,
    y = 0,

    index = 0,-- 选择的索引0开始,默认初始化为0

    tf,-- 文本

    g_gap,  --(一行的高度间隔)

    g_width,--(一行的宽度)
    
    }
    setmetatable(s, ListBox);
    s:init(x,y);
    --    self:init();
--    print(self);  print(s);
    return s;
end
function ListBox:tf_vis_switch()
	local list  = self;
    local v = list.b_drop
    
    local _stat = v == true and 1 or 0;
	for key, value in pairs(list.tflist) do      
        func_ftext_vis(value,_stat);
	end 
end
local function f_click(name)
   -- print(self,name);
--    print(self.b_drop);
--    self.b_drop = not self.b_drop
--	local height = self.g_gap


   local list = func_getTable(name);
    print(list.x);
    list:tf_vis_switch();
end
function ListBox:init(_x,_y)
    --    print(self);
    _x = _x or 0;
	_y = _y or 0;
    self.g_gap = 15;
    self.g_width= 60;
    self.x = _x;
    self.y = _y;
    self.b_drop = false;
    local name = func_tableSave(self);
    self.bg = sprite_create(name,_x,_y,self.g_width,self.g_gap);
    func_setIcon(self.bg,"gundi.png");

    evt_on(self.bg,EVENT_ENGINE_SPRITE_CLICK,f_click);

--    setv(self.bg,FLAGS_VISIBLE);
end
--function ListBox:get_tfList()
--    return self.tflist;
--end
function ListBox:add(str)
    local c = func_get_table_count(self.tflist)+1;
    print(c,self.tflist,self);
    local tf=func_ftext_create();--tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
    func_ftext_setpos(tf,self.x,self.y + self.g_gap*c);
--    func_ftext_vis(tf,0);
    func_ftext_reset(tf,str);
    self.tflist[c-1]=tf;
end
