local _engine;

function engine_init()
	local e = {
		_3dcam,
		fbocam,
		fboTexSprite,
	};
	
	local _3dcam,fbocam,fboTexSprite = getEngine();
	--print(_3dcam..","..fbocam);
	e._3dcam = _3dcam;
	e.fbocam = fbocam;
    e.fboTexSprite = fboTexSprite;
	return e;
end
--构造一个新的层级
function engine_newPosZ()
    return get_attr(nil,"newPosZ");
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
function engine_getFBO_sprite()
    return _engine.fboTexSprite;
end

function engine_setBg(r,g,b)
    --设置背景色
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end

_engine=engine_init();