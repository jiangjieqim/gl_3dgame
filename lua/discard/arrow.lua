--¼ýÍ·¿ØÖÆÆ÷
function arrow(x,y,callBack,url,w)
	
	local function f_arrowbtnClick(btn)
		--print(btn.name)
		if(callBack) then callBack(btn_get_data(btn))end
	end
	
	x = x or 0
	y = y or 0
	url = url or "arrow3.png"
	w = w or 32;
	
	local arrow1=btn_create(x + w,y,w,w,url)
	local arrow2 = btn_create(x+2*w,y+w,w,w,url);
	change_attr(arrow2.sprite,"sprite_rotateZ",tostring(-PI/2));
	
	local arrow3 = btn_create(x+w,y+w*2,w,w,url);
	change_attr(arrow3.sprite,"sprite_rotateZ",tostring(-PI));
	
	local arrow4 = btn_create(x,y+w,w,w,url);
	change_attr(arrow4.sprite,"sprite_rotateZ",tostring(-PI*1.5));
	
	btn_bindClick(arrow1,f_arrowbtnClick)
	btn_bindClick(arrow2,f_arrowbtnClick)
	btn_bindClick(arrow3,f_arrowbtnClick)
	btn_bindClick(arrow4,f_arrowbtnClick)
	
	btn_set_data(arrow1,0)--top
	btn_set_data(arrow2,1)--right
	btn_set_data(arrow3,2)--bottom
	btn_set_data(arrow4,3)--left

end
