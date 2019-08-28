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

function Camera:rotate(value)
	change_attr(nil,"cam_rotate",string.format("%s",value));
end

function Camera:new()
	local cam = {
	}
	
	self.__index = self;
    setmetatable(cam, self);
	--print("***************cam");
	return cam;
end
--cam = Camera:new();


function cam_setPosition(x,y,z)
	change_attr(nil,"cam_xyz",string.format("%.3f,%.3f,%.3f",x,y,z));
end

function cam_setRotateX(rx)
	change_attr(nil,"camRX",rx);
end

--÷ÿ÷√camera
function cam_reset()
    change_attr(nil,"cam_xyz",string.format("%.3f,%.3f,%.3f",0,0,0));
    change_attr(nil,"camRX",0);
    change_attr(nil,"camRY",0);
    change_attr(nil,"camRZ",0);
end