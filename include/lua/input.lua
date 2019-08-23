function input_create(w)
	w = w or 128
	return c_input(nil,"create",w);
end
function input_set_pos(ptr,x,y)
	c_input(ptr, "setpos", string.format("%s,%s", x or 0, y or 0));
end

function input_dispose(ptr)
	c_input(ptr, "dispose");
end

function input_get_str(ptr)
	return c_input(ptr,"getstr");
end

function input_get_container(ptr)
	return c_input(ptr,"get_container");
end
--输入框是否在焦点内
function input_isFocusIn(ptr)	
	if (func_get_curFocus() == input_get_container(ptr))then
		return true;
	end
	return nil;
end

function input_clear(ptr)
	c_input(ptr,"clear");
end