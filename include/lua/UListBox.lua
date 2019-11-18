local g_gap = 15--(一行的高度间隔)
local g_width = 60 --(一行的宽度)

local r = 1.0;
local g = 0.0;
local b = 0.0;

local function f_create() 
	local listBox=
	{
	
		name,
		type = 8,
		b_drop = false,--是否是下拉着的
		
		tflist={},--textFiled列表
		
		bg=nil,--spirte,容器
		
		callBack = nil,--选择回调
		param;
		
		x=0,y=0,
		
		index = 0,--选择的索引0开始,默认初始化为0
		
		tf,	--文本
		
		titletf,--列表标题文本
	};
	return listBox
end

--显示隐藏文本
local function 
f_tf_vis(list)
	local v = list.b_drop
	for key, value in pairs(list.tflist) do      
		local tf =  value
        --ftext_vis(tf,v == true and 1 or 0)
--		label_set_visible(tf,v == true and 1 or 0);
		label_set_visible(tf,v);
	end 
end
--获取索引(-1开始)
local function
f_get_index(list)
	local x,y = func_get_sprite_mouse_xy(list.bg)
	local f = y / g_gap
	
	--print(string.format("%s\t%s\t%d",tostring(f),string.format('%#.0f',f),math.floor(f)))
	--return tonumber( string.format('%#.0f',f)) - 1
	return math.floor(f) - 1
end
--function listbox_refresh(list)
--    f_tf_vis(list);
--end
--选择一个节点,索引从0开始
function
listbox_select(list,n)
	
	if(n >= 0) then
		list.index = n
		--tf_setText(list.tf,listbox_get_label(list));--设置文本
		--print(list,n,list.callBack);
		if(list.callBack) then list.callBack(list,list.param)end
	end
end

function listbox_get_container(list)
	return list.bg;
end

function 
listbox_set_label(list,label)
--	tf_setText(list.tf,label);
    if(list.tf) then
        label_set_text(list.tf,label);
    end
end

local function f_select_call(list)
	list.b_drop = not list.b_drop
	local height = g_gap
	f_tf_vis(list)
	
	if(list.b_drop) then
		--展开
		height = (func_get_table_count(list.tflist) + 1) * g_gap
	end
	
	func_set_sprite_size(list.bg,g_width,height)--重绘背景宽高
	--设置点击区域范围
	sprite_set_hit_rect(list.bg,0,0,g_width,height);
	
	--print(height);
	
	if(list.b_drop == false) then listbox_select(list,f_get_index(list))end
end

--[[
--是否显示着
local function 
f_is_drop(list)
	local w,h = func_get_sprite_size(list.sprite)
	
	if(h > g_gap) then
		return true
	end
	return false
end
--]]

--获取其索引
function listbox_get_index(list)
	return list.index
end

function listbox_get_param(list)
	return list.param;
end
local function f_click(name,param)
	--f_listBoxCallBack(b);
	--local list = func_getTable(name);
	--print(name,param);
		
	local list = param;
	
	--listbox_set_title(list,listbox_get_index(list));
	f_select_call(list);
	
	--print(listbox_get_index(list));
	--listbox_set_title(list,listbox_get_index(list));
end
function 
listbox_new(_x,_y)
	_x = _x or 0
	_y = _y or 0
	
	local list = f_create()
	
	--func_tableSave(list)
	list.x = _x
	list.y = _y
	
	--local name = func_getTableName(list)--获取引用名
	
	--list.bg = sprite_create(name,_x,_y,g_width,g_gap,"f_listBoxCallBack","","");
	--list.bg = sprite_create(name,_x,_y,g_width,g_gap);
	--func_setIcon(list.bg,"gundi.png")
	
	list.bg = func_create_grid_sprite(_x,_y,g_width,g_gap,"checkbox.png");--gundi.png
	
	evt_on(list.bg,EVENT_ENGINE_SPRITE_CLICK,f_click,list);

--	list.tf =ftext_create(); --tf_create(128,list.x,list.y,r,g,b);
--  ftext_setpos(list.tf,list.x,list.y);
	
    return list
end

--绑定一个回调函数
function 
listbox_bind(list,callBack,param)
	list.callBack = callBack;
	list.param = param;
end

--获取选择的文本字符
function 
listbox_get_label(list)
	local n = listbox_get_index(list)
--	return func_get_tf_text(list.tflist[n])
    return label_get_text(list.tflist[n]);
end

--增加一个节点
function 
listbox_add(list,str)
	local count =	func_get_table_count(list.tflist) + 1

--[[
	local tf=ftext_create();--tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
    ftext_setpos(tf,list.x,list.y + g_gap*(count));--+ g_width/2

	list.tflist[count - 1] = tf;
    ftext_vis(tf,0);
    ftext_reset(tf,str);
--]]	
	
	local tf = label_create();
	--label_set_pos(tf,list.x,list.y + g_gap*(count));
	list.tflist[count - 1] = tf;
	label_set_visible(tf,0);
	label_set_text(tf,str);
	--print(list.bg,tf);
	func_addnode(list.bg,tf,0,g_gap*(count));
	--local c =ftext_get_container(tf);
end

--为列表选项卡设置一个标题
function listbox_set_title(self,str)
	if(self.titletf == nil) then
		local tf = label_create();
		--label_set_pos(tf, self.x + g_width, self.y);
		func_addnode(self.bg,tf,g_width, 0);
		
		self.titletf = tf;
	end
	label_set_text(self.titletf, str);
end

--销毁listbox组件
function 
listbox_del(list)
	evt_off(list.bg,EVENT_ENGINE_SPRITE_CLICK,f_click);

	ptr_remove(list.bg)

	for key, value in pairs(list.tflist) do
		label_dispose(value)
	end
	--ptr_remove(list.tf)
	
	if(list.tf) then 
		label_dispose(list.tf); 
	end
	
	if(list.titletf)then
		label_dispose(list.titletf); 
	end
	
	func_clearTableItem(list.tflist)
	func_clearTableItem(list)
end
