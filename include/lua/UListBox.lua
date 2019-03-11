local g_gap = 15--(一行的高度间隔)
local g_width = 60 --(一行的宽度)

local r = 1.0;
local g = 0.0;
local b = 0.0;

local function
f_create()
local listBox=
	{
		b_drop = false,--是否是下拉着的
		
		tflist={},--textFiled列表
		
		bg=nil,--spirte
		
		callBack = nil,--选择回调
		
		x=0,y=0,
		
		index = 0,--选择的索引0开始,默认初始化为0
		
		tf,	--文本
	};
	return listBox
end

--显示隐藏文本
local function 
f_tf_vis(list)
	local v = list.b_drop
	for key, value in pairs(list.tflist) do      
		local tf =  value
--		if(v) then
--			setv(tf,FLAGS_VISIBLE)
--		else
--			resetv(tf,FLAGS_VISIBLE);
--		end
        func_ftext_vis(tf,v == true and 1 or 0)
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
		
		if(list.callBack) then list.callBack(list)end
	end
end

function 
listbox_set_label(list,label)
--	tf_setText(list.tf,label);
    func_ftext_reset(list.tf,label);
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

function listbox_get_index(list)
	return list.index
end
local function f_click(name)
	--f_listBoxCallBack(b);
	local list = func_getTable(name);
	--print(list);
	f_select_call(list);
end
function 
listbox_new(_x,_y)
	_x = _x or 0
	_y = _y or 0
	
	local list = f_create()
	
	func_tableSave(list)
	list.x = _x
	list.y = _y
	
	local name = func_getTableName(list)--获取引用名
	
	--list.bg = sprite_create(name,_x,_y,g_width,g_gap,"f_listBoxCallBack","","");
	list.bg = sprite_create(name,_x,_y,g_width,g_gap);
	
	evt_on(list.bg,EVENT_ENGINE_SPRITE_CLICK,f_click);

	func_setIcon(list.bg,"gundi.png")

	list.tf =func_ftext_create(); --tf_create(128,list.x,list.y,r,g,b);
    func_ftext_setpos(list.tf,list.x,list.y);
	return list
end

--绑定一个回调函数
function 
listbox_bind(list,callBack)
	list.callBack = callBack
end

--获取选择的文本字符
function 
listbox_get_label(list)
	local n = listbox_get_index(list)
--	return func_get_tf_text(list.tflist[n])
    return func_ftext_str(list.tflist[n]);
end

--增加一个节点
function 
listbox_add(list,str)
	local count =	func_get_table_count(list.tflist) + 1
	local tf=func_ftext_create();--tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
    func_ftext_setpos(tf,list.x + g_width/2,list.y + g_gap*(count));
	list.tflist[count - 1] = tf;
	--tf_setText(tf,str);
    func_ftext_reset(tf,str);
	--resetv(tf,FLAGS_VISIBLE);
    --func_ftext_vis(tf,0);
end



--销毁listbox组件
function 
listbox_del(list)
	evt_off(list.bg,EVENT_ENGINE_SPRITE_CLICK,f_click);

	ptr_remove(list.bg)

	for key, value in pairs(list.tflist) do
		ptr_remove(value)
	end
	--ptr_remove(list.tf)
    func_fext_dispose(list.tf);
	func_clearTableItem(list.tflist)
	func_clearTableItem(list)
end
