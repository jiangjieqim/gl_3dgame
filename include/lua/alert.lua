function alert_init()
    local new_sharp = { 
	    bg;--sprite
	    closeBtn;--btn
	    label;
	    isDrag;--是否可以拖拽
        closeSize = 30;

        --img;
    };
    return new_sharp;
end

local function f_drag(evtData,self)
	--拖拽移动事件
end

--居中
local function f_center(self)
	local x,y;
	local sx,sy = func_screenSize();
	local bgw,bgh = func_get_sprite_size(self.bg);
	x = (sx - bgw)/2;
	y = (sy - bgh)/2;
	func_setPos(self.bg,x,y);--设置关闭按钮的坐标
end
local function f_resize(evtData,self)
	local sw,sh = func_screenSize();
	
	local sx,sy = func_get_sprite_xy(self.bg);
	local bgw,bgh = func_get_sprite_size(self.bg)

	if(self.isDrag and sw > bgw and sh > bgh) then
		sprite_setDragScope(self.bg,-sx,-sy,sw-sx,sh-sy);
	end
	f_center(self);
end;

local function f_hide(self)
	resetv(self.bg,FLAGS_VISIBLE);
	--[[if(self.img) then
		resetv(self.img,FLAGS_VISIBLE);
	end--]]
    ftext_vis(self.label,0);
	btn_visible(self.closeBtn,false);
end
--设置可拖拽
function alert_set_drag(self,value)
	self.isDrag = value;
	if(value) then
		evt_on(self.bg,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_drag,self);--增加拖拽事件
	else
		evt_off(self.bg,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_drag);		
	end
end

--[[local function addFbo(self)
	local spr = engine_get_fbo_sprite();
    node_fbo();
    func_addchild(self.bg,spr);
    engine_addNode(spr);
    self.img = spr;
end--]]


local function f_closeCallBack(data,param)
	f_hide(param);
end

local function f_alert_create(self,w,h)
	
	local x = 0;
	local y = 0;
	
    --local self = create();

	--self.bg = sprite_create(nil,x,y,w,h);
	--func_setIcon(self.bg,"gundi.png");
	
--[[	local sprite = sprite_create_typical(name,x,y,w,h);
	sprite_set_9grid(sprite,"checkbox.png",3,3,3,3);
	engine_addNode(sprite);--]]
	
	local sprite=func_create_grid_sprite(x,y,w,h,"checkbox.png",name);
	self.bg = sprite;
	
	--print(self.closeBtn)

    --##############################################
    --FBO渲染的Sprite
--    addFbo(self);
    --##############################################



	local closeSize = self.closeSize;
	self.closeBtn=btn_create(x,y,closeSize,closeSize,"checkbox.png");
	--func_sprite_removechild(self.bg,self.closeBtn.sprite);
	
	alert_set_drag(self,true);
		
	btn_bindClick(self.closeBtn,f_closeCallBack,self);
	
	self.label=ftext_create(w-closeSize,w-closeSize);--tf_create(128,x,y,1.0,0.0,0.0);
    ftext_set_buffer(self.label,256);

	func_addchild(self.bg,btn_get_container(self.closeBtn),w-self.closeSize,0);
	func_addchild(self.bg,ftext_get_container(self.label));
    
    

--    func_sprite_removechild(self.bg,ftext_get_container(self.label));

    evt_on(self,EVENT_ENGINE_RESIZE,f_resize,self);	
	
    return self;
end
--销毁视图
function alert_dispose(self)
	ptr_remove(self.bg);
	fext_dispose(self.label);
	btn_dispose(self.closeBtn);
end

local function show(self,str)
	btn_visible(self.closeBtn,true);
	setv(self.bg,FLAGS_VISIBLE);
--[[	if(self.img) then
		setv(self.img,FLAGS_VISIBLE);
	end--]]
    ftext_vis(self.label,1);

	ftext_reset(self.label,str);
	f_resize(nil,self);
end

local function f_tex_complete(n)
	local str = n.str or "";
	local self = n.self;
	f_alert_create(self,300,150);
	show(self,str);
	--[[if(n.callBack) then
		n.callBack(alert1);
	end--]]
	
	local obj = func_get_address(self);
	evt_dispatch(obj,EVENT_ENGINE_COMPLETE,obj);
end

function alert_start(self,str)	
	local url = "checkbox.png";
	loadtexs(url,f_tex_complete, {str=str,self=self});
end