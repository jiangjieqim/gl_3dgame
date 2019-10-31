--EVENT_BUTTON_CLICK = 10000;--点击事件


--#######################################################
--	按钮组件
--
--
--#######################################################
--创建一个按钮结构体
local function f_createBtn()
	return {
		--对象类型
		type = "Button",
		--引擎中的Sprite引用
		sprite=nil,
		
		--点击回调函数
		clickEvtCallBack=nil,
	
		--鼠标按下去回调
		mouseDownCallBack = nil,
	 
		--textField
		label=nil,
		
		--id = 0,	--按钮id
		
		data = nil,
		
	};
end
local function f_delLabel(btn)
	if(btn.label) then
		--ptr_remove(btn.label);
        fext_dispose(btn.label);
		btn.label = nil
	end
end

local function f_loadend(n)
	--print("f_loadend  "..n.url);
	sprite_set_9grid(n.sprite,n.url,3,3,3,3);
end

--[[
	创建一个按钮
	x,y默认值为0,0
--]]
function btn_create(x,y,w,h,url)
	local btn = f_createBtn()
	--local w = 100
	--local h = 100
	--btn.id = 123
	
	--保存按钮表引用
	func_tableSave(btn)

	--func_printTable(btn)
	
	local name = func_getTableName(btn)--获取btn引用名
	
	---------------------------------------
	--默认参数设置
	if(url == nil ) then url = "smallbtn.png" end
	if(x == nil) then x = 0	end
	if(y == nil) then y = 0 	end
	if(w == nil) then w = 80	end
	if(h == nil) then h = 18	end
	---------------------------------------

	local sprite = sprite_create_typical(name,x,y,w,h);
	engine_addNode(sprite);
	loadtexs(url,f_loadend, { sprite=sprite;url=url});
    	
	
	btn.sprite =  sprite;
	btn_scaleStyle(btn);
	
	return btn
end
--获取btn的位置及其尺寸
function btn_get_rect(btn)
    local x,y = get_attr(btn.sprite,"spritePos");
	local w,h = get_attr(btn.sprite,"spriteSize");
    return x,y,w,h;
end
--获取按钮的容器(Sprite)
function btn_get_container(btn)
	return btn.sprite;
end

--[[
	为按钮绑定一个点击回调
	clickEvt(point)		point 为btn引用
--]]
function btn_bindClick(btn,clickEvt)
	btn.clickEvtCallBack = clickEvt;
	evt_on(btn.sprite,EVENT_ENGINE_SPRITE_CLICK,clickEvt);
end

--[[
	为按钮绑定一个点击回调
	clickEvt(point)		point 为btn引用
--]]
function btn_bindDown(btn,c)
	btn.mouseDownCallBack = c;
	evt_on(btn.sprite,EVENT_ENGINE_SPRITE_CLICK_DOWN,c);
end
--移除事件
local function f_remove_evt(btn)
	evt_off(btn.sprite,EVENT_ENGINE_SPRITE_CLICK,btn.clickEvtCallBack);
	evt_off(btn.sprite,EVENT_ENGINE_SPRITE_CLICK_DOWN,btn.mouseDownCallBack);
end
--[[
	销毁按钮对象
--]]
function btn_dispose(btn)
	
	f_delLabel(btn)
	
	ptr_remove(btn.sprite)
	btn.sprite = nil
	
	btn.mouseDownCallBack = nil
	--btn.clickEvtCallBack = nil
	f_remove_evt(btn);


	func_tableDel(btn)
	
end
--[[
	设置按钮按下的缩放状态
--]]
function btn_scaleStyle(btn)
	setv(btn.sprite,FLAGS_BUTTON_EFFECT);
end
--重置label的坐标
local function f_reset_label_pos(btn)
    local w,h = ftext_getsize(btn.label);
    local x,y = func_get_sprite_xy(btn.sprite);--get_attr(btn.sprite,"spritePos")
    local sw,sh=func_get_sprite_size(btn.sprite);
    ftext_setpos(btn.label,x+(sw-w)/2,y+(sh-h)/2);

end
--[[
	设置按钮label
--]]
function btn_label(btn,str,r,g,b)
	if(str == nil) then
		
		--删除文本
		f_delLabel(btn)
	else
	
		if(btn.label == nil) then
			if(r == nil) then r = 1.0	end
			if(g == nil) then g = 1.0	end
			if(b == nil) then b = 1.0	end
			
--			local x,y = get_attr(btn.sprite,"spritePos")

--			btn.label=tf_create(128,x,y,r,g,b);
            btn.label = ftext_create();
--            ftext_setpos(btn.label,x,y);
		end
--		tf_setText(btn.label,str);
        ftext_reset(btn.label,str);
        
        f_reset_label_pos(btn);
	end
end
--设置btn的name
function btn_set_data(btn,data)
	btn.data = data
end

function btn_get_data(btn)
	return btn.data
end

--[[
	设置按钮隐藏显示
--]]
function btn_visible(btn,vis)
	local f = FLAGS_VISIBLE
	if(vis) then
		if(btn.label) then
--			setv(btn.label,f);
            ftext_vis(btn.label,1);
		end
		setv(btn.sprite,f);
	else
		if(btn.label) then
--			resetv(btn.label,f);
            ftext_vis(btn.label,0);
		end
		resetv(btn.sprite,f);
	end
end

--[[
	设置按钮坐标坐标
--]]
function btn_pos(btn,x,y)
	func_setPos(btn.sprite,x,y)
	
	if(btn.label) then
--		func_setPos(btn.label,x,y)
--        ftext_setpos(btn.label,x,y);
         f_reset_label_pos(btn);
	end
end
--按钮设置为线框渲染模式
--function btn_set_line(btn)
--    setv( btn.sprite,FLAGS_DRAW_PLOYGON_LINE);
--end
---------------------------------------------------
