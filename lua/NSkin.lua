NSkin = {
	-- xmlurl,--xml��ַ ������xml�ı���ʽ������
	-- list,-- �洢�ڵ��б����ջ
	-- namemap,--�������ִ洢skin�����tabel
	-- xml,--xml����,
	-- panel=nil;--��ǰ��NPanel����
	-- isLoaded = nil;--Ƥ���Ƿ��Ѿ����������
}
NSkin.__index = NSkin;
setmetatable(NSkin, Base);

--�Ƿ���xml����
local function isXmlUrl(url)
	local a = string.find(url or "","<");
	return a == nil;
end
--�������
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
		
		n:dispose();--ɾ�����
        
		
	elseif(_type == UI_TYPE.ListBox) then	
		listbox_del(n);
	else
		func_error();
	end
end

function NSkin:new()
	local self = Base:new();
	setmetatable(self, NSkin);
	self.isLoaded = false;--�Ƿ�skin�Ѿ����������
	self.vis = true;
	self:settype(UI_TYPE.Skin);	--5
	return self;
end
local function f_delAll(n)
	func_dispose(n);
end

--�������
function NSkin:dispose()
	local skin = self;
	--����Ƥ�����
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
--��ȡ�б��еĸ�����
local function f_get_parent(list,node)
	local name = xml_get_str(node,"parent");
	if(name~=nil) then
		--û��parent���Ե�xml row��Ϊ������
		local p	 = stack_find_by_name(list,name);
		local n = p:get_container();
		return n;
	end
end

local function f_create_by_node(skin,node,myParent,offsetx,offsety)
	local name = xml_get_str(node,"name");
	local _type =  xml_get_str(node,"type");
	
	if(name == nil) then
		name = func_create_name(_type);--û��������ʱ��,Ĭ�Ϲ���һ������
		--print("newname:"..name);
	end
	
	if(skin.list == nil) then
		skin.list = stack_new();--����һ��ջ
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
		--ʹ���Զ���ĸ���������
		parent = skin.customParent;
	else
		--ʹ�÷��Զ���ĸ�������ʱ��,�ҵ�������ĸ�����
		parent = f_get_parent(list,node);
	end
--]]	
	--skin.container = parent;
	
	
	--print("�������=============>",x,y,"name=",name,_type);
	
	--x = x + skin.ox or 0;
	--y = y + skin.oy or 0;
	
	local child = nil;
	
	if(_type == "Panel") then

		func_error("��֧��Panel����!");
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
	    func_error("��ʵ��!");
	
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
			func_error("Image������0");
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
	��3������ֻ���ڽ���skin���͵����ݵ�ʱ�����
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
	
	--����Ҫ����һ������ ���署ListBox�ŵ������������ϲ�
	
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
	self.isLoaded = true;
	-- print("skin is load end!");
	evt_dispatch(self,ENGINE_EVENT_COMPLETE,self);--�������
end

--Ƥ���Ƿ��Ѿ����������
function NSkin:isSkinLoaded()
	return self.isLoaded;
end

--[[
0.  xmlurl ������xml���ı�����,������xml����
1.	����skin	 "gundi.png;checkbox.png;smallbtn.png"
2.	texs == nil��ʱ�����첽����,ֱ������Ƥ��
--]]
function NSkin:load(xmlurl,texs)
	-- if(texs == nil) then
	-- 	print("=======================> error");
	-- 	func_error();
	-- 	return;
	-- end

	-- print(texs);

	self.xmlurl = xmlurl;
	if(texs~=nil) then
		loadtexs(texs,f_tex_complete,self);
	else
		f_tex_complete(self);
	end
end

--����
function NSkin:center()
	local np = self.panel;
	np:center();
end
--��ȡn�ڸ������еľֲ�����
function NSkin:getLocalPos(n)
	local px,py = self.panel:get_pos();
	local x,y = n:get_pos();
	return x-px,y-py;
end


--��ȡ��panel����
function NSkin:get_panel()
	return self.panel;
end
--����Skin�е�skin�����ʱ������,ע��˷���ֻ������Skin���ڲ�item���,������һ����ͬ��Parent
function NSkin:synload(xmlurl,myParent,offsetx,offsety)
	self.xmlurl = xmlurl;
	f_skin_parse(self,myParent,offsetx,offsety);
end

--��������
function NSkin:set_pos(x,y)
	local list = self.list;
	local node = stack_find_by_index(list,0);--Ĭ��ȡջ��0�������е�����
	node:set_pos(x,y);
end

--��ȡ����
function NSkin:get_pos()
	local list = self.list;
	local node = stack_find_by_index(list,0);--Ĭ��ȡջ��0�������е�����
	return node:get_pos();
end

--���������ҵ��������
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
		func_error("δʵ������:".._type);
	end
end
--������ʾ����Ƥ��
function NSkin:visible(v)
	local l = self.list;
	self.vis = v;
	--local node = stack_find_by_index(list,0);--Ĭ��ȡջ��0�������е�����
	--node:visible(v);
	local list = l.list;
	local cnt = l.cnt;
	for i=0,cnt-1,1 do
		local node = list[i];
		--[[if(node.name == name)then
			return node;
		end--]]

		f_node_visible(node,v);
	end
end

function NSkin:is_visible()
	--Skin�Ƿ���ʾ��
	return self.vis;
end

--[[
--����ʾ��

--************************************************************************

local function f_cpmlete(self)
	print("���ؽ���");
	
	self:set_pos(10,20);--��������

	local skin1 = self:find("skin1");
	local label = skin1:find("2");
	--print(skin1,label);
	--label_set_text(label, "x*");

	--self:dispose();--����
end

local nskin = NSkin:new();
evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmlete);
nskin:load("\\resource\\crl.xml","gundi.png;checkbox.png;smallbtn.png");

--************************************************************************

--]]
