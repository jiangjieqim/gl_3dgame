NSkin = {
	-- xmlurl,--xml地址 或者是xml文本格式的数据
	-- list,-- 存储节点列表的链栈
	-- namemap,--根据名字存储skin组件的tabel
	-- xml,--xml引用,
	-- panel=nil;--当前的NPanel引用
}
NSkin.__index = NSkin;
setmetatable(NSkin, Base);

--是否是xml链接
local function isXmlUrl(url)
	local a = string.find(url or "","<");
	return a == nil;
end
--销毁组件
local function func_dispose(n)
   
	local _type = n.type;
	if(_type == UI_TYPE.ScrollBar) then
		scrollBar_del(n);
--	elseif(_type == UI_TYPE.Button)then
--		btn_dispose(n);
	elseif(_type == UI_TYPE.CheckBox
		or _type == UI_TYPE.ProgressBar 
		or _type == UI_TYPE.Input
		or _type == UI_TYPE.Image
		or _type == UI_TYPE.Shape
		or _type == UI_TYPE.NScrollBar
		or _type == UI_TYPE.NListBox
		or _type == UI_TYPE.NPanel
		or _type == UI_TYPE.Skin
		or _type == UI_TYPE.NLabel
		or _type == UI_TYPE.NButton
										)then
		
		n:dispose();--删除组件
        
		
	elseif(_type == UI_TYPE.ListBox) then	
		listbox_del(n);
	else
		func_error();
	end
end

function NSkin:new()
	local self = Base:new();
	setmetatable(self, NSkin);
	
	self.vis = true;
	self:settype(UI_TYPE.Skin);	--5
	return self;
end
local function f_delAll(n)
	func_dispose(n);
end

--销毁组件
function NSkin:dispose()
	local skin = self;
	--销毁皮肤组件
	stack_foreach(skin.list,f_delAll,nil,true);
	
	stack_del(skin.list);

	if(skin.namemap) then
		func_clearTableItem(skin.namemap);
	end
	
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

local function f_create_by_node(skin,node,myParent,offsetx,offsety)
	local name = xml_get_str(node,"name");
	local _type =  xml_get_str(node,"type");
	
	if(name == nil) then
		name = func_create_name(_type);--没有命名的时候,默认构造一个名字
		--print("newname:"..name);
	end
	
	if(skin.list == nil) then
		skin.list = stack_new();--构造一个栈
	end
	local x = xml_get_float(node,"x");
	local y = xml_get_float(node,"y");
	local list = skin.list;
	local parent = nil;
	--*********************************************************
	if(myParent) then
		parent = myParent;
		x = x + offsetx;
		y = y + offsety;
	else
		parent = f_get_parent(skin.list,node);
	end
	--*********************************************************
	
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
	
	
	--print("构造对象=============>",x,y,"name=",name,_type);
	
	--x = x + skin.ox or 0;
	--y = y + skin.oy or 0;
	
	local child = nil;
	
	if(_type == "Panel") then

		func_error("不支持Panel类型!");
--[[		
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
--]]		
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
		else
			np:set_pos(x,y);
		end
		stack_push(list,np);

		skin.panel = np;
		
	elseif(_type == "NLabel") then
		local str = xml_get_str(node,"label");
		local label = NLabel:new();

		label:set_text(str or "label");
		
		label:setname(name);
		child = label;
		
	elseif(_type == "Button") then
--		local w = xml_get_float(node,"w");
--		local h = xml_get_float(node,"h");
--		local btn = btn_create(0,0,w,h);
--		btn.name = name;

