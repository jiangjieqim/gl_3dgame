--滚动条
ScrollBar = {}
--function ScrollBar.showProgress(sc,)
	--if(sc.progressTf) then
	--	tf_setText(sc.progressTf,tostring(sc.progressValue));
	--end
--end

--是否是整数
local function IsIntegral(sc)
	return sc.isIntegral == 1
end

--设置文本
local function SetProgressTf(sc)
	if(sc.progressTf) then
		
		local str = str_cut_decimal(sc.progressValue,2)--取小数后两位
		
		--设置为整数
		if(IsIntegral(sc)) then
			str = tl_getIntegralPart(tonumber(sc.progressValue));
		end
		tf_setText(sc.progressTf,str);
	end
end
--设置百分比
local function ScDragMove(sc,progress)
	
	if(sc == nil) then
		return
	end	
	
	local width = progress * (sc.e - sc.s)
	
	sc.progressValue = sc.s + width
	
	SetProgressTf(sc)
	
	sc.callBackDragMove(sc)--百分比回调
	
end

function LuaDragMove(name,progress) 
	ScDragMove(instance[name],progress)
end

--设置滑动的区间值
function ScrollBar.setProportion(sc,s,e)
	--sc.proportion = value;
	sc.s = s;
	sc.e = e;
end

--进度数据显示为整数
function ScrollBar.showInt(sc)
	sc.isIntegral = 1;
end

--背景的后缀
local function GetLittleSuffix()
	return "_bg"
end

--获取小按钮的拖拽方向
local function 
f_Sprite_DragDirection(littleName)
	local scrollbtn = xfun.FindNode(littleName)
	return get_attr(scrollbtn,"dragDirection")
end

--设置小按钮位置(横向)
--[[
	bgName:进度条(Sprite)大背景名
	littleName:小按钮(Sprite)名
	DragDirection:拖拽的方向
--]]
local function 
f_SetBarPostion(bgName,littleName)
	
	--滑动的方向
	local dragDirection=f_Sprite_DragDirection(littleName)
	
	--背景sprite引用
	local sprite = func_getInstance(bgName)
	
	--背景局部坐标
	local local_x,local_y = get_attr(sprite,"spriteLocalXY")
	
	--进度条的坐标
	local bx,by = get_attr(sprite,"spritePos")
	
	--进度条的宽高
	local sprite_w,sprite_h = get_attr(sprite,"spriteSize")
	
	--小按钮引用
	local scrollbtn = xfun.FindNode(littleName)
	
	--小按钮的宽高
	local sc_w,sc_h = get_attr(scrollbtn,"spriteSize")
		
	--local cur_x = --GetBarX(x,sc_w,w)
	
	
	if(dragDirection == CONST_DIRECTION_HORIZONTAL) then
		local v = local_x
		
		local target_x = bx + v 
		
		--print("设置小按钮的位置:"..tostring(target_x)..","..tostring(by)..",sc_w="..sc_w..',sc_h='..sc_h..',sprite_w='..sprite_w..',sprite_h='..sprite_h)

		if (target_x > sprite_w  - sc_w + bx) then
			target_x = sprite_w - sc_w + bx
			--print('纠正!!!')
		end
		
		sprite_setpos(scrollbtn,target_x,by)
		return v / sprite_w
	else

		local v = local_y
		
		local target_y = by + v 
		
		--print("设置小按钮的位置:"..tostring(target_x)..","..tostring(by)..",sc_w="..sc_w..',sc_h='..sc_h..',sprite_w='..sprite_w..',sprite_h='..sprite_h)

		if (target_y > sprite_h  - sc_h + by) then
			target_y = sprite_h - sc_h + by
			--print('纠正!!!')
		end
		
		sprite_setpos(scrollbtn,bx,target_y)
		return v / sprite_h
	end
end


--[[
	点击滑动条上的小按钮
--]]
function ScrollBarBG_Click(name)
	
	--大背景
	local sprite = func_getInstance(name)
	local x,y = get_attr(sprite,"spriteLocalXY")
	local w,h = get_attr(sprite,"spriteSize")
	
	
	--小按钮
	local scName=string.sub(name,0,-string.len(GetLittleSuffix())-1)
	local sc = instance[scName]
	
	--拖拽
	local v =  f_SetBarPostion(name,scName)
	ScDragMove(sc,v)
end

local function CreateScrollBg(spName,x,y,bgWidth,bgHeight,bgImg)
	--local spName = name.."_bg"
	local bg=sprite_create(spName,x,y,bgWidth,bgHeight,"ScrollBarBG_Click");
	--setv(bg,flags.FLAGS_DRAW_PLOYGON_LINE);--设置为线框渲染
	--func_bindInstance(bg)
	instance[spName] = bg
	
	func_getInstance(spName)
	--local bgSprite = func_getInstance(spName)
	
	sprite_texName(bg,bgImg);
	
	return bg
end

local function createScrollBar()
	local sc = 
	{
		isIntegral = nil,
		btn = nil,
		bg = nil,
		tf = nil,
		progressTf = nil,
		
		progressValue=0,
		callBackDragMove=nil,
		
		--proportion = 1.0;--比率1~无穷
		--滑条的区间
		s = 0.0,
		e = 1.0,
	}
	return sc
end
--[[

	dragDirection = 0;	--0:横向拖动滑块,1纵向拖动滑块
	label :显示的文本
	bProgress = true 显示进度条文本,false 隐藏进度条文本
--]]
function ScrollBar.new(
	x,y,
	barImg,
	barWidth,
	barHeight,
	bgImg,
	bgWidth,
	bgHeight,
	callBackProgress,
	dragDirection,
	label,
	bProgress)
	
	local sc = createScrollBar()

	sc.callBackDragMove = callBackProgress
	
	local name = tableToInt(sc);--tostring(sc);
	instance[name] = sc;
	
	if(label) then
		sc.tf=tf_create(128,x+bgWidth,y,1.0,1.0,1.0);
		tf_setText(sc.tf,label);
	end
	
	if(bProgress) then
		sc.progressTf=tf_create(128,x+bgWidth + 50,y,1.0,1.0,1.0);
	end
	
	--背景
	local bg= CreateScrollBg(name..GetLittleSuffix(),x,y,bgWidth,bgHeight,bgImg)
	

	--滚动小按钮  
	local btn=sprite_create(name,x,y,barWidth,barWidth,"","LuaDragMove","",dragDirection);
	
	sprite_texName(btn,barImg);
	--设置可拖拽
	sprite_setDragScope(btn,0,0,bgWidth,bgHeight);
	
	sc.btn = btn;
	sc.bg = bg;
	
	--print("***********************"..instance[name].progressValue.."**("..name..")");
	
	return sc;
end

--创建一个默认的样式
--bProgress:是否显示一个进度文本
function ScrollBar.CreateHorizontal(x,y,callBackProgress,label,bProgress)
	return	ScrollBar.new(x,y,"gunbar.png",10,10,"gundi.png",100,10,callBackProgress,CONST_DIRECTION_HORIZONTAL,label,bProgress);
end

function ScrollBar.CreateVertucal(x,y,callBackProgress,label,bProgress)
	return	ScrollBar.new(x,y,"gunbar.png",10,10,"gundi.png",10,100,callBackProgress,CONST_DIRECTION_VERTICAL,label,bProgress);
end

--销毁
function ScrollBar.dispose(sc)
	ptr_remove(sc.btn);
	ptr_remove(sc.bg);
	ptr_remove(sc.progressTf);
	ptr_remove(sc.tf);
end


