function stack_new()
	local l  = {
		cnt = 0;
		list = {};
	}
	return l;
end

function stack_push(l,node)
	l.list[l.cnt] = node;
	l.cnt = l.cnt + 1;
end

function stack_length(l)
	return l.cnt;
end

function stack_del(l)
	func_clearTableItem(l.list);
	func_clearTableItem(l);
end

function stack_pop(l)
	local length = stack_length(l);
	if(length <=0) then
		return nil;
	else	
		local node = l.list[l.cnt-1];
		l.list[l.cnt-1] = nil;
		l.cnt = l.cnt - 1;
		return node;	
	end
end
--遍历栈
function stack_foreach(l,func,p)
	local list = l.list;
	for i=0,l.cnt-1,1 do
		local node = l.list[i];
		func(node,i,p);
	end
end
--根据名字寻找node
function stack_find_by_name(l,name)
	local list = l.list;
	for i=0,l.cnt-1,1 do
		local node = l.list[i];
		if(node.name == name)then
			return node;
		end
	end
end

function example_stack()
	local function forEach(n,index,p)
		print(n,index,p);
	end
	
	
	local _l = stack_new();
	stack_push(_l,"a");
	stack_push(_l,"b");
	stack_push(_l,"c");
	
	stack_pop(_l);
	
	stack_foreach(_l,forEach,"params");
end

--example_stack();