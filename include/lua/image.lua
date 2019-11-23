Image = {
	--name = nil,
	--type = 10,
	--container,
};
Image.__index = Image;

setmetatable(Image, Shape);

--[[local function clickEvt(d,p)
	print(d,p);
end
--]]

function Image:new(w,h)
	local self = {};
	self = Shape:new(w,h);
	setmetatable(self, Image);
	self:settype(10);

	self:init();
	self:mouseEnable(false);--默认关闭鼠标事件
	
	
	--self:on(EVENT_ENGINE_SPRITE_CLICK,clickEvt,self);
	
	
	return self;
end

function Image:init()
	local sw,sh = self:getsize();
	local img = sprite_create_typical(nil,0,0,sw,sh);
	self.container = img;
	engine_addNode(img);
end

function Image:seticon(url)
	func_texloadend({sprite=self.container;url=url})
end

--线框模式渲染
function Image:linestlye()
	setv(self.container,FLAGS_DRAW_PLOYGON_LINE);
end
--销毁
function Image:dispose()
	ptr_remove(self.container);
	func_clearTableItem(self);
end
