--#######################################################
--	ScrollBar组件
--
--
--#######################################################

--创建一个按钮结构体
local function f_create()
	return {
		--滚动条的宽高
		defaultBg_width,
		defaultBg_height,
	
		--对象类型
		type = "ScrollBar",
		
		--滚动按钮实例(Sprite)
		btn = nil,
		
		bg = nil,
		
		--滑条的区间(默认设置一个初值)
		s = 0.0,
		e = 1.0,
		
		callBack = nil,
		param = nil,
		
		--进度值
		value = nil,
		
		click = nil,

        --描述文本
        --tf = nil,
		
		--parent = nil,	--在 func_addchild_scrollBar 会对其赋值
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
		--print("滚动条的回调句柄为nil!")--回调为nil
	end
end
--获取小按钮的拖拽方向
local function 
f_Sprite_DragDirection(scrollbtn)
	--local scrollbtn = func_find_obj(littleName)
	return get_attr(scrollbtn,"dragDirection")
end


--设置小按钮位置(横向)
--[[
	bgName:进度条(Sprite)大背景名
	littleName:小按钮(Sprite)名
	DragDirection:拖拽的方向
--]]
local function 
f_SetBarPostion(sprite,scrollbtn)
	
	--滑动的方向
	local dragDirection=f_Sprite_DragDirection(scrollbtn)
	
	--背景sprite引用
	--local sprite = func_find_obj(bgName)--func_getTable(bgName)
	
	--背景局部坐标
	local local_x,local_y = get_attr(sprite,"spriteLocalXY")
	
	--进度条的坐标
	local bx,by = get_attr(sprite,"spritePos")
	
	--进度条的宽高
	local sprite_w,sprite_h = get_attr(sprite,"spriteSize")
	
	--小按钮引用
	--local scrollbtn = func_find_obj(littleName)
	
	--小按钮的宽高
	local sc_w,sc_h = get_attr(scrollbtn,"spriteSize")
		
	--local cur_x = --GetBarX(x,sc_w,w)
	
	
	if(dragDirection == CONST_DIRECTION_HORIZONTAL) then
		local v = local_x
		
		local target_x = bx + v 
		
		--print("设置小按钮的位置:"..tostring(target_x)..","..tostring(by)..",sc_w="..sc_w..',sc_h='..sc_h..',sprite_w='..sprite_w..',sprite_h='..sprite_h)

		if (target_x > sprite_w  - sc_w + bx) then
			target_x = sprite_w - sc_w + bx;
			--print('纠正!!!')
		end
		--sprint(target_x,by);
		sprite_setpos(scrollbtn,target_x,by);
		--print("***",target_x,by);
		return v / sprite_w
	else

		local v = local_y
		
		local target_y = by + v 
		
		--print("设置小按钮的位置:"..tostring(target_x)..","..tostring(by)..",sc_w="..sc_w..',sc_h='..sc_h..',sprite_w='..sprite_w..',sprite_h='..sprite_h)

		if (target_y > sprite_h  - sc_h + by) then
			target_y = sprite_h - sc_h + by
			--print('纠正!!!')
		end
		
		sprite_setpos(scrollbtn,bx,target_y);
		--print("##",bx,target_y);
		return v / sprite_h
	end
end
--[[
	点击滑动条上的小按钮
--]]
local function 
f_scrollBarBG_Click2(name,p)
	local sc = p.sc;--func_getTable(scName)

	
	--print(name,p);
	--大背景
	local sprite = sc.bg;--func_find_obj(name)
	--print('########### '..name)
	--print(sprite)
	
	local x,y = get_attr(sprite,"spriteLocalXY")
	local w,h = get_attr(sprite,"spriteSize")
	
	
	--小按钮
	--local scName=string.sub(name,0,-string.len("_bg")-1)
	
	--拖拽
	local v =  f_SetBarPostion(sprite,sc.btn);
	f_ScDragMove(sc,v)
end;

local function f_f_LuaDrag_move(data,p)
	local sc = p;
	
	
	local arr = func_split(data,",");
	
	--local name = arr[1];
	local progress = tonumber(arr[4]);
	--arr = nil
	--f_ScDragMove(func_getTable(name),progress);
	--print(progress);	
	
	f_ScDragMove(sc,progress);
end

--设置滑动条的文本
function scrollBar_label(sc,label)
   --[[ if(sc.tf ==nil) then
        sc.tf = ftext_create();
		func_addchild(sc.bg,ftext_get_container(sc.tf));
    end
    ftext_reset(sc.tf,label);
    local x,y = get_attr(sc.bg,"spritePos");
    local w,h = get_attr(sc.bg,"spriteSize")
    ftext_setpos(sc.tf,x+w,y);--]]
end



--[[--重置label的坐标
local function f_reset_label_pos(btn)
    local w,h = ftext_getsize(btn.label);
    local x,y = func_get_sprite_xy(btn.sprite);--get_attr(btn.sprite,"spritePos")
    local sw,sh=func_get_sprite_size(btn.sprite);
    ftext_setpos(btn.label,x+(sw-w)/2,y+(sh-h)/2);
end--]]

--[[function scrollBar_pos(sc,x,y)
	func_setPos(sc.bg,x,y)
	
	if(sc.tf) then
--		func_setPos(btn.label,x,y)
--        ftext_setpos(btn.label,x,y);
    --     f_reset_label_pos(btn);
	end
end--]]

--根据cw,ch的值来确定是横向e还是综向滑动
function 
scrollBar_new(x,y,parent,cw,ch)
	cw = cw or 100;
	ch = ch or 15;

	x = x or 0;
	y = y or 0;
	
	local _dragDirection;
	local sc = f_create()
	
	sc.defaultBg_width = cw;
	sc.defaultBg_height= ch;
	local barSize;
	if(cw > ch) then
		_dragDirection=CONST_DIRECTION_HORIZONTAL;
		barSize = ch;
	else
		_dragDirection=CONST_DIRECTION_VERTICAL;
		barSize = cw;
	end

	--func_tableSave(sc);
	
	--local name = func_getTableName(sc)--获取btn引用名

	--背景
	--local bg = sprite_create(string.format("%s%s",name,"_bg"),x,y,sc.defaultBg_width,sc.defaultBg_height)
	--func_setIcon(bg,"gundi.png");
	--string.format("%s%s",name,"_bg")
	
	local bg = sprite_create_typical(nil,x,y,sc.defaultBg_width,sc.defaultBg_height);
    local url = "checkbox.png";
	engine_addNode(bg);
	loadtexs(url,func_texloadend, { sprite=bg;url=url});
	
	--创建小按钮
	--local btn=sprite_create(name,x,y,barSize,barSize,_dragDirection);
	--func_setIcon(btn,"smallbtn.png");
	url = "checkbox.png";--"gundi.png";
	local btn = sprite_create_typical(nil,x,y,barSize,barSize);
	engine_addNode(btn);
	--print(bg,btn);
	func_addchild(bg,btn);

	loadtexs(url,func_texloadend, { sprite=btn;url=url});
	sprite_set_direction(btn,_dragDirection);
	
	--对sc赋值
	sc.bg = bg
	sc.btn = btn

	evt_on(bg,EVENT_ENGINE_SPRITE_CLICK,f_scrollBarBG_Click2,{sc=sc});

	evt_on(btn,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_f_LuaDrag_move,sc);
	
	
	

	--设置可拖拽范围
	sprite_setDragScope(btn,0,0,sc.defaultBg_width,sc.defaultBg_height);
	if(parent)then
		func_addchild(parent,bg,x,y);
	end
	return sc;
end

function scrollBar_get_rect(sc)
    local x,y = get_attr(sc.bg,"spritePos");
	local w,h = get_attr(sc.bg,"spriteSize");
    return x,y,w,h;
end

--删除
function scrollBar_del(sc)
	
	evt_off(sc.bg,EVENT_ENGINE_SPRITE_CLICK,f_scrollBarBG_Click2);
	evt_off(sc.btn,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_f_LuaDrag_move);
	
	--[[
    if(sc.tf) then
        fext_dispose(sc.tf);
		func_sprite_removechild(sc.bg,ftext_get_container(sc.tf));
    end
	--]]
	--func_sprite_removechild(sc.bg,sc.btn);
	
	sprite_removeSelf(sc.bg);
	sprite_removeSelf(sc.btn);
	
	ptr_remove(sc.bg)
	ptr_remove(sc.btn)
	--local parent = sc.parent;
	--if(parent) then
	--	func_sprite_removechild(parent,sc.bg);
		--print("移除sc");
	--end
	--func_tableDel(sc)	
end

--绑定一个回调函数
function 
scrollBar_bind(sc,call,param)
	sc.callBack = call;
	sc.param = param;
end

--获取一个参数
function 
scrollBar_get_param(sc)
	return sc.param;
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

--[[
local sc = scrollBar_new(100,100);
local function f_animscHandle(sc)
	--func_rotate(crl.o, key, sc.value);
	print("****",sc.value);
end
scrollBar_bind(sc, f_animscHandle);
scrollBar_label(sc,"a");
--scrollBar_del(sc);
--]]