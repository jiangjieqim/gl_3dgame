--一个弹出框
local win ={
	bg;
	closeBtn;
	label;
	visible = false;
}

--print(win,win.bg);
local w = 300;
local h = 100;
local x = 100;
local y = 100;
local btnx = x + 270;
win.bg = sprite_create("alert",x,y,300,100);
win.closeBtn=btn_create(btnx,y,30,30);
win.label=tf_create(128,x,y);

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
	btnx = x + 270;
	func_setPos(win.bg,x,y)
	func_setPos(win.label,x,y)
	btn_pos(win.closeBtn,btnx,y);
	
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
end

function alert(str)
	win.visible = true;
	--print(string.format("alert_show = %s",str));
	btn_visible(win.closeBtn,true);
	setv(win.bg,FLAGS_VISIBLE);
	setv(win.label,FLAGS_VISIBLE);
	tf_setText(win.label,str);
	f_center()
end
--显示一个弹出框
--alert_show("");
--f_hide();

btn_bindClick(win.closeBtn,f_onCloseHandle);
evt_on(win,EVENT_ENGINE_RESIZE,f_resize);
f_hide_pos();


evt_off(win,EVENT_ENGINE_RESIZE,f_resize);

