local DEBUG = 0;--Ĭ�ϵ�ģʽ0:����,1�ر�

FLAGS_RENDER_BOUND_BOX	=	0x01;
FLAGS_DRAW_RAY_COLLISION	=	0x02;	--�������ߺ���	(�Ƿ�������߾�̬��Χ��,���ڲ��� �鿴���߰�Χ��),��ʹû������FLAGS_RAY,��ôҲ�ǿ��Ի������ߺ��ӵ�,�������������鿴
FLAGS_LINE_RENDER			=	0x04;	--�Ƿ���ʾ��Ⱦ�߿�
FLAGS_OUTLINE				=	0x08;	--�Ƿ���ʾ��Ե����
FLAGS_RAY					=	0x10;	--16�Ƿ��������ʰȡ����(��ֻ����3dʰȡ,2dʰȡ����������)
FLAGS_VISIBLE				=	0x20;	--32��ʾ��������ģ��,(�Ƿ������Ⱦ������)
FLAGS_RENDER_DRAWSKELETON	=	64;		--��Ⱦ�����ڵ�(md5ģ�ͲŻ���ʾ)
FLAGS_ANIM_ADAPTIVE		=	128;		--�Ƿ���ݹؼ�֡��֡������ؼ�֡�Ķ���(�Ƿ�����fps 1,����  0,������)
FLAGS_GLSL_OUTLINE		=	256;		--��GLSLʵ�ֵ�������
FLAGS_DRAW_PLOYGON_LINE	=	512;		--�ڵ�һ���������������߿���Ⱦ(�̶�����ģʽ����GL_LINE)
FLAGS_BUTTON_EFFECT		=	1024;	--���ð�ť��Ч(���е���¼���ǰ���²Ż��и���Ч)
FLAGS_DRAW_NORMAL 		= 	2048;	--����
FLAGS_DRAG				=	4096;	--�Ƿ����ק
FLAGS_DRAW_PLOYGON_POINT= 	8192;	--��ģʽ
FLAGS_DISABLE_CULL_FACE	=	16384;	--������ʾ˫��
FLAGS_REVERSE_FACE		=	32768;	--��ת��Ⱦ��,ֻ��������˫����Ⱦ����Ч
--[[
--//�ļ����Ͷ���
TYPE_OBJ_FILE = 0--	//obj����
TYPE_MD5_FILE =1 --//md5����
TYPE_MD2_FILE =2 --//md2����
TYPE_SPRITE_FLIE =3--//UI���������е�sprite
--TYPE_TEXT_FILE	=4	--//�ı�����
TYPE_OBJ_VBO_FILE=	5--//VBO�ļ�����

SUFFIX_OBJ ="obj"
SUFFIX_MD5MESH ="md5mesh"
SUFFIX_MD2 ="md2"

--]]

------------------------------------------------------------
EVENT_RAY_PICK = 1						--ʰȡ����ص�
EVENT_ENGINE_RENDER_3D =100				--������Ⱦ�ص�
EVENT_ENGINE_KEYBOARD  =101				--ȫ�ּ����¼�
EVENT_ENGINE_RESIZE	   =102				--resize�¼�

-- EVENT_ENGINE_BASE_UPDATE	 =  102		--base�����¼�
EVENT_ENGINE_BASE_END		 =  103		--base�����¼�
EVENT_ENGINE_SPRITE_CLICK = 104
EVENT_ENGINE_SPRITE_CLICK_DOWN = 105
EVENT_ENGINE_SPRITE_CLICK_MOVE = 106    --click move
EVENT_ENGINE_TEX_LOAD_COMPLETE = 108	--������ؽ���
EVENT_ENGINE_COMPLETE		   = 109	--����¼�

CUST_LUA_EVENT_SPRITE_FOCUS_CHANGE =110 --��lua�㷢�ͽ���仯
CUST_LUA_EVENT_INPUT_CHANGE = 111		--input�������ݷ����仯

EVENT_TIMER = 201--��ʱ��

ENGINE_EVENT_COMPLETE = 1000;	--�����¼�

--��ֵö��
 KEY_ESC = 27
 KEY_1 = 49
 KEY_2 = 50
 KEY_A = 97
 KEY_B = 98
 KEY_C = 99
 KEY_D = 100
 KEY_I = 105

