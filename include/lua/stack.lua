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
--±éÀúÕ»
function stack_foreach(l,func)
	local list = l.list;
	for i=0,l.cnt-1,1 do
		local node = l.list[i];
		func(node,i);
	end
end

function example_stack()
	local function forEach(n,index)
		print(n,index);
	end
	
	
	local _l = stack_new();
	stack_push(_l,"a");
	stack_push(_l,"b");
	stack_push(_l,"c");
	
	stack_pop(_l);
	
	stack_foreach(_l,forEach);
end