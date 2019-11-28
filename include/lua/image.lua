Image = {
	
};
Image.__index = Image;

setmetatable(Image, Shape);

--[[local function clickEvt(d,p)
	print(d,p);
end
--]]

function Image:new(w,h)
	local self = {};
	self = Shape:new(false,w,h);
	setmetatable(self, Image);
	self:settype(10);

	self:init();
	self:mouseEnable(false);--Ĭ�Ϲر�����¼�
	--self:on(EVENT_ENGINE_SPRITE_CLICK,clickEvt,self);
	return self;
end

function Image:init()
	local sw,sh = self:get_size();
	local img = sprite_create_typical(nil,0,0,sw,sh);
	self.container = img;
	engine_addNode(img);
end

function Image:seticon(url)
	func_texloadend({sprite=self.container;url=url})
end

--�߿�ģʽ��Ⱦ
function Image:linestlye()
	setv(self.container,FLAGS_DRAW_PLOYGON_LINE);
end