--		local str = xml_get_str(node,"label");
--		if(str~="")then
--			btn_label(btn,str);
--		end
--		child = btn;
	    func_error("不实现!");
	
	elseif(_type == "NButton") then
		local w = xml_get_float(node,"w");
		local h = xml_get_float(node,"h");
		local url = xml_get_str(node,"url");
		local label = xml_get_str(node,"label");
		local btn = Button:new(w,h,url);
		btn:setname(name);
		if(label~=nil) then
			btn:set_label(label);
		end
		btn:set_pos(x,y);
		
		local rotatez = xml_get_str(node,"rotatez");
		if(rotatez) then
			btn:setRotateZ(rotatez);
		end
		
		child = btn;
	elseif(_type == "ScrollBar") then
		local sc = scrollBar_new();
		sc.name = name;
		child = sc;
	
	elseif(_type == "Skin") then
		--string.format("%s")
		local url = xml_get_str(node,"url");
		--local itemSkin = f_itemskin_load(url,parent,x,y);
		local nskin = NSkin:new();
		nskin:setname(name);
		nskin:synload(url,parent,x,y);
		stack_push(list,nskin);
	elseif(_type == "CheckBox") then
		local ck = CheckBox:new();
		ck:setname(name);
		local label = xml_get_str(node,"label");
		
		if(label and #label > 0) then
			ck:setlabel(label);
		end
	
		child = ck;
	elseif(_type == "ProgressBar") then
		local pb = ProgressBar:new();
		pb:setname(name);
		
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

	    
		
		child = lb;
		
	elseif(_type == "Input") then
		local _in = Input:new();
		_in:setname(name);
        
		child = _in;
	elseif(_type == "Image")then
	
		--	<ui type="Image" x="0" y="140" w="128" h="14" value="checkbox.png" linestlye="0" parent="1"/>

		local w = xml_get_float(node,"w");
		local h = xml_get_float(node,"h");
		
		if(w == 0 or h == 0) then
			func_error("Image宽或高是0");
		end
		local img = Image:new(w,h);
		img:setname(name);
		local v = xml_get_str(node,"value");
		img:seticon(v);
		if(xml_get_float(node,"linestlye") == 1) then
			img:linestlye();
		end
	    
		
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
		
		--print("name:",name,child);
		if(name) then
			if(skin.namemap == nil) then
				skin.namemap = {};
			end
			skin.namemap[name] = child;
		end
		
		stack_push(list,child);
	end
	
	--print("====>",parent);
end

--[[

	myParent,offsetx,offsety
	这3个参数只有在解析skin类型的数据的时候才用
--]]
local function f_skin_parse(self,myParent,offsetx,offsety)
	
	local xml = Xml:new();
	
	if(isXmlUrl(self.xmlurl))then
		xml:load(self.xmlurl);
	else
		xml:loadstr(self.xmlurl);
	end
	local cnt = xml:len();

	local n = 0;
	
	--这里要进行一个排序 比如讲ListBox放到父容器的最上层
	
	local _l = stack_new();

	for n = 0,cnt-1 do
		local node = xml:get_index(n);--xml_get_node_by_index(xml,n);
		stack_push(_l,node);
	end
		
	_l = stack_sort(_l,f_sort);
	
	for n = 0,cnt-1 do
		local node = stack_find_by_index(_l,n);
		f_create_by_node(self,node,myParent,offsetx,offsety);
	end	
	
	stack_del(_l);
	--xml_del(xml);
	xml:dispose();
	-- self:visible(false);
end

local function f_tex_complete(self)
	f_skin_parse(self);
	evt_dispatch(self,ENGINE_EVENT_COMPLETE,self);
end

--[[
0.  xmlurl 可能是xml纯文本数据,可能是xml链接
1.	加载skin	 "gundi.png;checkbox.png;smallbtn.png"
2.	texs == nil的时候不用异步加载,直接生成皮肤
--]]
function NSkin:load(xmlurl,texs)
	if(texs == nil) then
		func_error();
		return;
	end

	-- print(texs);

	self.xmlurl = xmlurl;
	if(texs~=nil) then
		loadtexs(texs,f_tex_complete,self);
	else
		f_tex_complete(self);
	end
end

--居中
function NSkin:center()
	local np = self.panel;
	np:center();
end

--加载Skin中的skin组件的时候是用,注意此方法只适用于Skin的内部item组件,它们有一个共同的Parent
function NSkin:synload(xmlurl,myParent,offsetx,offsety)
	self.xmlurl = xmlurl;
	f_skin_parse(self,myParent,offsetx,offsety);
end

--设置坐标
function NSkin:set_pos(x,y)
	local list = self.list;
	local node = stack_find_by_index(list,0);--默认取栈中0号索引中的数据
	node:set_pos(x,y);
end

--获取坐标
function NSkin:get_pos()
	local list = self.list;
	local node = stack_find_by_index(list,0);--默认取栈中0号索引中的数据
	return node:get_pos();
end

--根据名字找到组件引用
function NSkin:find(name)
	if(self.list~=nil) then
		return stack_find_by_name(self.list,name);
	end
end

local function f_node_visible(n,v)
	local _type = n.type;
	if(	   _type == UI_TYPE.NPanel
		or _type == UI_TYPE.NLabel
		or _type == UI_TYPE.NButton 
		or _type == UI_TYPE.NScrollBar
		or _type == UI_TYPE.NListBox
		or _type == UI_TYPE.CheckBox
		or _type == UI_TYPE.ProgressBar
		or _type == UI_TYPE.Shape
		or _type == UI_TYPE.Input
		or _type == UI_TYPE.Image
		) then
		
		n:visible(v);
	elseif(_type == UI_TYPE.Button) then
		btn_visible(n,v);
	else
		func_error("未实现类型:".._type);
	end
end
--设置显示隐藏皮肤
function NSkin:visible(v)
	local l = self.list;
	self.vis = v;
	--local node = stack_find_by_index(list,0);--默认取栈中0号索引中的数据
	--node:visible(v);
	local list = l.list;
	for i=0,l.cnt-1,1 do
		local node = list[i];
		--[[if(node.name == name)then
			return node;
		end--]]

		f_node_visible(node,v);
	end
end

function NSkin:is_visible()
	--Skin是否显示着
	return self.vis;
end

--[[
--调用示例

--************************************************************************

local function f_cpmlete(self)
	print("加载结束");
	
	self:set_pos(10,20);--设置坐标

	local skin1 = self:find("skin1");
	local label = skin1:find("2");
	--print(skin1,label);
	--label_set_text(label, "x*");

	--self:dispose();--销毁
end

local nskin = NSkin:new();
evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmlete);
nskin:load("\\resource\\crl.xml","gundi.png;checkbox.png;smallbtn.png");

--************************************************************************

--]]
