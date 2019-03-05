--信息面板

local function f_select(_l)
	local label = listbox_get_label(_l)
	--func_set_anim(m,label)
	--string.format('%s vbo=%s',label,tostring(func_is_vbo(obj)) )
	--listbox_set_label(_l,label)
	listbox_set_label(_l,label)
	
	if(label == "ex_info") then
		func_ex_info();
	elseif(label == "gc") then
		func_gc();
	end
end

function infowin_show(x,y)
	local list =  listbox_new(x or 0,y or 0)
	listbox_add(list,"ex_info")
	listbox_add(list,"gc")
	
	listbox_bind(list,f_select)
	--listbox_select(list,0)--默认选0号索引
end