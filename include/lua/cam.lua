function cam_setPosition(x,y,z)
	change_attr(nil,"cam_xyz",string.format("%.3f,%.3f,%.3f",x,y,z));
end

function cam_setRotateX(rx)
	--print(rx);
	change_attr(nil,"camRX",rx);
end

--÷ÿ÷√camera
function cam_reset()
    change_attr(nil,"cam_xyz",string.format("%.3f,%.3f,%.3f",0,0,0));
    change_attr(nil,"camRX",0);
    change_attr(nil,"camRY",0);
    change_attr(nil,"camRZ",0);
end


