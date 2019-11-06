function label_create(x,y,cam)
	local label = {
		--组件类型
		type = 1,
		tf = nil;
	};
	label.tf = ftext_create(x,y,cam);
	return label;
end

function label_dispose(label)
	fext_dispose(label.tf);
	func_clearTableItem(label);
end

function label_text(label,s)
	ftext_reset(label.tf,s);
end