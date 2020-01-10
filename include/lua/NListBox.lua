--下拉列表组件
NListBox = {
	bg,--Shape
	tf,--NLabel
	h,--缩放时候的高度
	
	data,--tabel列表中的数据

	index,--当前的索引

	labelstr,--上一次的label值
	
	callBack,
	param,
};

NListBox.__index = NListBox;
setmetatable(NListBox, Base);

--获取列表的长度
function NListBox:get_cnt()
	local list = self;
	local cnt = 0;
	if(list.data) then
		cnt = #list.data;
	end
	return cnt;
end
--获取选择的文本字符
local function f_get_label(list,index)
	local label;
	if(list.data) then
		label = list.data[index + 1];
	end
	return label;
end
--根据index获取其label
function NListBox:getLabelByIndex(index)
	return f_get_label(self,index);
end

local function f_refresh(self,index,isSpread)
	local list = self;
	
	local c = f_get_label(self,index) or "";
	--print("####",index,isSpread,c,"*");
	--print(string.format("[%s]",c));
	if(c~="") then
		self.labelstr = c;
	else
		c = self.labelstr or "";
	end
	
	if(list.data) then
		local n;
		local str = c.."\n";
		local _len = #list.data;
		if(isSpread) then
			for n = 1,_len do
				str = str..list.data[n].."\n";
			end
		end
		
		if(#str > 0 ) then
			local s = string.sub(str,0,#str - 1);--去掉最后的一个换行符
			local label = list.tf;
			
			label:set_text(s);
			--print("set label:",s);
		end
	end
	if(self.callBack and index ~= -1)then
		self.callBack(self,index,self.param);
	end
end

function NListBox:get_container()
	return self.bg:get_container();
end
function NListBox:selectIndex(v)
	f_refresh(self,v);
end

local function f_click(name,self)
--	f_select_call(list);
	local bg = self.bg;
	local w,h = bg:get_size();
	local height = self.h;
	--print(height,h);
		
	local isSpread;
	
	if(h <= self.h) then
		--展开
		height = (self:get_cnt() + 1) * self.h;
		--print(0);
		isSpread = true;
	else
		--print(1);
	end
	bg:set_height(height);
	bg:hit_rect(0,0,w,height);
	
	local mx,my = bg:local_mouse_xy();
	local index = math.floor(my / self.h) - 1;
	
	--print(index,name,self,h,height,self.h );
	--print(height,h,self.h);
	if(index>=0) then
		self.index = index;
	end
	f_refresh(self,index,isSpread);
end

function NListBox:visible(v)
	local bg = self.bg;
	local tf = self.tf;
	bg:visible(v);
	tf:visible(v);
end

function NListBox:set_pos(x,y)
	self.bg:set_pos(x,y);
end

--w的listBox的宽度
--size:Label的size大小
function NListBox:new(x,y,size,w)
	x = x or 0;
	y = y or 0;
	
	local self = Base:new();
	setmetatable(self, NListBox);
	self:settype(13);
	
	local h = 12;
	w = w or 64;
	size = size or 64;
	--local label = NLabel:new();
	--label:set_text("R");
	--local w,h = label:get_size();
	
	--print(w,h);
	
	local bg = Shape:new(true,w,h);
	bg:set_pos(x,y);
	self.bg = bg;
	bg:mouseEnable(true);
	bg:on(EVENT_ENGINE_SPRITE_CLICK,f_click,self);
	self.h = h;
	self.index = -1;
	local label = NLabel:new(size,size);
	self.tf = label;
	bg:addChild(label:get_container());
	return self;
end

function NListBox:addItem(str)
	local list = self;
	if(list.data==nil)then
		list.data = {};
	end
	list.data[#list.data+1]=str;
end

function NListBox:dispose()
	local bg = self.bg;
	bg:off(EVENT_ENGINE_SPRITE_CLICK,f_click);
	local tf = self.tf;
	tf:dispose();
	bg:dispose();
	func_clearTableItem(self);
end
--p:参数
function NListBox:bind(func,p)
	self.callBack = func;
	self.param = p;
end