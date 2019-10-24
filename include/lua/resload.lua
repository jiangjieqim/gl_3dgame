local function f_start(url,obj)
   	if(obj.cnt >= 0) then
		local node = obj.list[obj.cnt-1];
		evt_on(0,EVENT_ENGINE_TEX_LOAD_COMPLETE,f_start,obj);
		load_tex(node.url);
		obj.cnt = obj.cnt - 1;
		
		func_clearTableItem(node);
		node = nil;
	else
		obj.callback(obj.param);
		func_clearTableItem(obj);
		obj = nil;
	end
end

--加载资源完成之后回调
function resload(endCallBack,param,...)
	local obj = {}
	obj.list = {};
	obj.callback = endCallBack;
	obj.param = param;
	obj.cnt = 0;
	
	
	for index,value in ipairs({...}) do
		--print(index,value);
		local node = {};
		node.index = index - 1;
		node.url = value;
		obj.list[node.index] = node;
		obj.cnt = index;
	end
	f_start("",obj);
	--endCallBack(param);
end

