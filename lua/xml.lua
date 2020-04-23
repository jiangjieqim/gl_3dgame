--获取节点中等于k的值(string),如果没有取到该key对应的值就返回nil
function xml_get_str(node,k,buffersize)
	--func_error(k);
	buffersize = buffersize or 128;
	local str = core_xml("getstr",node,k,buffersize);
	return	str;
end

function xml_get_float(node,k)
	local v = core_xml("getfloat",node,k);
	if(v==nil) then
		--func_error("key:"..k.." is nil!设置了一个默认值0!");
		v = 0;
	end
	return v;
end
--*****************************************************

--[[
测试用例1:

		local xml = Xml:new();
		xml:load("\\resource\\config.xml");
		local node = xml:get_index(0);
		print(xml:len(),xml_get_float(node,"w"));
		xml:dispose();

--]]


Xml = {

};

Xml.__index = Xml;
function Xml:new()
	local self = {
		p = nil,
	};
	setmetatable(self,Xml);
	return self;
end

function Xml:load(url)
	self.p = core_xml("load",url);
end
--以加载文本的方式构造xml对象
function Xml:loadstr(str)
	self.p = core_xml("loadstr",str);
end

--xml中的节点的个数
function Xml:len()
	return core_xml("get_node_cnt",self.p);
end

function Xml:get_index(index)
	local n = core_xml("getNodeByIndex",self.p,index);
	if(n == nil) then
		func_error("find index "..index);
	end
	return n;
end

function Xml:get_node(k,v)
	return core_xml("getrow",self.p,k,v);
end

function Xml:dispose()
	if(self.p) then
		core_xml("del",self.p);
	end
	func_clearTableItem(self);
end