--��������
UI_TYPE = {
	Label = 1,
	Button =2,
	ScrollBar = 3,
	Panel = 4,
	Skin = 5,
	CheckBox = 6,--ѡ�����
	ProgressBar = 7,--������
	ListBox = 8,--�����б�
	Input = 9,--�������
	Image = 10,--image���
	Shape = 11,--shape���
	NScrollBar = 12,--���������
	NListBox = 13,--�б�
	NLabel = 14,--Label
	NPanel = 15,
	NButton = 16,
};

--�������ͻ�ȡ��������
function func_getNameByUIType(t)
	local u = UI_TYPE;
	local a = {
		[u.Label] = "Label",
		[u.Button] = "Button",
		[u.ScrollBar] = "ScrollBar", 
		[u.Panel] = "Panel",
		[u.CheckBox] = "CheckBox",
		[u.ProgressBar] = "ProgressBar",
		[u.ListBox] = "ListBox",
		[u.Image] = "Image",
		[u.Shape] = "Shape",
		[u.NScrollBar] = "NScrollBar",
		[u.NListBox] = "NListBox",
		[u.NLabel] = "NLabel",
		[u.NPanel] = "NPanel",
		[u.NButton] = "NButton",
	}
	return a[t];
end





require("stack")
require("xml")	--xml���
require("vec3")	--�Զ�����ѧ��
require("nstack");--��ջ

require("evt")	--�¼�������


--ת����һ����ַ
--"=table: 0082E758"  ===>8578904
local function getDddress(value)
	local len = string.len("table:  ")
	local a = string.len(tostring(value))
	local v = string.sub(tostring(value),len,a)
	local s = tonumber('0x'..v);
	return s;
end

--��"table: ff"ת��Ϊnumber
local function f_get_address(value)
	return getDddress(value);
end


--local nameKey = -1;
--����һ������
local function getName(suffix)
    local str = tl_newName(suffix);
    return str;
end
--[[
	����һ�����ü�������һ�����ݶ���

--]]
function func_load_material(url)
	
	local suffix = get_suffix(url);
	--print(s);
	local result;
	local xml = Xml:new();--xml_load(url);
	xml:load(url);
	if(suffix == "mat") then
		local node = xml:get_index(0);--xml_get_node_by_index(xml,0);
		local shader = xml_get_str(node,"shader");
		
		if(shader == nil) then
			func_error("shader is nil,"..url);
		else			
			if(shader == "") then
				func_error(url);
			end
			
			result = createMaterial(shader,
				xml_get_str(node,"tex0"),
				xml_get_str(node,"tex1"),
				xml_get_str(node,"tex2"),
				xml_get_str(node,"tex3"),
				xml_get_str(node,"tex4"),
				xml_get_str(node,"tex5"),
				xml_get_str(node,"tex6"),
				xml_get_str(node,"tex7")
			);
			local cb = xml_get_float(node,"cb");
			if(cb~=0) then
				tmat_setCallBack(result,cb);--������ɫ���ص�
			end

			if(shader == "diffuseStateBox") then
				local _lineColor =    xml_get_str(node,"_lineColor");
				--print(_lineColor);
				--local arr=func_split(_lineColor,",");

				glsl_set(result,string.format("_lineColor,%s",_lineColor));
			end
		end
	end
	
--	xml_del(xml);
	xml:dispose();
	return result;
end

--ɾ��table�����Ԫ��(�������еı�Ԫ������)
function func_clearTableItem(point)
	for k, v in pairs(point) do
		point[k] = nil
	end
end
--������ӡ��
local function printTable(t)
	func_print("start print table: "..tostring(t),0xffff00)
	--print("start print table: "..tostring(t))
	for key, value in pairs(t) do      
		--print('key=['..key..']'..'value['..value..']')
		local s = tostring(value)
		if(tonumber(value)) then
			
			--ת��Ϊ16��������
			s =  string.format("%#x",tonumber(value)).."\t("..value..")"
		end
		
		print(tostring(key).."\t\t\t="..s)
		--print(value)
		--print("-------------")
		
		--print(string.format("%#x",tonumber(value)))
		
		--print(tonumber(value))
	end 
	func_print("end print table:   ["..tostring(t).."]",0xffff00)
	
end

--��ȡ��������ʱ��
function func_get_longTime()
	return get_attr(nil,"get_longTime");
end

--��ӡһ������ɫ����־������̨
function func_print(s,c)
	if(DEBUG == 1)then
		c = c or 0xffff00
		--c = c or 0;
		
		--�����̨�������ɫ���ı���־
		dofunc("LogPut",string.format("lua: %s\n",s),c);
		
		--��ӡ��ջ
		--dofunc("LogPut",string.format("lua stack: %s\n",debug.traceback()),0x00ff00);
	end
