--local evtlist = {};

local list = NStack:new();--事件列表
local function f_each(node,p)
	--print(d,p);

	--[[local evt = 
	{
		id = id;
		func = func;
		obj = obj;
        params = params;
		once = once
	}
--]]
	if(node and node.obj == p.obj and node.id == p.id and node.func == p.func) then
		p.find = true;
		func_print("重复的事件,事件id = "..p.id);
		return true;
	end
end

local function f_find(node,p)
	if(node and node.obj == p.obj and node.id == p.id and node.func == p.func) then
		p.node = node;
		return true;
	end
end

--node and node.obj == p.obj and 
local function f_find_dispath(node,p)
	if(node.id == p.id) then
		p.node = node;
		return true;
	end
end

local function f_find_dispath_obj(node,p)
	if(node and node.obj == p.obj and node.id == p.id) then
		p.node = node;
		return true;
	end
end

--是否是table字符串
local function func_is_table_str(value)
	local s = tostring(value);
	return string.find(s,"table: ") ~= nil;
end
--[[
    params:默认不传递参数

    local function f_resize(evtData,self)
        print(c_data);--来自evt_dispatch中的	local data;--数据
    end
]]--
--将table转化为一个Number值(其实是table的地址引用)
local function f_cv(obj)
	if(func_is_table_str(obj)) then
		local n = func_get_address(obj);
		func_print("evt_on==>"..tostring(obj).." 转化为Number:"..string.format("%0x",n));
		return n;
	end
	return obj;
end

function evt_on(obj,id,func,params,once)	
	
	obj = f_cv(obj);--is number
	
    if(func == nil) then
        func_error("func = nil");
    end

	--local str = string.format("绑定事件 evt id = %d , obj = %d",id,obj);
	local str = string.format("绑定事件 evt id=(%d),obj=(%s) params=(%s)",id,tostring(obj),tostring(params));
	func_print(str);


	local findobj = {};
	findobj.obj = obj;
	findobj.id = id;
	findobj.func = func;
	findobj.find = false;
	
	list:for_each(f_each,findobj);
	if(findobj.find)then
		
	else
		local evt = 
		{
			id = id;
			func = func;
			obj = obj;
			params = params;
			once = once;
		}
		--local v = tonumber(f_cv(evt));
		list:push(evt);
		
		--func_print("add之后事件"..id.."数量"..list:len());
	end
--	evtlist[evt] = evt;
	findobj = nil;
end

function evt_once(obj,id,func,params)
	evt_on(obj,id,func,params,true);
end

--是否有该事件
function evt_has(obj,id,func)
	--[[for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.id == id and node.func == func and node.obj == obj) then
			return true
		end
	end
	return false--]]
	
	local findobj = {};
	findobj.obj = obj;
	findobj.id = id;
	findobj.func = func;
	findobj.node = nil;
	
	list:for_each(f_find,findobj);
	
	if(findobj.node) then
		return true;
	end
    return false;
end
--移除事件 并且释放事件引用
function evt_off(obj,id,func)

--[[
	if(evt_has(obj,id,func)==false)then
		local str = string.format("evt_has移除[%d]事件[%d]失败! 事件不存在",obj,id);
		func_print(str);
		return;
	end
--]]

	
	local ok = false;
	--[[
	for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.id == id and node.func == func and node.obj == obj) then
			
			local str = string.format("移除事件 evt_off==> evt.id = "..id..",obj = "..obj);
			func_print(str);
			
			

			evtlist[node]=nil;
			func_clearTableItem(node);
			node = nil;
			ok = true;
		end
	end
	--]]
	local findobj = {};
	findobj.obj = obj;
	findobj.id = id;
	findobj.func = func;
	findobj.node = nil;
	
	list:for_each(f_find,findobj);
	if(findobj.node) then
		local str = string.format("移除事件 evt_off==> evt.id = "..id..",obj = "..obj);
		func_print(str);
		
		list:del(findobj.node);
		--func_print("del之后的事件"..id.."数量"..list:len());
		ok = true;
		
	else
		func_error("移除事件"..id.."失败!");
	end
	
	findobj = nil;
	
	--[[if(ok~=true) then
		func_print("移除事件"..id.."失败!",0xff0000);
	end--]]
	
	return ok;
end


--全局事件
function evt_dispatch(...)
	 --id,data,obj
	local obj; --指向的对象
	local id;  --事件id
	local data;--数据
--	obj ,id,data = f_parse({...});
	local arr = {...};
	obj = arr[1];
	id  = arr[2];
	data= arr[3];
	
	obj = f_cv(obj);

	
--	print("evt_dispatch",obj ,id,data );
	
	
		
--[[		
	for index,value in ipairs({...}) do
		--print(index,value);
		if(index == 1) then
			obj = value;
		elseif(index == 2) then
			id = value;
		elseif(index == 3) then
			data = value;
		end
	end--]]
	
--    if(id == 103)then
--        print("************");
--    end
	
	
	local findobj = {};
	findobj.obj = obj;
	findobj.id = id;
	findobj.node = nil;

	
	if (obj == 0) then
		--全局事件
		--[[for k, v in pairs(evtlist) do
			local node = evtlist[k];
			if(node and node.id == id) then
				node.func(data,node.params);
				if(node.once) then
					evt_off(obj,id,node.func);--obj,id,func
					--func_print("移除全局事件"..id);
				end
			end
		end--]]
		list:for_each(f_find_dispath,findobj);
		local node = findobj.node;
		if(node)then
			node.func(data,node.params);
			if(node.once) then
				evt_off(obj,id,node.func);--obj,id,func
				--func_print("移除全局事件"..id);
			end
		end
	else
		--print(id,data,obj);
	
		
		list:for_each(f_find_dispath_obj,findobj);

		local node = findobj.node;
		if(node)then
			node.func(data,node.params);--data:C层传递的参数 node.params:Lua层传递的参数
				
				if(node.once) then
					
					--local str = string.format("evt_dispatch之后移除事件 evt id = %d  node = (%s) obj =(%d)  data=(%s) node.params=(%s)",id,tostring(node),obj,tostring(data),tostring(node.params));
					--func_print(str);
					
					evt_off(obj,id,node.func);--obj,id,func
				end
		end
	end
	--print(id);
end