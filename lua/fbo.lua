--创建一个装载fbotextue的sprite
local function f_createSprite(size,tex)
	local size = size;
    local spr = sprite_createEmptyTex(size,size);
    if(tex)then
        local mat = sprite_get_material(spr);
        tmat_pushTex(mat,tex);
    else
        print("fboTexture is nil");
    end
    return spr;
end
--初始化FBO对象
local function fboobj_init(tw,th)
    local fbo = {};
    local ptr,cam3d,cam2d,tex = fbo_init(tw,th);--构建fbo对象
	
	fbo.ptr = ptr;

	fbo.cam3d = cam3d;
	
	fbo.cam2d = cam2d;

	fbo.tex = tex;
	
    fbo.size = tw;
	
	fbo.spr = f_createSprite(tw,tex);
	
	JEngine:getIns():add(fbo.spr);--将spr添加到渲染列表
	
	add_fbo(ptr);--添加到引擎的fbolist中
		
	return fbo;
end

--销毁fbo对象
local function fboobj_dispose(fbo)
	fbo_dispose(fbo.ptr);
	remove_fbo(fbo.ptr);--将fbo从引擎的fbolist移除

	ptr_remove(fbo.spr);
end
 --[[
	设置坐标(兼容模式)
--]]
local function func_setPos(p,x,y)
	local pos = x..","..y;
	change_attr(p,"sprite_position",pos)
end
local function fboobj_set_pos(fbo,x,y)
	local spr = fbo.spr;
	func_setPos(spr,x,y);
	cam_set_2dxy(fbo.cam2d,x,y);
end

--print(fbo.ptr);
--**************************************************
FboRender = {
	ptr,cam3d,cam2d,tex,spr
}
FboRender.__index = FboRender;
setmetatable(FboRender, Base);

function FboRender:new(tw,th)
	local fbo = Base:new();
	setmetatable(fbo, FboRender);
	w = w or 128;
	h = h or 128;
	local ptr,cam3d,cam2d,tex = fbo_init(tw,th);--构建fbo对象
	fbo.ptr = ptr;
	fbo.cam3d = cam3d;
	fbo.cam2d = cam2d;
	fbo.tex = tex;
	
	fbo.spr = f_createSprite(tw,tex);
	
	JEngine:getIns():add(fbo.spr);--将spr添加到渲染列表
	
	add_fbo(ptr);--添加到引擎的fbolist中
	return fbo;
end

--装载fbo的sprite容器句柄
function FboRender:get_container()
	return self.spr;
end

function FboRender:dispose()
--还需要将添加进来的渲染对象删除掉

	fbo_dispose(self.ptr);
	remove_fbo(self.ptr);--将fbo从引擎的fbolist移除
	ptr_remove(self.spr);--销毁fbo占用的sprite对象
end

function FboRender:get_cam3d()
	return self.cam3d;
end
function FboRender:get_cam2d()
	return self.cam2d;
end
--设置fbo所在的屏幕坐标
function FboRender:set_pos(x,y)
	local fbo = self;
	local spr = fbo.spr;
	func_setPos(spr,x,y);
	cam_set_2dxy(fbo.cam2d,x,y);
end
--显示,隐藏Fbo的Sprite
function FboRender:visible(v)
	if(v) then
		setv(self.spr,FLAGS_VISIBLE);
	else 
		resetv(self.spr,FLAGS_VISIBLE);
	end
end