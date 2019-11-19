Image = {
	name = nil,
	type = 10,
	container,
};
Image.__index = Image;
function Image:new(w,h)
	w = w or 64;
	h = h or 64;
	local self = {};
	setmetatable(self, Image);
	local img = sprite_create_typical(nil,0,0,w,h);
	self.container = img;
	engine_addNode(img);
	return self;
end

function Image:seticon(url)
	func_texloadend({sprite=self.container;url=url})
end

--Ïß¿òÄ£Ê½äÖÈ¾
function Image:linestlye()
	setv(self.container,FLAGS_DRAW_PLOYGON_LINE);
end

function Image:dispose()
	ptr_remove(self.container);
	func_clearTableItem(self);
end
