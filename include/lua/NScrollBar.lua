local DIRECTION_HORIZONTAL = 0	--水平,横 
local DIRECTION_VERTICAL   = 1 	--垂直,竖

NScrollBar = {
	name = nil,
	type = 12,
	
	bg,--可滚动的背景(Image)
	btn,--滚动交互小按钮
	
	--回调及回调参数
	callBack,
	callBackParam,
};

NScrollBar.__index = NScrollBar;

local function f_scrollBarBG_Click2(name,p)
	print(name,p);
end
--当滚动游标移动的时候触发
local function f_f_LuaDrag_move(progress,self)    
	--print(data,p);
	--print(data);;
	if(self.callBack) then
		self.callBack(progress,self.callBackParam);
	end
end

function NScrollBar:bindCallback(callBack,callBackParam)
	self.callBack = callBack;
	self.callBackParam = callBackParam;
end

function NScrollBar:new(cw,ch)
	local self = {};
	setmetatable(self, NScrollBar);
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
	bg:seticon("checkbox.png");
	self.bg = bg;	
	--创建小按钮
	local btn = Shape:new(true,barSize,barSize);
	--local btn = Image:new(barSize,barSize); 
	--btn:seticon("gundi.png");
	
	btn:mouseEnable(true);
	btn:setcolor(0,1,0);
	btn:set_drag_direct(_dragDirection);
	--设置可拖拽范围
	--btn:set_drag_rect(0,0,cw,ch);
	
	self.btn = btn;
	
	bg:addChild(btn:get_container());
	
	--bg:drawPloygonLine(true);
	
	--bg:on(EVENT_ENGINE_SPRITE_CLICK,f_scrollBarBG_Click2,self);
	btn:on(EVENT_ENGINE_SPRITE_CLICK_MOVE,f_f_LuaDrag_move,self);
	
	self:set_pos(0,0);
	return self;
end

--设置坐标
function NScrollBar:set_pos(x,y)
	local bg = self.bg;
	local btn = self.btn;
	bg:set_pos(x,y);
	local cw,ch = bg:get_size();
	btn:set_drag_rect(0,0,cw,ch);
end
function NScrollBar:get_container()
	local bg = self.bg;
	return bg:get_container();
end

function NScrollBar:dispose()
	local bg = self.bg;
	local btn = self.btn;
	bg:off(EVENT_ENGINE_SPRITE_CLICK,f_scrollBarBG_Click2);
	bg:dispose();
	
	btn:dispose();
	btn:off(EVENT_ENGINE_SPRITE_CLICK_MOVE,f_f_LuaDrag_move);
	
	func_clearTableItem(self);
end
