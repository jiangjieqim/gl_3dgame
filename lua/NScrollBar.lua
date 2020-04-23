local DIRECTION_HORIZONTAL = 0	--水平,横 
local DIRECTION_VERTICAL   = 1 	--垂直,竖

NScrollBar = {
	
	bg,--可滚动的背景(Image)
	btn,--滚动交互小按钮
	
	--回调及回调参数
	callBack,
	callBackParam,
};

NScrollBar.__index = NScrollBar;
setmetatable(NScrollBar, Base);

local function 
f_setBarPostion(sprite,scrollbtn)
	--滑动的方向
	local dragDirection=scrollbtn:get_drag_type();
	--背景局部坐标
	local local_x,local_y = sprite:local_mouse_xy();
	
	--进度条的坐标
	local bx,by = 0,0;--sprite:get_pos();

	--进度条的宽高
	local sprite_w,sprite_h = sprite:get_size();

	--小按钮的宽高
	local sc_w,sc_h = scrollbtn:get_size();
				
	if(dragDirection == DIRECTION_HORIZONTAL) then
		local v = local_x;
		local target_x = bx + v;
		--print("设置小按钮的位置:"..tostring(target_x)..","..tostring(by)..",sc_w="..sc_w..',sc_h='..sc_h..',sprite_w='..sprite_w..',sprite_h='..sprite_h)
		if (target_x > sprite_w  - sc_w + bx) then
			target_x = sprite_w - sc_w + bx;
			--print('纠正!!!')
		end

		--sprite_setpos(scrollbtn,target_x,by);
		scrollbtn:set_pos(target_x,by);
		
		--print("设置坐标***",target_x,by);
		return v / sprite_w;
	else
		local v = local_y;
		local target_y = by + v ;
		--print("设置小按钮的位置:"..tostring(target_x)..","..tostring(by)..",sc_w="..sc_w..',sc_h='..sc_h..',sprite_w='..sprite_w..',sprite_h='..sprite_h)

		if (target_y > sprite_h  - sc_h + by) then
			target_y = sprite_h - sc_h + by;
			--print('纠正!!!')
		end
		
--		sprite_setpos(scrollbtn,bx,target_y);
		scrollbtn:set_pos(bx,target_y);
		
		--print("##",bx,target_y);
		return v / sprite_h;
	end
	
--]]
end

--当滚动游标移动的时候触发
local function f_luaDrag_move(progress,self)    
	
	if(self.callBack) then
		--print(progress,self);
		self.callBack(progress,self.callBackParam);
	end
end

local function f_scrollBarClick(name,self)
	local v = f_setBarPostion(self.bg,self.btn);
	f_luaDrag_move(v,self);
end

function NScrollBar:bindCallback(callBack,callBackParam)
	self.callBack = callBack;
	self.callBackParam = callBackParam;
end
function NScrollBar:visible(v)
	local bg = self.bg;
	local btn = self.btn;
	bg:visible(v);
	btn:visible(v);
end
function NScrollBar:new(x,y,cw,ch)
	local self = Base:new();
	self:settype(12);

	setmetatable(self, NScrollBar);
	
	--设置默认的宽高
	cw = cw or 100;
	ch = ch or 15;
	
	local _dragDirection;	--滚动的方向
	local barSize;			--滚动块的尺寸
	if(cw > ch) then
		_dragDirection=DIRECTION_HORIZONTAL;
		barSize = ch;
	else
		_dragDirection=DIRECTION_VERTICAL;
		barSize = cw;
	end
	
	--创建可滑动的背景
	local bg = Image:new(cw,ch);
	bg:mouseEnable(true);
	bg:set_pos(x or 0,y or 0);
	bg:seticon("checkbox.png");
	self.bg = bg;	
	--创建小按钮
	local btn = Shape:new(true,barSize,barSize);
	--local btn = Image:new(barSize,barSize); 
	--btn:seticon("gundi.png");
	
	btn:mouseEnable(true);
	btn:setcolor(0,1,0);
	btn:set_drag_type(_dragDirection);
	--设置可拖拽范围
	btn:set_drag_rect(0,0,cw,ch);
	
	self.btn = btn;
	
	bg:addChild(btn:get_container());
	
	--bg:drawPloygonLine(true);
	
	bg:on(EVENT_ENGINE_SPRITE_CLICK,f_scrollBarClick,self);
	btn:on(EVENT_ENGINE_SPRITE_CLICK_MOVE,f_luaDrag_move,self);
	
	--self:set_pos(0,0);
	return self;
end

function NScrollBar:get_container()
	local bg = self.bg;
	return bg:get_container();
end

function NScrollBar:dispose()
	local bg = self.bg;
	local btn = self.btn;
	bg:off(EVENT_ENGINE_SPRITE_CLICK,f_scrollBarClick);
	bg:dispose();
	
	btn:off(EVENT_ENGINE_SPRITE_CLICK_MOVE,f_luaDrag_move);
	btn:dispose();
	
	func_clearTableItem(self);
end
