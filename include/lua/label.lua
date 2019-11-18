function label_create(w,h,cam)
	local label = {
		name = nil,
		--组件类型
		type = 1,
		tf = nil;
	};
	
	label.tf = ftext_create(w,h,cam);
	return label;
end

function label_dispose(label)
	fext_dispose(label.tf);
	func_clearTableItem(label);
end

function label_set_text(label,s)
	ftext_reset(label.tf,s);
end

function label_get_text(label,s)
	return	ftext_str(label.tf);
end

function label_set_pos(label,x,y)
	ftext_setpos(label.tf,x,y);
	
end

function label_get_size(label)
	return ftext_getsize(label.tf);
end
function label_set_visible(label,v)
	if(type(v) == "boolean") then
		if(v) then 
			v  = 1;
		else
			v = 0;
		end
	end
	
	ftext_vis(label.tf,v);
end