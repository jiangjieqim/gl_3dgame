-------------------------------------------------
JEngine ={
	atals,
	cam2d,
	cam3d,
};

JEngine.__index = JEngine;
setmetatable(JEngine, Instance);--继承自单例

function JEngine:new()
	local self =  Instance:new();
	setmetatable(self, JEngine);
	local uiAtals=atals_load("//resource//texture//","1");
    self.atals = uiAtals;
	
	
	local cam2d,cam3d = getEngine();
	self.cam2d = cam2d;
	self.cam3d = cam3d;
	return self;
end
function JEngine:get_cam()
    return  self.cam2d,self.cam3d;
end
function JEngine:get_atals()
    return self.atals;
end

function JEngine:dispose()
	atals_dispose(self.atals);
	func_clearTableItem(self);
end

function JEngine:new_z()
	 return get_attr(nil,"newPosZ");
end

function JEngine:rename(o,value)
	change_attr(o,"rename",tostring(value));
end

function JEngine:refresh_cam3d()
	change_attr(nil,"cam_refreshModel",self.cam3d);
end
function JEngine:get_fps()
    return get_attr(nil,"fps");
end

function JEngine:get_delayTime()
	return get_attr(nil,"delayTime");
end

function JEngine:exit()
	change_attr(nil,"exit");
end

function JEngine:setbg(r,g,b)
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end

function JEngine:bind_3dcam(o)
	set_cam(o,self.cam3d);
end

function JEngine:add(n)
	change_attr(nil,"ex_add",n);
end

--加载一个资源,只是加载,并不会加载到渲染列表
--默认都是加载vbo类型的数据
function JEngine:load(url,name)
	name = name or func_create_name();
	return change_attr(nil,"ex_loadVBO",name,url);
end
--设置3dcamera的坐标
function JEngine:set_cam_pos(x,y,z)
	change_attr(nil,"cam_xyz",string.format("%.3f,%.3f,%.3f",x,y,z));
end


--[[
	获取对象的类型
#define TYPE_OBJ_FILE 0	//obj类型
#define TYPE_MD5_FILE 1 //md5类型
#define TYPE_MD2_FILE 2 //md2类型
#define TYPE_SPRITE_FLIE 3//UI界面类型中的sprite
#define TYPE_TEXT_FILE	4	//文本类型
#define TYPE_OBJ_VBO_FILE	5//VBO文件数据

--]]
--function func_get_type(p)--func_getGameObjectType
	--func_error();
--	return	get_attr(p,"type")
--end
function JEngine:get_type(p)
	return	get_attr(p,"type");
end

function JEngine:get_type_str(p)
	local t = self:get_type(p);
	if(t == 0) then
		return 'obj'
	elseif(t == 1)then
		return 'md5'
	elseif(t == 2)then
		return 'md2'
	elseif(t == 3)then
		return 'sprite'
	elseif(t == 4)then
		return 'text'
	elseif(t == 5)then
		return 'vbo'
	end
end

function cam_setRotateX(rx)
	--print(rx);
	change_attr(nil,"camRX",rx);
end

--重置camera
function cam_reset()
    change_attr(nil,"cam_xyz",string.format("%.3f,%.3f,%.3f",0,0,0));
    change_attr(nil,"camRX",0);
    change_attr(nil,"camRY",0);
    change_attr(nil,"camRZ",0);
end



