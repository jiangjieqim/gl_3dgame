Alert = {}


--重置子节点坐标
function Alert:f_resize_window_child()
	local sx,sy = func_get_sprite_xy(self.bg);
	local bgw,bgh = func_get_sprite_size(self.bg)

	func_setPos(self.label,sx,sy);
	
	--print(self.closeBtn,sx+(bgw-30),sy);
	btn_pos(self.closeBtn,sx+(bgw-30),sy);
end;

--居中
function Alert:f_center()
	
	local sx,sy = func_screenSize();

	local bgw,bgh = func_get_sprite_size(self.bg)
	
	local x = (sx - bgw)/2;
	local y = (sy - bgh)/2;

	func_setPos(self.bg,x,y)
	
	self:f_resize_window_child();
end;
	--拖拽移动事件

function Alert:create()

	local new_sharp = { 
			bg;
			closeBtn;
			label;
			
	--###########################################################
	f_resize	= function ()
		local sw,sh = func_screenSize();
		self:f_center();
		
		
		local sx,sy = func_get_sprite_xy(self.bg);
		local bgw,bgh = func_get_sprite_size(self.bg)

		--print("screenSize = ",sw,sh,self);
		if(sw > bgw and sh > bgh) then
			sprite_setDragScope(self.bg,-sx,-sy,sw-sx,sh-sy);
		end
	end;
	--###########################################################
	f_init = function(w,h)
		local x = 0;
		local y = 0;
		
		self.bg = sprite_create("alert",x,y,w,h);
		--print(self.closeBtn)
		self.closeBtn=btn_create(x,y,30,30);
		--print(self)

		btn_bindClick(self.closeBtn,
				function(b)
				--print(b,self)
					--self.f_hides();
					--print(1);
					--print(self,self.bg,self.label,self.closeBtn);
					
					resetv(self.bg,FLAGS_VISIBLE);
					resetv(self.label,FLAGS_VISIBLE);
					btn_visible(self.closeBtn,false);
				end
		);
		
		evt_on(self.bg,EVENT_ENGINE_SPRITE_CLICK_MOVE,
				function(data)
				
					local arr = func_split(data,",");
					--print(arr[2])
					local name = arr[1];
					--local progress = tonumber(arr[4]);
					--print("****^"..arr[2],arr[3]);
					arr = nil;
					
					self:f_resize_window_child();	
				end
		
		);	
		self.label=tf_create(128,x,y);
	end;
	
	} ;
	
	--#################################################
	
	
	
	self.__index = self;
    setmetatable(new_sharp, self);
	
	return new_sharp;
end

function Alert:show(str)
	--self.visible = true;
	btn_visible(self.closeBtn,true);
	setv(self.bg,FLAGS_VISIBLE);
	setv(self.label,FLAGS_VISIBLE);
	tf_setText(self.label,str);
	
	self.f_resize();
end

local alert1;

function alert(str)
	if(alert1 == nil) then
		alert1 = Alert:create();
		alert1.f_init(300,100);
		evt_on(alert1,EVENT_ENGINE_RESIZE,alert1.f_resize);		
	end

	alert1:show(str);
end