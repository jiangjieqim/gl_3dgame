function btnCallBack(name)
	local btn = instance[name];
	if(btn.clickEvtCallBack)	then	btn.clickEvtCallBack(btn)	end
end
function btnMouseDown(name)
	local btn = instance[name];
	--print(name);
	if(btn.mouseDownEvt) then
		btn.mouseDownEvt(btn);
	end
end
--按钮
button = {}
function button.new(url,x,y,w,h,clickEvt,btnLabel,mouseDownEvt)
	local btn = 
	{
		ptr=nil,--Sprite引用
		tf=nil,
		clickEvtCallBack=nil,
		
		id = 0,	--按钮id
		mouseDownEvt=nil,
	};
	
	btn.clickEvtCallBack = clickEvt;
	local name = tableToInt(btn);--tostring(sc);
	instance[name] = btn;
	
	local mouseDownEvtStr = nil;
	if(mouseDownEvt==nil) then
		btn.mouseDownEvt = nil;
	else 
		mouseDownEvtStr = "btnMouseDown";
		btn.mouseDownEvt = mouseDownEvt;
	end
	
	btn.ptr=sprite_create(name,x,y,w,h,"btnCallBack",nil,mouseDownEvtStr);
	sprite_texName(btn.ptr,url);
	
	--设置按钮点击缩放特效
	setv(btn.ptr,flags.FLAGS_BUTTON_EFFECT);
	
	--创建按钮文本
	if btnLabel == "" or btnLabel == nil then
		
	else
		--print("btn tf_create");
		btn.tf=tf_create(128,x,y,1.0,1.0,1.0);
		tf_setText(btn.tf,btnLabel);
	end
	
	--setv(btn.ptr,flags.FLAGS_DRAG);--设置为可以拖拽状态
	
	return btn;
	
end

--设置按钮的文本
function button.setLabel(btn,label)
	tf_setText(btn.tf,label);
end

--设置按钮id
function button.setId(btn,id)
	btn.id = id;
end

--获取按钮id
function button.getId(btn)
	return btn.id;
end
--销毁按钮对象
function button.dispose(btn)
	ptr_remove(btn.ptr);
	ptr_remove(btn.tf);
end
--[[
	创建一个通用样式按钮
	local btn1 = button.createStyle1Btn(0,0,btnClick,"AShjs");
	local btn1 = button.createStyle1Btn(0,0,nil,"AShjs");
	local btn1 = button.createStyle1Btn(0,0,btnClick,);
]]--
function button.createStyle1Btn(x,y,clickEvt,label)
	return button.new("smallbtn.png",x,y,60,20,clickEvt,label)
end