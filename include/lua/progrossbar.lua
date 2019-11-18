--进度条组件
ProgressBar={
	type = 7,
	name,
	container,--容器
	
	--**********************************
	barwidth,--进度条数据
	barheight,
	bar;
};
ProgressBar.__index = ProgressBar;
function ProgressBar:new(x,y,w,h)
	local self = {};
	setmetatable(self, ProgressBar);
	w = w or 80;
	h = h or 10;
	
	local gap = 4;	
	
	local url = "checkbox.png";
	local sprite = sprite_create_typical(nil,x,y,w,h);
	engine_addNode(sprite);
	--loadtexs(url,func_texloadend, { sprite=sprite;url=url});
	
	func_texloadend({ sprite=sprite;url=url});
	self.container = sprite;
	
	self.barwidth = w - gap;
	self.barheight= h - gap;
	local bar1 =  sprite_create_typical(nil,0,0,self.barwidth,self.barheight);
	engine_addNode(bar1);
	func_texloadend({sprite=bar1;url="gundi.png"});--gundi
	
	func_addchild(sprite,bar1,gap/2,gap/2);
	
	self.bar = bar1;
	return self;
end

--设置百分比
function ProgressBar:progress(v)
	local o = self.bar;
	if(v <=0) then
		resetv(o,FLAGS_VISIBLE);--进度值小于0就隐藏掉bar,优化drawcall
	else
		if(v > 1.0) then
			func_error("值超过1.0!");
		else
			setv(o,FLAGS_VISIBLE);
			local w = v * self.barwidth;
			func_set_sprite_size(self.bar,w,self.barheight);
		end
	end
end
--销毁
function ProgressBar:dispose()
	ptr_remove(self.bar);
	ptr_remove(self.container);
	func_clearTableItem(self);
end