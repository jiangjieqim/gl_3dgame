--一个弹出框
local win ={
	bg;
	closeBtn;
	label;
	visible = false;
}

--重置界面内的子节点坐标
local function f_resize_window_child(window)
	local sx,sy = func_get_sprite_xy(window.bg);
	func_setPos(window.label,sx,sy)
	btn_pos(window.closeBtn,sx+270,sy);
end

--print(win,win.bg);
local w = 300;--窗口宽度
local h = 100;--窗口高度
local x = 100;
local y = 100;
--local btnx = x + 270;
win.bg = sprite_create("alert",x,y,w,h);
win.closeBtn=btn_create(x,y,30,30);
win.label=tf_create(128,x,y);


local function f_LuaDrag_move(data)
	local arr = func_split(data,",");
	--print(arr[2])
	local name = arr[1];
	--local progress = tonumber(arr[4]);
	
	--print("****^"..arr[2],arr[3]);
	f_resize_window_child(win);
	arr = nil
	--f_ScDragMove(func_getTable(name),progress)
end


evt_on(win.bg,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_LuaDrag_move);

--func_setPos(win.bg,0,0);

--change_attr(win.bg,"sprite_position","0,0")
--local t = func_get_type(win.bg)


local function f_hide_pos()
	local y = -1024
	func_setPos(win.bg,0,y)
	func_setPos(win.label,0,y)
	btn_pos(win.closeBtn,0,y);
end



--居中
local function f_center()
	local sx,sy = func_screenSize();
	x = (sx - w)/2;
	y = (sy - h)/2;--居中
	
	func_setPos(win.bg,x,y)
	
	f_resize_window_child(win);
end

local function f_hide()
	btn_visible(win.closeBtn,false);
	resetv(win.bg,FLAGS_VISIBLE);
	resetv(win.label,FLAGS_VISIBLE);
end

local function f_onCloseHandle(btn)
	f_hide()
	--print(btn);
end
local function f_resize()
	
	if(win.visible) then
		f_center();
	end
	
	local sw,sh = func_screenSize();
	--print("screenSize = ",sw,sh);
	local sx,sy = func_get_sprite_xy(win.bg);
	
	if(sw > w and sh > h) then
		--当窗口尺寸>当前win.bg尺寸的时候才能拖动
		sprite_setDragScope(win.bg,-sx,-sy,sw-sx,sh-sy);
	end
end




function alert(str)
	win.visible = true;
	--print(string.format("alert_show = %s",str));
	btn_visible(win.closeBtn,true);
	setv(win.bg,FLAGS_VISIBLE);
	setv(win.label,FLAGS_VISIBLE);
	tf_setText(win.label,str);
	f_center();
	f_resize();
end
--显示一个弹出框
--alert_show("");
--f_hide();

btn_bindClick(win.closeBtn,f_onCloseHandle);
evt_on(win,EVENT_ENGINE_RESIZE,f_resize);
f_hide_pos();


--evt_off(win,EVENT_ENGINE_RESIZE,f_resize);

