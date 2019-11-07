--获取父亲节点
local function f_get_parent(list,node)
	local parentName = xml_get_str(node,"parent");
	if(parentName ~= "") then
		local fnode	 = stack_find_by_name(list,parentName);
		return func_get_container(fnode);
	end
	return nil;
end

local function f_create_byNode(list,node,name,_type)
	local parent = f_get_parent(list,node);
	local x = xml_get_float(node,"x");
	local y = xml_get_float(node,"y");
	--print("构造对象=============>",x,y,"name=",name,_type);
	
	if(_type == "Panel") then
		
		local a = alert_init();
		a.name = name;
		alert_start(a);
		local center = xml_get_float(node,"center");
		local drag = xml_get_float(node,"drag");
		
		if(drag == 1)then
			alert_set_drag(a,true);
		end
		if(center == 1) then
			alert_enable_resize(a,true);
		end
		
		stack_push(list,a);
	elseif(_type == "Label") then
		local label =  label_create();
		label.name = name;
		local str = xml_get_str(node,"label");
		label_set_text(label, str);
		
		if(parent)then
			func_addnode(parent,label,x,y);
		end
		stack_push(list,label);
	elseif(_type == "Button") then
		
		local btn = btn_create();
		btn.name = name;

		local str = xml_get_str(node,"label");
		if(str~="")then
			btn_label(btn,str);
		end
		func_addnode(parent,btn,x,y);
		stack_push(list,btn);
		
	elseif(_type == "ScrollBar") then
		local sc = scrollBar_new();
		func_addnode(parent,sc,x,y);
		stack_push(list,sc);
	end
	--print("====>",parent);
end

local function f_tex_complete(param)
	local ins = param;
	local xml = xml_load(ins.url);

	local cnt = xml_get_length(xml);

	--print(cnt);

	local n = 0;
	for n = 0,cnt-1 do
		local node = xml_get_node_by_index(xml,n);
		local name = xml_get_str(node,"name");
		local _type = xml_get_str(node,"type");
		--print("n = ",n,name,_type);
		f_create_byNode(ins.list,node,name,_type);
	end
	xml_del(xml);
end

function loadui(url,texs)
	
	local ins = {
		list;--栈链表
		url,--xml url
	};
	ins.url = url;
	texs = texs  or "gundi.png;checkbox.png";
	ins.list=stack_new();

	loadtexs(texs,f_tex_complete,ins);
	return ins;
end
