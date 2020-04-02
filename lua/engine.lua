-------------------------------------------------
JEngine ={
	atals,
	cam2d,
	m_cam3d,
	
	plugin,--插件管理器
};

JEngine.__index = JEngine;
setmetatable(JEngine, Instance);--继承自单例

function JEngine:new()
	local self =  Instance:new();
	setmetatable(self, JEngine);
	
	--构造一个图集
	local uiAtals=atals_load("//resource//texture//","1");
    self.atals = uiAtals;
	
	
	local cam2d,cam3d = getEngine();
	self.cam2d = cam2d;
	
	self.m_cam3d = Camera:new(cam3d);

	self.plugin = PluginMan:new();

	return self;
end
--获取插件管理器的句柄
function JEngine:get_plugin()
	return self.plugin;
end

function JEngine:get_cam()
    return self.m_cam3d;
end
--获取屏幕的尺寸
function JEngine:screen_size()
	return get_attr(nil,"screenSize");
end

--获取点击的坐标
function JEngine:get_hit()
	return get_attr(nil,"hit");
end

function JEngine:get_atals()
    return self.atals;
end

function JEngine:dispose()
	atals_dispose(self.atals);
	func_clearTableItem(self);
end

--打印信息
function JEngine:print_info()
	get_attr(nil,"ex_info");
end

function JEngine:rename(o,value)
	change_attr(o,"rename",tostring(value));
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

--从引擎层获取对对象
function JEngine:find(name)
	return dofunc("find_node",name);
end

function JEngine:setbg(r,g,b)
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end

--将对象o绑定到引擎默认的3dcam空间
function JEngine:bind_3dcam(o)
	self:get_cam():bind(o);
end
--添加一个对象到引擎
function JEngine:add(n)
	change_attr(nil,"ex_add",n);
end

--加载一个资源,只是加载,并不会加载到渲染列表
--默认都是加载vbo类型的数据
function JEngine:load(url,name)
	name = name or func_create_name();
	return change_attr(nil,"ex_loadVBO",name,url);
end

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



