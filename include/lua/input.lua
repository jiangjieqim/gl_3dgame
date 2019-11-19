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
--输入框是否在焦点内
local function input_isFocusIn(ptr)	
	if (func_get_curFocus() == input_get_container(ptr))then
		return true;
	end
	return nil;
end

local function input_clear(ptr)
	c_input(ptr,"clear");
end

--创建一个输入组件
local function example_input(x,y)
	--作为输入框的背景
	--local sprite =sprite_create("bg11",100,50,128,14);
	--func_setIcon(sprite,"gundi.png");
	
	x = x or 0;
	y = y or 0;
	
	local sprite = func_create_grid_sprite(x,y,128,14);
	
	
	
	--setv(sprite,FLAGS_DRAW_PLOYGON_LINE);


	--local sprite1 =func_sprite_createNoHit(100,100,256,256);
	--func_setIcon(sprite1,"gundi.png");
	--setv(sprite1,FLAGS_DRAW_PLOYGON_LINE);

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

--输入框组件
Input = {
	name = nil,
	type = 9,
	_in,
	container;
};

Input.__index = Input;

function Input:new()
	local self = {};
	setmetatable(self, Input);
	local _in = input_create();
	self._in = _in;
	self.container = input_get_container(_in);	
	return self;
end

function Input:dispose()
	input_dispose(self._in);
	func_clearTableItem(self);
end

