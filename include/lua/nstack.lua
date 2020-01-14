NStack = {
	
};

--[[
--example:
		local function f_each(d,p)
			print(d,p);
		end

		local n = NStack:new();
		n:push("a");
		n:push("b");
		n:push("c");
		n:del("b");
		print(n:len());
		n:for_each(f_each,"ppp1");
		n:dispose();

]]

NStack.__index = NStack;

function NStack:new()
	local self = {
		pre,--当前的node
		next,
	};
	setmetatable(self, NStack);
	
	return self;
end

local function createNode(data)
	local node = {};
	node.data = data;
	return node;
end

function NStack:push(data)
	local node = createNode(data);
	
	--[[
	node.pre = self;
	self.next = node;
	self.cur = node;
	
	--]]
	
	node.next = self.next;
	node.pre = self;
	self.next = node;
end

--根据节点中的数据删除一个节点
function NStack:del(data)
	local node = self;
	while(node.next~=nil)
	do
		node = node.next;
		--f(node.data,p);
		if(node.data == data) then
			local n =	node.next;
			if(n) then
				n.pre = node.pre;
			end
            node.pre.next = n;
			break;
		end
	end
end

function NStack:pop()
	if(self:len()>=0) then
		return self.cur;
	end
end
function NStack:for_each(f,p)
	local node = self;
	while(node.next~=nil)
	do
		node = node.next;
		if(f(node.data,p)~=nil)then
			break;
		end
	end
end
--当前的链表长度
function NStack:len()
	local node = self;
	local n = 0;
	while(node.next~=nil)
	do
		n = n + 1;
		node = node.next;
	end
	return n;
end

--销毁链栈
function NStack:dispose()	
	local node = self;
    --local n = node.next;
	local node=self;--self.next;
	
	local curNode = node.next;
	
	while(node~=nil and curNode~=nil)
	do
		node.data = nil;
		node.pre = nil;
        curNode = node.next;
		
		--[[
			if(curNode) then
				print(curNode.data);
			end
		--]]
		
		node = node.next;
	end
	func_clearTableItem(self);
end