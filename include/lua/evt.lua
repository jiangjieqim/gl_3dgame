local evtlist = {};

--[[
    params:默认不传递参数

    local function f_resize(evtData,self)
        print(c_data);--来自evt_dispatch中的	local data;--数据
    end
]]--

function evt_on(obj,id,func,params,once)	
	
	if(func_is_table_str(obj)) then
		func_print("evt_on==>"..tostring(obj).." 转化为number");
		obj = func_get_address(obj);
	end
	
	
    if(func == nil) then
        func_error("func = nil");
    end

	--local str = string.format("绑定事件 evt id = %d , obj = %d",id,obj);
	local str = string.format("绑定事件 evt id=(%d),obj=(%s) params=(%s)",id,tostring(obj),tostring(params));
	func_print(str);

	for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.obj == obj and node.id == id and node.func == func) then
			--node.func(data);
			--重复的事件
			
			func_print("重复的事件");
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
					func_print("移除全局事件"..id);
				end
			end
		end
	else
		--print(id,data,obj);
		for k, v in pairs(evtlist) do
			local node = evtlist[k];
			if(node and node.obj == obj--[[  接受事件的对象检测判断--]] and node.id == id) then
				
				node.func(data,node.params);
				
				if(node.once) then
					
					local str = string.format("evt_dispatch之后移除事件 evt id = %d  node = (%s) obj =(%d)  data=(%s) node.params=(%s)",id,tostring(node),obj,tostring(data),tostring(node.params));
					func_print(str);
					
					evt_off(obj,id,node.func);--obj,id,func
				end
			end
		end
	end
	--print(id);
end




