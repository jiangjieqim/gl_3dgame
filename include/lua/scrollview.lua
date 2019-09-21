function scrollView_set_pos(sv,x,y)
	sv.x = x;
	sv.y = y;
	fboobj_set_pos(sv.fbo,x,y);
end

--当前的2dcam句柄
function scrollView_get_cam(sv)
	return sv.fbo.cam2d;
end

local function f_offset_xy(sv,oy)
	local cur_h = oy;
	local data = sv.sprList;
	for key, value in pairs(data) do      
		--print('key=['..key..']'..'value['..value..']')
		--print(value);
		
		local spr = value;
		sprite_setpos(spr,0,cur_h);
		local w,h = func_get_sprite_size(spr);
		cur_h = cur_h + h;
		
--[[		
		local itemView = sv.itemFunc();
		sv.sprList[n]= itemView;
		
		local w,h = func_get_sprite_size(itemView);
		--print(w,h);
		
		
		--***********************
		--纵向
		sprite_setpos(itemView,0,cur_h)
		n = n + 1;
		cur_h = cur_h + h;
		--***********************
		--]]
	end
end

--填加一个滚动条
local function f_add_scrollBar(sv)
	local sc;
	if(sv.dir == CONST_DIRECTION_VERTICAL) then
		sc = scrollBar_new(sv.x+sv.sw,sv.y,15,sv.maxSize);
	else
		print("待实现");
	end
	sv.sc = sc;
	
	--滑动
    local function f_scHandle(_sc)
	---[[	
	
		--*************************纵向
		if(sv.sh >= sv.dataHeight) then
			return;
		end
		local v = _sc.value;
		
		local oy = (sv.sh - sv.dataHeight)*v;
        --print(oy);
		f_offset_xy(sv,oy);
        --func_setPos(sprite,0,(h-ch)*v);--具有矩形遮罩区域的滚动条
		--]]
		--**************************
    end
	scrollBar_bind(sc,f_scHandle);	
end

--数据驱动填充srollView中的数据
function scrollView_set_data(sv,data)
	sv.dataList = data;
	--local cam = f_get_cam(sv);
	if(sv.sprList==nil) then
		sv.sprList = {}
	end
	
	local cur_h = 0;
	local n = 0;
	if(data) then
		for key, value in pairs(data) do      
			--print('key=['..key..']'..'value['..value..']')
			print(value);
			local itemView = sv.itemFunc();
			sv.sprList[n]= itemView;
			
			local w,h = func_get_sprite_size(itemView);
			--print(w,h);
			
			
			--***********************
			--纵向
			sprite_setpos(itemView,0,cur_h)
			n = n + 1;
			cur_h = cur_h + h;
			--***********************
		end
	end
	
	sv.dataHeight = cur_h;
end

--CONST_DIRECTION_HORIZONTAL = 0	--水平,横
--CONST_DIRECTION_VERTICAL   = 1 	--垂直,竖
function scrollView_init(sw,sh,x,y,dir)
    local sv = {
		fbo,--FBO句柄
		sc,--滚动条句柄
		
		sprList,--sprite列表
		dataList,--数据列表
		itemFunc,--item列表的创建方法

		
		x,
		y,
		sw,--宽
		sh,--高
		dir,
		maxSize,
		
		dataHeight,--容器总高度，跟dataList有关系
    };
	dir = dir or CONST_DIRECTION_VERTICAL;
	
	local maxSize = sw;

	if(sw < sh) then
		maxSize = sh;
	end
---[[
	sv.maxSize = maxSize;
	
	sv.sw = sw;
	sv.sh = sh;
	
	sv.dir = dir;
	
	sv.fbo = fboobj_init(maxSize,maxSize);
    
	x = x or 0;
	y = y or 0;
	sv.x =x;
    sv.y =y;
	f_add_scrollBar(sv);
    ---[[
	scrollView_set_pos(sv,x,y);
	--]]
	return sv;
end
