
Editor = {
	linelist,
};
Editor.__index = Editor;

--创建线框
local function f_createLines(n,s)
	if(n == 1) then
		func_error("f_createLines n is error!");
	end
	
	local list = stack_new();

	s = s or 1;--线段半径
	local _l = n / 2;--_l or 1;

	--print(n,index);
	
	--print(s);
	for i = 0,n,1 do 
		local ln= LineNode:new(2);
		ln:setcolor(0.5,0.5,0.5);
		local x = (i-n/2)*s;
		--print(x,s);
		ln:push(x,0,-_l);
		ln:push(x,0,_l);
		ln:pushend();
		stack_push(list,ln);
	end
	
	for i = 0,n,1 do 
		local ln= LineNode:new(2);
		ln:setcolor(0.5,0.5,0.5);
		local z = (i-n/2)*s;
		--print(x,s);
		ln:push(-_l,0,z);
		ln:push(_l,0,z);
		ln:pushend();
		stack_push(list,ln);
	end
	return list;
end
function Editor:new()
	local self = {};
	setmetatable(self, Editor);
	self.linelist = f_createLines(10,1);
	return self;
end
local function f_delLines(node,index,p)
	--print(node,index,p);
	node:dispose();
end
function Editor:dispose()
	--print(self);
	stack_foreach(self.linelist,f_delLines);
	func_clearTableItem(self);
end