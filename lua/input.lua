local function input_create(w)
	w = w or 128
	return c_input(nil,"create",w);
end
local function input_set_pos(ptr,x,y)
	c_input(ptr, "setpos", string.format("%s,%s", x or 0, y or 0));
end

local function input_dispose(ptr)
	c_input(ptr, "dispose");
end

local function input_get_str(ptr)
	return c_input(ptr,"getstr");
end

local function input_get_container(ptr)
	return c_input(ptr,"get_container");
end

local function input_visible(ptr,v)
	local o = input_get_container(ptr);
	if(v ) then
		setv(o,FLAGS_VISIBLE);
	else
		resetv(o,FLAGS_VISIBLE);
	end
end

local function input_get_ftext(ptr)
	return c_input(ptr, "get_ftext");
end

--��ȡ��ǰ�Ľ���
local function func_get_curFocus()
    return get_attr(nil,"curFocus");
end
--������Ƿ��ڽ�����
local function input_isFocusIn(ptr)	
	if (func_get_curFocus() == input_get_container(ptr))then
		return true;
	end
	return nil;
end

local function input_clear(ptr)
	c_input(ptr,"clear");
end

--����һ���������
local function example_input(x,y)
	--��Ϊ�����ı���
	x = x or 0;
	y = y or 0;
	
	local sprite = func_create_grid_sprite(x,y,128,14);
	
	local _in = input_create();
	input_set_pos(_in,x,y);
	local function f_onkey(data)
		local key = tonumber(data);
		--print('my key = '..key);
		if(key == 13) then
			--print(input_isFocusIn(_in));
			if(input_isFocusIn(_in)) then
				print("inout str = ["..input_get_str(_in).."]");
				input_clear(_in);
			end
		elseif(key == KEY_B)then
		   
		elseif(key == KEY_ESC) then
			
		end
	end
	evt_on(_in,EVENT_ENGINE_KEYBOARD,f_onkey);

	--input_dispose(_in);
end

--��������
Input = {
	_in,
	img,--�̶�Shape
	timer,
};

Input.__index = Input;
setmetatable(Input, Base);

local function f_timer(data,self)
	local w,h = self:get_get_wordpos();
	local img = self.img;		
	img:set_pos(w,0);
	img:visible(not img:is_visible());
end

local function f_onInputChange(data,self)
	f_timer(nil,self);
	--print(data,self);
end

local function f_onFocusChange(data,self)
	--print(data,param);
	if(data == 1) then
		--print("���뽹��");
		evt_on(self.timer,core.EVENT.TIMER,f_timer,self);
	else
		--print("�뿪����");
		evt_off(self.timer,core.EVENT.TIMER,f_timer);
		self.img:visible(false);
	end
end
--�����ı��ĳߴ�
function Input:get_get_wordpos()
	local txt = input_get_ftext(self._in);
	return ftext_get_wordpos(txt);
end

function Input:new()
	local self = Base:new();
	self:settype(9);
	setmetatable(self,Input);
	
	local _in = input_create();

	self._in = _in;
	local container = input_get_container(_in);
	evt_on(_in,CUST_LUA_EVENT_SPRITE_FOCUS_CHANGE,f_onFocusChange,self);
	
	evt_on(_in,CUST_LUA_EVENT_INPUT_CHANGE,f_onInputChange,self);

	
	
--[[	if(hasBg) then
		self.bg = Shape:new(true,128,14);
		func_addnode(self.container,self.bg);
	end--]]
	
	local img = Shape:new(true,1,14);
	self.img = img;
	--img:seticon("gundi.png");
	func_addnode(container,img);
	img:setcolor(1,1,1);
	img:visible(false);
	
	local timer = timelater_new(1000);
	self.timer = timer;
	--evt_on(timer,EVENT_TIMER,f_timer,self);

	return self;
end
function Input:get_container()
	--func_error();
	return input_get_container(self._in);
end
function Input:dispose()
	evt_off(self._in,CUST_LUA_EVENT_SPRITE_FOCUS_CHANGE,f_onFocusChange);
	evt_off(self._in,CUST_LUA_EVENT_INPUT_CHANGE,f_onInputChange);

	evt_off(self.timer,core.EVENT.TIMER,f_timer);
	timelater_remove(self.timer);

	self.img:dispose();
	--[[if(self.bg) then
		self.bg:dispose();
	end--]]
	input_dispose(self._in);
	func_clearTableItem(self);
end

function Input:visible(v)
	input_visible(self._in,v);
	self.img:visible(v);
end