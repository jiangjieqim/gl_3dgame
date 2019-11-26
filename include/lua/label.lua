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

function label_get_wordpos(label)
	return ftext_get_wordpos(label.tf);
end
--[[function label_get_size(label)
	return ftext_getsize(label.tf);
end--]]
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
--***************************************************
NLabel = {
	tf,
};
NLabel.__index = NLabel;
setmetatable(NLabel, Base);

function NLabel:new(w,h,cam)
	local self = Base:new();
	setmetatable(self, NLabel);
	self:settype(14);
	self.tf = ftext_create(w,h,cam);
	return self;
end

function NLabel:set_text(s)
	ftext_reset(self.tf,s);
end

function NLabel:set_pos(x,y)
	ftext_setpos(self.tf,x,y);
end

function NLabel:get_size()
	return ftext_get_wh(self.tf);
end

function NLabel:get_text()
	return ftext_str(self.tf);
end

function NLabel:dispose()
	fext_dispose(self.tf);
	func_clearTableItem(self);
end

