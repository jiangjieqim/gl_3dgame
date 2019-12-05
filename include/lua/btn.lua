--EVENT_BUTTON_CLICK = 10000;--点击事件


--#######################################################
--	按钮组件
--
--
--#######################################################
--创建一个按钮结构体
local function f_createBtn()
	return {
		name = nil,
		--对象类型
		type = 2,
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
       -- fext_dispose(btn.label);
		--btn.label = nil
		func_dispose(btn.label);
	end
end

--[[
	创建一个按钮
	x,y默认值为0,0
	use9grid: 默认使用九宫格数据
	useeffect:是否使用按钮缩放特效,默认是使用(true)
--]]
function btn_create(x,y,w,h,url,use9grid,useeffect)
	local btn = f_createBtn();
	
	useeffect = useeffect or true;
	--local w = 100
	--local h = 100
	--btn.id = 123

	--func_printTable(btn)
	
	--local name = func_getTableName(btn)--获取btn引用名
	
	---------------------------------------
	--默认参数设置
	
	url = url or "smallbtn.png";
	x = x or 0;
	y = y or 0;
	w = w or 80;
	h = h or 18;
	---------------------------------------

	local sprite = sprite_create_typical(nil,x,y,w,h);
	engine_addNode(sprite);
	
	use9grid = use9grid or 1;--是否使用9宫格
	if(use9grid == 1) then
		loadtexs(url,func_texloadend, { sprite=sprite;url=url});
	else
		func_setIcon(sprite,url);
	end
	
	btn.sprite =  sprite;
	if(useeffect) then
		btn_scaleStyle(btn);
	end
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
function btn_bindClick(btn,clickEvt,param)
	btn.clickEvtCallBack = clickEvt;
	evt_on(btn.sprite,EVENT_ENGINE_SPRITE_CLICK,clickEvt,param);
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
	
	f_remove_evt(btn);
		
end
--[[
	设置按钮按下的缩放状态
--]]
function btn_scaleStyle(btn)
	setv(btn.sprite,FLAGS_BUTTON_EFFECT);
end
--重置label的坐标
local function f_reset_label_pos(btn,style)
    local w,h = label_get_wordpos(btn.label);--label_get_size(btn.label);
	
    local x,y = func_get_sprite_xy(btn.sprite);
    local sw,sh=func_get_sprite_size(btn.sprite);
	local y = (sh-h)/2;
	if(style==nil) then
		label_set_pos(btn.label,(sw-w)/2,y);
	elseif(style == "rightout")then	
		label_set_pos(btn.label,sw,0);
	end
end
	
--[[
	设置按钮label
	
	style:checkbox的对齐方式的样式,
	
		nil:默认居中对齐方式
		rightout:右边框对齐
--]]
function btn_label(btn,str,style,r,g,b)
	--center = center or true;
	
	if(str == nil) then
		
		--删除文本
		f_delLabel(btn)
	else
	
		if(btn.label == nil) then
			if(r == nil) then r = 1.0	end
			if(g == nil) then g = 1.0	end
			if(b == nil) then b = 1.0	end
			
            btn.label = label_create();
			func_addnode(btn.sprite,btn.label);
		end
		-- ftext_reset(btn.label,str);
		label_set_text(btn.label,str);
		
        f_reset_label_pos(btn,style);
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
            --ftext_vis(btn.label,1);
			label_set_visible(btn.label,true);
		end
		setv(btn.sprite,f);
	else
		if(btn.label) then
--			resetv(btn.label,f);
            --ftext_vis(btn.label,0);
			label_set_visible(btn.label,false);
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

function example_btn()
	local function btnClick(btnName,p)
		print(btnName,p);--p = abc
	end
	local b = btn_create();
	btn_bindClick(b,btnClick,"abc");
end