end
---�����쳣��ʱ����һ�����
function func_error(msg,func)
	msg = msg or "";
	local s = ''
	
	s = 'lua error '..msg..''
	--func_print(s,0xff0000)
	--func_print('lua error:'..s,0xff0000)
	
	func_print(s,0xff0000);
	
	s = ''
	if(func) then
		--print ('line num:'..debug.getinfo(1).currentline..',file name:'..debug.getinfo(1).name )
		local info = debug.getinfo(func)
		for k,v in pairs(info) do
			
				--ȫ����ӡ
				--print(k, ':', info[k])
				
				--linedefined				
				--short_src
				--s = s..tostring(k)..':\t\t'..tostring(info[k])..'\n'
				
				if(tostring(k) == 'linedefined') then
					s = s..'lineNum:'..tostring(info[k])..'\t'
				end
				
				if (tostring(k) == 'short_src') then
					s = s..'file:'..tostring(info[k])..'\t'
				end
		end
	end
	func_print(s,0xff0000);
	--printTable(debug.getinfo(1))
	--print(debug.traceback())
	
	func_print(debug.traceback(),0xff0000);
	if(DEBUG == 0) then
		print(debug.traceback());--��û�����������־��ʱ��,����̨Ĭ�������־.
	end
	assert(nil,s);
	
	--error(msg)
    --print ( debug.getinfo(1).name )
end

--[[
	����sprite���������ڸ����������
--]]
function func_set_local_pos(p,x,y)
	local pos = x..","..y;
	change_attr(p,"sprite_set_self_pos",pos);
end

--[[
	��child��ӵ�parent��
--]]
function func_addchild(parent,child,x,y)
	x = x or 0;
	y = y or 0;
	
	if(parent==nil) then
		func_error();
	end
	
	sprite_addChild(parent,child);
	func_set_local_pos(child,x,y);
end

--�ڴ����Cģ���
function func_gc()
	change_attr(nil,"gc");
	--print('gc')
end

--lua��GC
function func_lua_gc(key)
	local old =  collectgarbage("count");
	--func_print("==> GC֮ǰ LUA VMʹ�õ����ڴ���:"..(collectgarbage("count")*1024).." bytes");
	collectgarbage("collect");
	local n = collectgarbage("count");
	func_print((key or "").." GC��� LUA VMʹ�õ����ڴ���:\t"..(n/1024).." mb\t("..n.."kb) gc��["..((old -  n)*1024).."]bytes",0x00ff00);
end

--��������
function func_create_name(suffix)
	return getName(suffix);
end

--Ϊsprite������ͼ
function func_setIcon(sprite,url)
	--��ȡһ��atalsͼ��,û��ͼ���Ľ����Ǻ�ɫ��
	local atals = JEngine:getIns():get_atals();
	if(atals) then
		sprite_bindAtals(sprite,atals);

		sprite_texName(sprite,url);
	end
end

--��ȡsprite�Ŀ��
function func_get_sprite_size(o)
	return get_attr(o,"spriteSize")	
end

--����sprite�Ŀ��
function func_set_sprite_size(o,w,h)
	change_attr(o,"sprite_resize",string.format('%d,%d',w,h));
end

--��ȡ���ʰȡ��sprite�������
function func_get_sprite_mouse_xy(o)
	local x , y=get_attr(o,"spriteLocalXY");
	return x,y
end

--�����ļ�����һ���ַ���
function func_loadfile(url)
    return change_attr(nil,"loadfile",url);
end


--�ַ����ָ��table
function func_split(str, delimiter)
    delimiter = delimiter or ","
    if str==nil or string.len(str)==0 or delimiter==nil then
        return nil
    end
 
    local result = {}
    for match in (str..delimiter):gmatch("(.-)"..delimiter) do
        table.insert(result, match)
    end
    return result
end

--������tex֮��Ļص�����
function func_texloadend(n)
	sprite_set_9grid(n.sprite,n.url,3,3,3,3,JEngine:getIns():get_atals());
end

--����һ��֧�־Ź���ĵ�sprite
function func_create_grid_sprite(x,y,w,h,url,name)
	url = url or "gundi.png";
	local sprite = sprite_create_typical(name,x,y,w,h);
	loadtexs(url,func_texloadend, { sprite=sprite;url=url});
	JEngine:getIns():add(sprite);
	return sprite;
