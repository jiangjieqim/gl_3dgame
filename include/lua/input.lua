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