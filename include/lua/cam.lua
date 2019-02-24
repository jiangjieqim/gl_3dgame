local Camera = {}

function Camera:position(x,y,z)
	change_attr(nil,"cam_xyz",string.format("%.3f,%.3f,%.3f",x,y,z))
end

function Camera:rx(rx)
	change_attr(nil,"camRX",rx);
end

function Camera:ry(ry)
	change_attr(nil,"camRY",ry);
end

function Camera:ry(rz)
	change_attr(nil,"camRZ",rz);
end

function Camera:new()
	local cam = {
	}
	
	self.__index = self;
    setmetatable(cam, self);
	--print("***************cam");
	return cam;
end
cam = Camera:new();
