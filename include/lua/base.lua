Base = {
	name = nil,--该对象的名字
	type = nil,--该对象的类型
};
Base.__index = Base;
function Base:setname(name)
	self.name = name;
end

function Base:new()
	local self = {};
	setmetatable(self, Base);
	return self;
end

function Base:settype(t)
	--print(self,"设置类型"..t);
	self.type = t;	
end

function Base:gettype()
	return self.type;
end

function Base:setname(name)
	print(string.format("设置名字为[%s]",name));
	self.name = name;
end

function Base:getname()
	return self.name;
end

--************单例**************
Instance={
	ins,--单例引用
};
Instance.__index = Instance;

function Instance:new()
	local self = {};
	setmetatable(self, Instance);
	--print(tostring(self).."单例初始化");
	return self;
end

--单例
function Instance:getIns()
    if self.ins == nil then
        self.ins = self:new();	
    end
    return self.ins;
end