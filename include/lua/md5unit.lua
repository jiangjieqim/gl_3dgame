Md5Unit = {
	
};

Md5Unit.__index = Md5Unit;
setmetatable(Md5Unit, NUnit);

function Md5Unit:new()
	local s = NUnit:new();
	setmetatable(s,Md5Unit);
	return s;
end
--动作管理器句柄
function Md5Unit:get_anim()
	
end
--加载模型
function Md5Unit:load(cam,model,material)
	self.p = load_model(func_create_name(),model  or "\\resource\\md5\\wolf\\body.md5mesh");
	setMaterial(self.p,self:load_material(material or "//resource//material//wolf.mat"));
	
	md5_loadAnim(self.p, "\\resource\\md5\\wolf\\walk.md5anim","walk");
	
	self:setv(FLAGS_ANIM_ADAPTIVE);

	self:set_cam(cam);
	self:set_fps(260);
	self:visible(true);
end

function Md5Unit:dispose()
	ptr_remove(self.p);
	func_clearTableItem(self);
end

--绘制骨骼节点
function Md5Unit:drawSkeleton(v)
	local flag = FLAGS_RENDER_DRAWSKELETON;
	if(v) then
		self:setv(flag);
	else
		self:resetv(flag);
	end
end
--设置当前帧
function Md5Unit:set_frame(v)
	change_attr(self.p, "setframe", v)
end
--获取动作关键帧数(md5)
function Md5Unit:frame_count()
	return get_attr(self.p,"frameCount");
end
