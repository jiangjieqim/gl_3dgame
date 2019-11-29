NSkin = {
	xmlurl,--xml地址
	list,-- stack
}
NSkin.__index = NSkin;
setmetatable(NSkin, Base);

function NSkin:new()
	local self = Base:new();
	setmetatable(self, NSkin);
	self:settype(5);
	return self;
end
local function f_delAll(n)
	func_dispose(n);
end	
function NSkin:dispose()
	local skin = self;
	--销毁皮肤组件
	stack_foreach(skin.list,f_delAll,nil,true);
	
	stack_del(skin.list);
	func_clearTableItem(skin);
	--ENGINE_EVENT_COMPLETE
end



local function f_sort(_pre,_next)	
	local t1 = xml_get_str(_pre,"type");
	local t2 = xml_get_str(_next,"type");
		
	if(t1 == "ListBox" and t2 ~= "ListBox")then
		return 1;
	end
	return -1;
end

local function f_get_parent(list,node)
	local name = xml_get_str(node,"parent");
	if(name~=nil) then
		--没有parent属性的xml row作为父对象
		local p	 = stack_find_by_name(list,name);
		local n = p:get_container();
		return n;
	end
end

local function f_create_by_node(skin,node)
	local name = xml_get_str(node,"name");
	local _type =  xml_get_str(node,"type");
	
	if(name == nil) then
		name = func_create_name(_type);--没有命名的时候,默认构造一个名字
		--print("newname:"..name);
	end
	
	if(skin.list == nil) then
		skin.list = stack_new();--构造一个栈
	end
	
	local list = skin.list;
	local parent = f_get_parent(skin.list,node);
--[[	
	if(skin.customParent ~= nil) then
		--使用自定义的父容器对象
		parent = skin.customParent;
	else
		--使用非自定义的父容器的时候,找到该组件的父对象
		parent = f_get_parent(list,node);
	end
--]]	
	--skin.container = parent;
	
	local x = xml_get_float(node,"x");
	local y = xml_get_float(node,"y");
	--print("构造对象=============>",x,y,"name=",name,_type);
	
	--x = x + skin.ox or 0;
	--y = y + skin.oy or 0;
	
	local child = nil;
	
	if(_type == "Panel") then

		func_error("不支持Panel类型!");
		
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
		
	elseif(_type == "NPanel") then
		local width = xml_get_float(node,"width");
		local height = xml_get_float(node,"height");

		local np = NPanel:new(width,height);
		np:setname(name);
		local center = xml_get_float(node,"center");
		if(center == 1) then 
			np:enable_center(true);
			np:setDrag(true);
			np:center();
		end
		stack_push(list,np);
	
	elseif(_type == "Label") then
		local label =  label_create();
		
		--print(string.format("name:[%s]%d",name,#name));
		label.name = name;
		local str = xml_get_str(node,"label");
		label_set_text(label, str);
		
		child = label;
		
		--func_addnode(parent,label,x,y);
		--stack_push(list,label);
	elseif(_type == "Button") then
		local w = xml_get_float(node,"w");
		local h = xml_get_float(node,"h");
		local btn = btn_create(0,0,w,h);
		btn.name = name;

		local str = xml_get_str(node,"label");
		if(str~="")then
			btn_label(btn,str);
		end
		child = btn;
		--func_addnode(parent,btn,x,y);
		--stack_push(list,btn);
		
	elseif(_type == "ScrollBar") then
		local sc = scrollBar_new();
		sc.name = name;
		child = sc;
		--func_addnode(parent,sc,x,y);
		--stack_push(list,sc);
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
		
		--func_addnode(parent,ck,x,y);
		--stack_push(list,ck);
		child = ck;
	elseif(_type == "ProgressBar") then
		local pb = ProgressBar:new();
		pb.name = name;
		--func_addnode(parent,pb,x,y);
		--stack_push(list,pb);
		child = pb;
	elseif(_type == "ListBox")then	
		local lb  = listbox_new();
		lb.name = name;
		local v = xml_get_str(node,"value");
		local ls = func_split(tostring(v),",");
		
		--print("["..tostring(v).."]"..#ls);
		local _len = #ls;
		local n;
		for n = 0,_len-1 do
			--print(ls[n+1].."**");
			listbox_add(lb,ls[n+1]);
		end

		--func_addnode(parent,lb,x,y);
		--stack_push(list,lb);
		
		child = lb;
		
	elseif(_type == "Input") then
		local _in = Input:new();
		_in.name = name;
		--_in:setbg();
		--func_addnode(parent,_in,x,y);
		--stack_push(list,_in);
		
		child = _in;
	elseif(_type == "Image")then
	
		--	<ui type="Image" x="0" y="140" w="128" h="14" value="checkbox.png" linestlye="0" parent="1"/>

		local w = xml_get_float(node,"w");
		local h = xml_get_float(node,"h");
		local img = Image:new(w,h);
		img.name = name;
		local v = xml_get_str(node,"value");
		img:seticon(v);
		if(xml_get_float(node,"linestlye") == 1) then
			img:linestlye();
		end
		--func_addnode(parent,img,x,y);
		--stack_push(list,img);
		
		child = img;
	elseif(_type == "Shape") then
		local w = xml_get_float(node,"w");
		local h = xml_get_float(node,"h");
		local shape = Shape:new(true,w,h);
		
		local r =  xml_get_str(node,"r");
		local g =  xml_get_str(node,"g");
		local b =  xml_get_str(node,"b");
		shape:setcolor(r,g,b);
		
		--shape:set_size(20,20);
		--shape:set_height(75);
		
		
		shape:setname(name);
		child = shape;
	elseif(_type == "NScrollBar")then
		local nb = NScrollBar:new();	
		--nb:set_pos(x,y);
		nb:setname(name);
		child = nb;
	elseif(_type == "NListBox")then
		local nb = NListBox:new();	
		nb:setname(name);
		child = nb;
	end
	--****************************************
	if(child~=nil) then
		if(parent == nil) then
			func_error("parent is nil!");
		end
		
		func_addnode(parent,child,x,y);
		stack_push(list,child);
	end
	
	--print("====>",parent);
end
local function f_skin_parse(self)
	
	local xml = xml_load(self.xmlurl);

	local cnt = xml_get_length(xml);


	--local n = xml_get_node(xml,"name","rx_sc11");
	--print(n);
	
	
	--print(cnt);

	local n = 0;
	
	--这里要进行一个排序 比如讲ListBox放到父容器的最上层
	
	local _l = stack_new();

	for n = 0,cnt-1 do
		local node = xml_get_node_by_index(xml,n);
		stack_push(_l,node);
	end
		
	_l = stack_sort(_l,f_sort);
	
	for n = 0,cnt-1 do
		local node = stack_find_by_index(_l,n);
		f_create_by_node(self,node);
	end	
	
	stack_del(_l);
	xml_del(xml);
	
	--[[if(ins.completeCallBack) then
		ins.completeCallBack(ins,ins.param);
	end--]]
end
local function f_tex_complete(self)
	--skin_parse(param);
	f_skin_parse(self);
	--print(self);
	evt_dispatch(self,ENGINE_EVENT_COMPLETE,self);
end
--加载skin	 "gundi.png;checkbox.png;smallbtn.png"
function NSkin:load(xmlurl,texs)
	if(texs == nil) then
		func_error();
		return;
	end
	self.xmlurl = xmlurl;
	loadtexs(texs,f_tex_complete,self);
end
