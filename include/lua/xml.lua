function xml_load(url)
	return core_xml("load",url);
end

--function xml_load_str(str)
--	return core_xml("loadstr",str);
--end

--xml中的节点的个数
function xml_get_length(xml)
	return	core_xml("get_node_cnt",xml);
end

function xml_get_node_by_index(xml,index)
	local n = core_xml("getNodeByIndex",xml,index);
	if(n == nil) then
		func_error("find index "..index);
	end
	
	return n;
end

function xml_get_node(xml,k,v)	
	return	core_xml("getrow",xml,k,v);
end

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

function xml_del(xml)
	core_xml("del",xml);
end

