Image = {
	
};
Image.__index = Image;

setmetatable(Image, Shape);

--[[local function clickEvt(d,p)
	print(d,p);
end
--]]

function Image:new(w,h)
	local self = Shape:new(false,w,h);
	setmetatable(self, Image);
	self:settype(10);

	self:init();
	self:mouseEnable(false);--默认关闭鼠标事件
	--self:on(EVENT_ENGINE_SPRITE_CLICK,clickEvt,self);
	return self;
end

function Image:init()
	local sw,sh = self:get_size();
	local img = sprite_create_typical(nil,0,0,sw,sh);
	self.container = img;
	JEngine:getIns():add(img);
end

---* 设置的是单独的icon,非合并的大图集,
---* 直接用的是sprite_set_9grid接口,
---* 这个是每个url就是一个texture
---@param url string
function Image:seticon(url)
	func_texloadend({sprite=self.container;url=url});
end

--线框模式渲染
function Image:linestlye()
	setv(self.container,FLAGS_DRAW_PLOYGON_LINE);
end