end
--�൱��addchild
function func_addnode(parent,n,x,y)
	x = x or 0;
	y = y or 0;
	local _type = n.type;
	
	if(_type == UI_TYPE.ScrollBar) then
		func_addchild(parent,scrollBar_get_container(n),x,y);
	elseif(_type == UI_TYPE.Button)then
		func_addchild(parent,btn_get_container(n),x,y);
	elseif(	
		_type == UI_TYPE.Image
		or _type == UI_TYPE.Shape
		) then
		func_addchild(parent,n.container,x,y);
	elseif(_type == UI_TYPE.ListBox) then
		func_addchild(parent,listbox_get_container(n),x,y);	
	elseif(_type == UI_TYPE.NScrollBar
			or _type == UI_TYPE.ProgressBar 
			or _type == UI_TYPE.NListBox
			or _type == UI_TYPE.NLabel
			or _type == UI_TYPE.NButton
			or _type == UI_TYPE.CheckBox
			or _type == UI_TYPE.Input

	) then
		local c = n:get_container();
		if(c == nil) then
			func_error();
		end
		
		--print(c);
		func_addchild(parent,c,x,y);	
	else
		func_error(string.format("type = %sδʵ��",tostring(_type)));
	end
end


require("base");
require("linenode");

require("label");--label�Ƕ�ftext���е�һ�η�װ

require("input");

require("loadtexs")	--��Դ������

require("cam");
require("engine")	--����1

require("fbo")	--fbo


require("shape")	--shape���
require("NPanel");

require("NListbox")

require("NScrollBar")	--���������

require("button");		--��ť���

require("image")	--image���

require("checkbox")	--checkbox���
require("progrossbar")
require("NSkin");--NSkinƤ�����

require("NUnit");
require("UnitBase");		--��ɫ��λ
require("md5unit");		--md5����

require("scrollview")	--scrollview

--[[
local tf = require("tf");
print(tf);
tf = nil;
--print(tf.get());
--]]


core = {};
local M = core;

local function f_time(data,o)
	if(o.c) then
		o.c(o.p);
	end
end

---��ȡTabel�ĳ���,�����Tabel�е����м�ֵ����
---@param tb Tabel
function M.getLen(tb)
	local n = 0;
	for k, v in pairs(tb) do
		n = n + 1;
	end
	return n;
end

---@param ms �ӳ�ms����ִ��callback
function M.setTimeout(ms,callback,param)
	local timer = timelater_new(ms);
	local o = {t=timer,c=callback,p=param};
	evt_once(timer,EVENT_TIMER,f_time,o);
	return o;
end

function M.clearTimeout(o)
	--evt_off(o.t,EVENT_TIMER,o.c);
	o.c = nil;
	o.t = nil;
end

function M.now()
	return func_get_longTime();
end
---���������Ϣ
---@param str string
function M.warning(str)
	str = string.format("warning >>>>>>>>>>>>>>> [%s] %s",str or "",debug.traceback());
	if(DEBUG == 1) then
		func_print(str,0xff00ff);
	else
		print(str);
	end
end
function M.gc()
	func_lua_gc();
end

--�Ƴ�ģ��,����е�ģ����Ҫ���¼��صĳ�ʼ����,����ʹ�øýӿ�
function M.removeRequire( preName )
    for key, _ in pairs(package.preload) do
        if string.find(tostring(key), preName) == 1 then
			print("preload remoeve preName:"..preName);
			package.preload[key] = nil;
        end
    end
    for key, _ in pairs(package.loaded) do
		if string.find(tostring(key), preName) == 1 then
			print("loaded remoeve preName:"..preName);
            package.loaded[key] = nil;
        end
    end
end

function M.debug(v)--�Ƿ���debugģʽ
	if(v == true or v == 1)then
		DEBUG=1;
		log_enable(1);
	elseif(v == false or v == 0)then
		DEBUG=0;
		log_enable(0);
	end
	print(string.format("������debug����״̬:%s",v));
end;
-- M.debug(0);

-- local function fc()
-- 	print("fc..."..func_get_longTime());
-- end

--print("xx..."..func_get_longTime());
--local o = setTimeout(1000,fc);
--clearTimeout(o);

M.UI_TYPE = UI_TYPE;
---@type JEngine
M.e = JEngine:getIns();
-- print("core init!!!");

---ΪTabel�������ַ
---@param o Tabel
function M.bindAddress(o)
	o.address=f_get_address(o);
end

---@type PluginMan
M.p = M.e:get_plugin();

M.ENUM = require("enum");
M.EVENT = require("event");
return core;