--管理器
local loadman = {
	isload=0,--是否在加载
	list = {};--列表
	cnt = 0;
	loadfunc=nil;
};

local function f_getnode()
	if(loadman.cnt == 0) then
		return nil;
	else
		loadman.cnt = loadman.cnt - 1;
		return loadman.list[loadman.cnt];
	end
end

local function f_callback(data,param)
	local node = param;
	
	--print("加载完成",node.callback,node.res);
	
	if(node.callback) then
		--print("回调",node.p);
		node.callback(node.p);
	end
	func_clearTableItem(node);
	local n =  f_getnode();
	
	if(n) then
		loadman.loadfunc(n);
	else
		loadman.isload = 0;
	end
end
local function f_load(node)
	--print("开始加载,加载状态:",loadman.isload);
	local r =  load_tex(node.res,JEngine:getIns():get_atals());
	evt_once(r,EVENT_ENGINE_TEX_LOAD_COMPLETE,f_callback,node);
    load_tex_start(r);
end

--开始加载资源
--callback:回调函数,p:参数
function loadtexs(res,callback,p)
	local node = {
		res = res;
		callback = callback;
		p = p;--参数
	};
	loadman.loadfunc = f_load;
	if(loadman.isload == 1) then
		loadman.list[loadman.cnt] = node;
		loadman.cnt = loadman.cnt + 1;
		func_print(string.format("%s[%s]","队列中有加载项目,等待加载完成之后 异步加载",res));
	else
		loadman.isload = 1;
		f_load(node);
	end
end