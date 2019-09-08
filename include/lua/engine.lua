local _engine;

function engine_init()
	local e = {
		_3dcam,
		fbocam,
		fboTexture,
        fboSprite,
	};
	
	local _3dcam,fbocam,fboTexture = getEngine();
	--print(_3dcam..","..fbocam);
	e._3dcam = _3dcam;
	e.fbocam = fbocam;
    e.fboTexture = fboTexture;
	return e;
end
--创建一个装载fbotextue的sprite
local function f_createFboSprite()
    local spr = sprite_createEmptyTex(256,256);
    local tex = engine_get().fboTexture;
    if(tex)then
        local mat = sprite_get_material(spr);
        tmat_pushTex(mat,tex);
    else
        print("fboTexture is nil");
    end
    return spr;
end

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
	change_attr(nil,"cam_refreshModel",_engine._3dcam);
end
--添加一个节点到引擎主frame,只有添加进去的节点才能被渲染
function engine_addNode(n)
    change_attr(nil,"ex_add",n);
end

--获取装载fbo的Sprite
function engine_get_fbo_sprite()
    local e = engine_get();
    if(e.fboSprite==nil) then 
        e.fboTexture = f_createFboSprite();
    end
    return _engine.fboTexture;
end


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
_engine=engine_init();
