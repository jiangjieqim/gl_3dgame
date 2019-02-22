

local evtlist = {};

function evt_on(obj,id,func)	
	local evt = 
	{
		id = id;
		func = func;
		obj = obj;
	}
	evtlist[evt] = evt;
end


function evt_off(obj,id,func)
	for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.id == id and node.func == func and node.obj == obj) then
			node.id = nil
			node.func = nil;
			node.obj = nil;
			evtlist[node]=nil;
		end
	end
end

--全局事件
function evt_dispatch(id,data,obj)
	--print(id,data,obj);
	if (obj == nil) then
		--全局事件
		for k, v in pairs(evtlist) do
			local node = evtlist[k];
			if(node and node.id == id) then
				node.func(data);
			end
		end
	else
		for k, v in pairs(evtlist) do
			local node = evtlist[k];
			if(node and node.obj == obj and node.id == id) then
				node.func(data);
			end
		end
	end
	--print(id);
end
