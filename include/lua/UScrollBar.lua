--#######################################################
--	ScrollBar组件
--
--
--#######################################################

--默认宽高
local defaultBg_width = 100
local defaultBg_height = 14
--小按钮宽度
local barSize = defaultBg_height
--背景后缀
local BgSuffix = '_bg'


local dragDirection = CONST_DIRECTION_HORIZONTAL --水平

--创建一个按钮结构体
local function f_create()
	return {
		--对象类型
		type = "ScrollBar",
		
		--滚动按钮实例(Sprite)
		btn = nil,
		
		bg = nil,
		
		--滑条的区间(默认设置一个初值)
		s = 0.0,
		e = 1.0,
		
		callBack = nil,
		
		--进度值
		value = nil,
	};
end

--设置百分比
local function 
f_ScDragMove(sc,progress)
	
	if(sc == nil) then
		print('f_ScDragMove sc = nil')
		return
	end	
	
	local width = progress * (sc.e - sc.s)
	
	sc.value = sc.s + width
	
	if(sc.callBack) then
		sc.callBack(sc)
	else
		print(sc.callBack)--回调为nil
	end
end
--获取小按钮的拖拽方向
local function 
f_Sprite_DragDirection(littleName)
	local scrollbtn = func_find_obj(littleName)
	return get_attr(scrollbtn,"dragDirection")
end


--设置小按钮位置(横向)
--[[
	bgName:进度条(Sprite)大背景名
	littleName:小按钮(Sprite)名
	DragDirection:拖拽的方向
--]]
local function 
f_SetBarPostion(bgName,littleName)
	
	--滑动的方向
	local dragDirection=f_Sprite_DragDirection(littleName)
	
	--背景sprite引用
	local sprite = func_find_obj(bgName)--func_getTable(bgName)
	
	--背景局部坐标
	local local_x,local_y = get_attr(sprite,"spriteLocalXY")
	
	--进度条的坐标
	local bx,by = get_attr(sprite,"spritePos")
	
	--进度条的宽高
	local sprite_w,sprite_h = get_attr(sprite,"spriteSize")
	
	--小按钮引用
	local scrollbtn = func_find_obj(littleName)
	
	--小按钮的宽高
	local sc_w,sc_h = get_attr(scrollbtn,"spriteSize")
		
	--local cur_x = --GetBarX(x,sc_w,w)
	
	
	if(dragDirection == CONST_DIRECTION_HORIZONTAL) then
		local v = local_x
		
		local target_x = bx + v 
		
		--print("设置小按钮的位置:"..tostring(target_x)..","..tostring(by)..",sc_w="..sc_w..',sc_h='..sc_h..',sprite_w='..sprite_w..',sprite_h='..sprite_h)

		if (target_x > sprite_w  - sc_w + bx) then
			target_x = sprite_w - sc_w + bx
			--print('纠正!!!')
		end
		
		sprite_setpos(scrollbtn,target_x,by)
		return v / sprite_w
	else

		local v = local_y
		
		local target_y = by + v 
		
		--print("设置小按钮的位置:"..tostring(target_x)..","..tostring(by)..",sc_w="..sc_w..',sc_h='..sc_h..',sprite_w='..sprite_w..',sprite_h='..sprite_h)

		if (target_y > sprite_h  - sc_h + by) then
			target_y = sprite_h - sc_h + by
			--print('纠正!!!')
		end
		
		sprite_setpos(scrollbtn,bx,target_y)
		return v / sprite_h
	end
end
--[[
	点击滑动条上的小按钮
--]]
function 
f_ScrollBarBG_Click2(name)
	
	--大背景
	local sprite = func_find_obj(name)
	--print('########### '..name)
	--print(sprite)
	
	local x,y = get_attr(sprite,"spriteLocalXY")
	local w,h = get_attr(sprite,"spriteSize")
	
	
	--小按钮
	local scName=string.sub(name,0,-string.len(BgSuffix)-1)
	local sc = func_getTable(scName)
	
	--拖拽
	local v =  f_SetBarPostion(name,scName)
	f_ScDragMove(sc,v)
end

function 
f_LuaDragMove2(name,progress) 
	f_ScDragMove(func_getTable(name),progress)
end

function 
scrollBar_new(x,y)
	local sc = f_create()
	
	if(x == nil) then x = 0	end
	if(y == nil) then y = 0 end
	func_tableSave(sc)
	
	local name = func_getTableName(sc)--获取btn引用名

	--背景
	local bg = sprite_create(string.format("%s%s",name,BgSuffix),x,y,defaultBg_width,defaultBg_height,'f_ScrollBarBG_Click2')
	func_setIcon(bg,"gundi.png")
	
	--创建小按钮
	local btn=sprite_create(name,x,y,barSize,barSize,"","f_LuaDragMove2","",dragDirection);

	--设置可拖拽范围
	sprite_setDragScope(btn,0,0,defaultBg_width,defaultBg_height);
	
	--对sc赋值
	sc.bg = bg
	sc.btn = btn
	return sc
end

--删除
function scrollBar_del(sc)
	ptr_remove(sc.btn)
	ptr_remove(sc.bg)
	func_tableDel(sc)
end

--绑定一个回调函数
function 
scrollBar_bind(sc,call)
	sc.callBack = call
end

--设置区间
function 
scrollBar_setRange(sc,s,e)
	sc.s = s
	sc.e = e
end

--获取相关信息
function 
scrollBar_getInfo(sc)
	local bg = sc.bg
	local x,y = get_attr(bg,"spritePos")
	local bgw,bgh = get_attr(bg,"spriteSize")
	return x,y,bgw,bgh
end

--显示一个文本
local function 
f_showTF(x,y,label)
	local tf = tf_create(128,x,y);
	tf_setText(tf,label);
	return tf
end

--添加一个text
function 
scrollBar_add_text(sc,label)
	local x,y,sw,sh=scrollBar_getInfo(sc)
	return f_showTF(x + sw,y,label)
end