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
function stack_foreach(l,func,p,isReverse)
	local list = l.list;
	
	if(isReverse) then
		for i=l.cnt-1,0,-1 do
			local node = l.list[i];
			
			--print("===>",i,node);
			
			func(node,i,p);
		end
	else
		for i=0,l.cnt-1,1 do
			local node = l.list[i];
			func(node,i,p);
		end
	end
end

local function forEach(node,index,p)
	print(node,index,p);
end

--local function f_c(l,n)
--	return l.cnt - n;
--end

--local _stat = 0;

local function f_sort(cnt,l,func,newlist)
	local list = l.list;
	
	--local cnt = l.cnt-n;
	
	local i;
	for i = 0,cnt - 1 do
		local _pre = list[i];
		if(i < cnt-1) then
			local _next = list[i+1];
			--print(_pre,_next);
			if	(_next~=nil and func(_pre,_next)==-1)	then
				local t = list[i+1];
				list[i+1]=list[i];
				list[i]=t;
				--print("c")			
			end
			
			--stack_foreach(l,forEach,"test"..i);
		end
	end
	if(l.cnt > 0) then
		local _top = stack_pop(l);
		--local _top = list[cnt-1];
		--_stat=_stat+1;
		--print("big =",_top);
		
		stack_push(newlist,_top);
	end
	local i;
	cnt = l.cnt;
	for i = 0,cnt-1 do 
		f_sort(cnt,l,func,newlist);
	end	
end

--排序
function stack_sort(l,func)
	local newlist = stack_new();
	local i;
	f_sort(l.cnt,l,func,newlist);
	stack_del(l);
	return newlist;	
end

--根据名字寻找node
function stack_find_by_name(l,name)
	local list = l.list;
	for i=0,l.cnt-1,1 do
		local node = list[i];
		if(node.name == name)then
			return node;
		end
	end
end

--根据index获取其节点
function stack_find_by_index(l,index)
	local list = l.list;
	return list[index];
end

--升序
local function test_sort(a,b)
	if(a > b) then
		return -1;
	elseif (a < b) then
		return 1;
	else
		return 0;
	end
end

function example_stack()
	
	
	
	local _l = stack_new();
	stack_push(_l,2);
	stack_push(_l,3);
	stack_push(_l,-30);
	stack_push(_l,1);
	stack_push(_l,5);

	--stack_push(_l,4);
	--stack_pop(_l);
	
	
	--print(func_getTime());
	_l = stack_sort(_l,test_sort);
	--print(func_getTime());

	
	stack_foreach(_l,forEach,"params");
end

--example_stack();