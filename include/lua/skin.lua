--设置自定义的父容器
local function f_set_customParent(skin,parent,x,y)
	skin.customParent = parent;
	skin.ox = x or 0;
	skin.oy = y or 0;
end


--获取父亲节点
local function f_get_parent(list,node)
	local parentName = xml_get_str(node,"parent");
	if(parentName ~= "") then
		local fnode	 = stack_find_by_name(list,parentName);
		return func_get_container(fnode);
	end
	return nil;
end

--parent:父容器对象
local function f_itemskin_load(xmlurl,parent,x,y)
	local skin = skin_load(xmlurl);
	f_set_customParent(skin,parent,x,y);
	skin_parse(skin);
	return skin;
end
--skin的容器
function skin_get_container(skin)
	return skin.container;
end

local function f_create_by_node(skin,node,name,_type)
	local list = skin.list;
	local parent = nil;
	
	if(skin.customParent ~= nil) then
		--使用自定义的父容器对象
		parent = skin.customParent;
	else
		--使用非自定义的父容器的时候,找到该组件的父对象
		parent = f_get_parent(list,node);
	end
	
	skin.container = parent;
	
	local x = xml_get_float(node,"x");
	local y = xml_get_float(node,"y");
	--print("构造对象=============>",x,y,"name=",name,_type);
	
	x = x + skin.ox or 0;
	y = y + skin.oy or 0;

	if(_type == "Panel") then
		local a = alert_init();
		a.name = name;
		
		local center = xml_get_float(node,"center");
		local drag = xml_get_float(node,"drag");
		local width = xml_get_float(node,"width");
		local height = xml_get_float(node,"height");
		
		alert_start(a,nil,width,height);
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
		
		func_addnode(parent,label,x,y);
		
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
		sc.name = name;
		func_addnode(parent,sc,x,y);
		stack_push(list,sc);
	elseif(_type == "Skin") then
		--string.format("%s")
		local url = xml_get_str(node,"url");
		local itemSkin = f_itemskin_load(url,parent,x,y);
		stack_push(list,itemSkin);
	elseif(_type == "CheckBox") then
		local ck = CheckBox:new();
		ck:setname(name);
		local label = xml_get_str(node,"label");
		
		if(#label > 0) then
			ck:setlabel(label);
		end
		
		func_addnode(parent,ck,x,y);
		stack_push(list,ck);
	elseif(_type == "ProgressBar") then
		local pb = ProgressBar:new();
		pb.name = name;
		func_addnode(parent,pb,x,y);
		stack_push(list,pb);
	elseif(_type == "ListBox")then	
		local lb  = listbox_new();
		lb.name = name;
		--print(listbox_get_container(lb));
		local v = xml_get_str(node,"value");
		local ls = func_split(tostring(v),",");
		
		--print("["..tostring(v).."]"..#ls);
		local _len = #ls;
		local n;
		for n = 0,_len-1 do
			--print(ls[n+1].."**");
			listbox_add(lb,ls[n+1]);
		end

		func_addnode(parent,lb,x,y);
		stack_push(list,lb);
	end
	--print("====>",parent);
end

function skin_parse(skin)
	
	local ins = skin;
	local xml = xml_load(ins.url);

	local cnt = xml_get_length(xml);

	--print(cnt);

	local n = 0;
	for n = 0,cnt-1 do
		local node = xml_get_node_by_index(xml,n);
		local name = xml_get_str(node,"name");
		local _type = xml_get_str(node,"type");
		--print("n = ",n,name,_type);
		f_create_by_node(ins,node,name,_type);
	end
	xml_del(xml);
	
	if(ins.completeCallBack) then
		ins.completeCallBack(ins,ins.param);
	end
end

local function f_tex_complete(param)
	skin_parse(param);
end
--获取当前皮肤组件的参数
function skin_get_param(skin)
	return skin.param;
end

--根据skin组件中的name获取组件
function skin_find(skin,name)
	local list = skin.list;
	return stack_find_by_name(list,name);
end
--根据xml加载界面skin,创建成功之后回调completeCallBack
--如果completeCallBack不传递值,是不进行异步回调的
function skin_load(url,completeCallBack,param,texs)
	
	local ins = {
		type = 5;
		container = nil;--当前skin的容器
		customParent = nil;--自定义一个父对象
		--偏移坐标
		ox = 0;
		oy = 0;
		
		list = nil;--栈链表
		url = nil;--xml url
		completeCallBack = nil;--加载完成回调
		
		param = nil;
	};
	ins.url = string.format("\\resource\\%s",url);
	
	ins.list = stack_new();
	
	if(completeCallBack) then 
		ins.completeCallBack = completeCallBack;
		ins.param = param;
		loadtexs(texs or "gundi.png;checkbox.png;smallbtn.png",f_tex_complete,ins);
	end
	return ins;
end

local function f_delAll(n)
	func_dispose(n);
end	
--销毁皮肤组件
function skin_dispose(skin)
	--销毁皮肤组件
	stack_foreach(skin.list,f_delAll,nil,true);
	
	
	stack_del(skin.list);
	func_clearTableItem(skin);
end

function skin_set_pos(skin,x,y)
	local list = skin.list;
	local node = stack_find_by_index(list,0);
	
	if(node and node.type == UI_TYPE.Panel) then
		alert_set_pos(node,x,y);
	else
		func_error("================= skin_set_pos 未找到节点");
	end
end