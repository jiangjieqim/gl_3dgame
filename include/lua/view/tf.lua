local tools = {};

function tools.get()
	-- body
	print("run it!!!");
end

local MyClass = {
};
MyClass.__index = MyClass;
setmetatable(MyClass, IPlugin);--继承自插件接口

function MyClass:load()
	local self = {name,label,tf};
	setmetatable(self, MyClass);
	self.name = tostring(math.random());
	print("MyClass=====>"..self.name);
	--self.label = NLabel:new(64,64);
	return self;
end

function MyClass:init()
	self.label = NLabel:new(64,64);
end

function MyClass:unload()
	if(self.label) then
		self.label:dispose();
	end
	func_clearTableItem(self);
	
	--setmetatable(self, nil);
	--MyClass.__index = nil;
end

function MyClass:getName()
	return "MyClass";
end

function MyClass:show(msg)
	if(self.label) then
		self.label:set_text(msg);
	end
	print("MyClass show!  ==>"..msg);
end
--print("====>",tools);
--return tools;
return MyClass;