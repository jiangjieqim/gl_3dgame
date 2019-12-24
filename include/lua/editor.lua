ModleShow = {
	nskin,
	fbo,
};
ModleShow.__index = ModleShow;

local function btnClick(self)
	--print("p=",self);
	--m_dispose(self);

	self:dispose();
end
local function f_cpmlete(self)
	local skin = self.nskin;
	local x,y = skin:get_pos();
	
	local fbo = FboRender:new(128,128);
	fbo:set_pos(x,y);
	self.fbo = fbo;
	--self.fbo:dispose();
	--print(x,y);
	--self:dispose();
	--skin:dispose();

	--self.nskin:dispose();
	
	
	local btn0 = skin_find(skin,"btn0");
	btn0:bind_click(btnClick,self);
end

function ModleShow:new()
	local self = {};
	setmetatable(self, ModleShow);
	
	local function f_cpmleteHandler(skin)
		f_cpmlete(self);
		--m_dispose(self);
	end
	local nskin = NSkin:new();
	self.nskin = nskin;
	evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmleteHandler);
	nskin:load("\\resource\\modelshow.xml","gundi.png;checkbox.png;smallbtn.png");	
	
	return self;
end
--销毁ModelShow
function ModleShow:dispose()
	--print("ModleShow:show");
	if(self.fbo) then
		self.fbo:dispose();
	end
	
	self.nskin:dispose();
	
	func_clearTableItem(self);
end


--************************************************************
Editor = {
	linelist,
	modelshow,
};
Editor.__index = Editor;

--创建线框
local function f_createLines(n,s)
	if(n == 1) then
		func_error("f_createLines n is error!");
	end
	
	local list = stack_new();

	s = s or 1;--线段半径
	local _l = n / 2;--_l or 1;

	--print(n,index);
	
	--print(s);
	for i = 0,n,1 do 
		local ln= LineNode:new(2);
		ln:setcolor(0.5,0.5,0.5);
		local x = (i-n/2)*s;
		--print(x,s);
		ln:push(x,0,-_l);
		ln:push(x,0,_l);
		ln:pushend();
		stack_push(list,ln);
	end
	
	for i = 0,n,1 do 
		local ln= LineNode:new(2);
		ln:setcolor(0.5,0.5,0.5);
		local z = (i-n/2)*s;
		--print(x,s);
		ln:push(-_l,0,z);
		ln:push(_l,0,z);
		ln:pushend();
		stack_push(list,ln);
	end
	return list;
end
function Editor:new()
	local self = {};
	setmetatable(self, Editor);
	self:createLine();
	
	
	self.modelshow = ModleShow:new();
	
	return self;
end

function Editor:createLine()
	self.linelist = f_createLines(10,1);
end

local function f_delLines(node,index,p)
	--print(node,index,p);
	node:dispose();
end
function Editor:dispose()
	--print(self);
	if(self.linelist) then
		stack_foreach(self.linelist,f_delLines);
	end
	self.modelshow:dispose();
	
	func_clearTableItem(self);
end