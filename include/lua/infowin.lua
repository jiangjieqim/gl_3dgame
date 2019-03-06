--信息调试面板

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
    elseif(label == "line") then
		ex:switch_spriteline()
        print("切换为Sprite线框状态:"..ex:get_spriteline());
	end
end

function infowin(x,y)
	local list =  listbox_new(x or 0,y or 0)
	listbox_add(list,"ex_info")
	listbox_add(list,"gc")
    listbox_add(list,"line")--切换线框渲染sprite

	
	listbox_bind(list,f_select)
	--listbox_select(list,0)--默认选0号索引
end