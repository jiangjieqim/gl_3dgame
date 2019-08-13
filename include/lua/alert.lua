local alert1;

local closeSize = 30;

--重置子节点坐标
local function f_resize_window_child(self)
	local sx,sy = func_get_sprite_xy(self.bg);
	local bgw,bgh = func_get_sprite_size(self.bg);
	--func_setPos(self.label,sx,sy);

    --设置文本的坐标
	func_ftext_setpos(self.label,sx,sy);

    --设置关闭按钮的坐标
	btn_pos(self.closeBtn,sx+(bgw-closeSize),sy);
end;
local function f_drag(evtData,self)
	--拖拽移动事件
--	local arr = func_split(data,",");
	--print(arr[2])
--	local name = arr[1];
	--local progress = tonumber(arr[4]);
	--print("****^"..arr[2],arr[3]);
--	arr = nil;

--    print(self);
	f_resize_window_child(self);	
end

--居中
local function f_center(self)
	local x,y;
	local sx,sy = func_screenSize();
	local bgw,bgh = func_get_sprite_size(self.bg);
	x = (sx - bgw)/2;
	y = (sy - bgh)/2;
	func_setPos(self.bg,x,y);--设置关闭按钮的坐标
	f_resize_window_child(self);
--    print("x y = ",x,y);
end
local function f_resize(evtData,self)
	local sw,sh = func_screenSize();
	
	local sx,sy = func_get_sprite_xy(self.bg);
	local bgw,bgh = func_get_sprite_size(self.bg)

	--print("screenSize = ",sw,sh,self);
	--print(self.isDrag)
	if(self.isDrag and sw > bgw and sh > bgh) then
		sprite_setDragScope(self.bg,-sx,-sy,sw-sx,sh-sy);
	end
	f_center(self);
end;

local function f_hide(self)
	resetv(self.bg,FLAGS_VISIBLE);
--	resetv(self.label,FLAGS_VISIBLE);
    func_ftext_vis(self.label,0);
	btn_visible(self.closeBtn,false);
end

local function set_drag(self,value)
	self.isDrag = value
	if(value) then
		evt_on(self.bg,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_drag,self);	
	else
		evt_off(self.bg,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_drag);		
	end
end

local function f_init(self,w,h)
	local x = 0;
	local y = 0;
	
	self.bg = sprite_create("alert",x,y,w,h);
	func_setIcon(self.bg,"gundi.png");
	--print(self.closeBtn)
		
	self.closeBtn=btn_create(x,y,closeSize,closeSize);
	--print(self)
		
	set_drag(self,true);
		
	btn_bindClick(self.closeBtn,
			function(b)
			--print(b,self)
				--self.f_hides();
				--print(1);
				--print(self,self.bg,self.label,self.closeBtn);
				--[[
				resetv(self.bg,FLAGS_VISIBLE);
				resetv(self.label,FLAGS_VISIBLE);
				btn_visible(self.closeBtn,false);
				--]]
				f_hide(self);
			end
	);
		
	self.label=func_ftext_create(w-closeSize,w-closeSize);--tf_create(128,x,y,1.0,0.0,0.0);
    func_ftext_set_buffer(self.label,256);
end;

local function show(self,str)
	--self.visible = true;
	btn_visible(self.closeBtn,true);
	setv(self.bg,FLAGS_VISIBLE);

--	setv(self.label,FLAGS_VISIBLE);
--	tf_setText(self.label,str);
    func_ftext_vis(self.label,1);

	func_ftext_reset(self.label,str);
	f_resize(nil,self);
end

local function create()
    local new_sharp = { 
	    bg;
	    closeBtn;
	    label;
	    isDrag;--是否可以拖拽
    };
    return new_sharp;
end

function alert(str)
	str = str or "";
	if(alert1 == nil) then
		alert1 = create();
		f_init(alert1,300,100);
		evt_on(alert1,EVENT_ENGINE_RESIZE,f_resize,alert1);	
		--evt_on(alert1.bg,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_drag);		
	end
	--print(evt_has(alert1.bg,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_drag));
	
	--self:set_drag(false)
	show(alert1,str);
end