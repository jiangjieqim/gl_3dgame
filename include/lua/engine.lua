local _engine;

function engine_init()
	local e = {
		
	};
	
	local cam2d,cam3d= getEngine();
	e.cam2d = cam2d;
	e.cam3d = cam3d;
	--[[e.fbo2dcam = fbo2dcam;
	e.fbo3dcam = fbo3dcam;
    e.fboTexture = fboTexture;
	e.fbo = fbo;
	--]]
	--cam_set_2dxy(e.fbo2dcam,50,100);
	
	return e;
end


--[[
--当前引擎中的FBO对象引用
function engine_get_fbo()
	local e = engine_get();
	return e.fbo;
end
function engine_get_3dcam()
	return e.cam3d;
end
--]]


--[[
--创建一个装载fbotextue的sprite
local function f_createFboSprite()
	local size = 256;
    local spr = sprite_createEmptyTex(size,size);
    local tex = engine_get().fboTexture;
    if(tex)then
        local mat = sprite_get_material(spr);
        tmat_pushTex(mat,tex);
    else
        print("fboTexture is nil");
    end
    return spr;
end
--]]
--构造一个新的层级
function engine_newPosZ()
    return get_attr(nil,"newPosZ");
end

--重命名
function engine_rename(o,value)
	change_attr(o,"rename",tostring(value));
end

--刷新引擎默认的cam
function engine_refreshCam3d()
	change_attr(nil,"cam_refreshModel",_engine.cam3d);
end
--添加一个节点到引擎主frame,只有添加进去的节点才能被渲染
function engine_addNode(n)
    change_attr(nil,"ex_add",n);
end
--[[
--获取装载fbo的Sprite
function engine_get_fbo_sprite()
    local e = engine_get();
    if(e.fboSprite==nil) then 
        e.fboTexture = f_createFboSprite();
    end
    return _engine.fboTexture;
end
--]]
--当前的fps
function engine_get_fps()
	return get_attr(nil,"fps");
end

--每一帧需要的毫秒数
function engine_getDelayTime()
    return get_attr(nil,"delayTime");
end

--退出引擎
function engine_exit()
    change_attr(nil,"exit");
end

function engine_setBg(r,g,b)
    --设置背景色
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end
function engine_get()
    return _engine;
end
--设置一个默认的3dcam给对象
function engine_bind_default_3dcam(self)
	local e = engine_get();
	set_cam(self,e.cam3d);
end

_engine=engine_init();
