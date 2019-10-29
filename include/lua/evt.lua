local evtlist = {};

--[[
    params:默认不传递参数

    local function f_resize(evtData,self)
        print(c_data);--来自evt_dispatch中的	local data;--数据
    end
]]--

function evt_on(obj,id,func,params,once)	
	
    if(func == nil) then
        func_error("func = nil");
    end

	for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.obj == obj and node.id == id and node.func == func) then
			--node.func(data);
			--重复的事件
			return;
		end
	end	
		
	local evt = 
	{
		id = id;
		func = func;
		obj = obj;
        params = params;
		once = once
	}
	evtlist[evt] = evt;
end


function evt_once(obj,id,func,params)
	evt_on(obj,id,func,params,true);
end


--移除事件 并且释放事件引用
function evt_off(obj,id,func)
	for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.id == id and node.func == func and node.obj == obj) then
			node.id = nil
			node.func = nil;
			node.obj = nil;
            node.params = nil;
			evtlist[node]=nil;
		end
	end
end
--是否有该事件
function evt_has(obj,id,func)
	for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.id == id and node.func == func and node.obj == obj) then
			return true
		end
	end
	return false
end

--全局事件
function evt_dispatch(...)
	 --id,data,obj
	local obj; --指向的对象
	local id;  --事件id
	local data;--数据
	
	for index,value in ipairs({...}) do
		--print(index,value);
		if(index == 1) then
			obj = value;
		elseif(index == 2) then
			id = value;
		elseif(index == 3) then
			data = value;
		end
	end
	
--    if(id == 103)then
--        print("************");
--    end

	if (obj == 0) then
		--全局事件
		for k, v in pairs(evtlist) do
			local node = evtlist[k];
			if(node and node.id == id) then
				node.func(data,node.params);
				if(node.once) then
					evt_off(obj,id,node.func);--obj,id,func
					print("移除事件"..id);
				end
			end
		end
	else
		--print(id,data,obj);
		for k, v in pairs(evtlist) do
			local node = evtlist[k];
			if(node and node.obj == obj and node.id == id) then
				node.func(data,node.params);
				if(node.once) then
					evt_off(obj,id,node.func);--obj,id,func
					print("移除事件"..id);
				end
			end
		end
	end
	--print(id);
end




