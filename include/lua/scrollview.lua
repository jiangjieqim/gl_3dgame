--从itemRender list对象池中获取一个itemRender节点对象,如果对象池中没有就创建一个
local function f_pool_getItem(sv)
    local data = sv.pool;
    for key, value in pairs(data) do
            if(value.used == 0) then
                value.used = 1;
                return value;
            end
	end
    local node = sv.itemFunc();
    sv.pool[tostring(node)] = node;    
    return node;
end
--获取当前itemRender的高度,如果非nil取初始化的时候的gap值作为高度
local function f_getItemHeight(sv)
	if(sv.itemHeight==nil) then
		local node = f_pool_getItem(sv);
		node.used = 0                                                                                                                     
		local w,h = func_get_sprite_size(node.view);
		sv.itemHeight = h;
	end
	return sv.itemHeight;
end

--检测item对象是否在遮罩区域外
local function f_checkItemOutside(sv,pos_y)
    if(pos_y   >= sv.sh) then
        return 1;--向下超出
    end

    if(pos_y + f_getItemHeight(sv) <= 0 ) then
        return 2;--向上超出
    end

    return 0;--不在遮罩区域外
end

--当前的2dcam句柄,指定sprite必须指定对应的cam引用
function scrollView_get_cam(sv)
	return sv.fbo.cam2d;
end

--设置滚动条组件的坐标
function scrollView_set_pos(sv,x,y)
	sv.x = x;
	sv.y = y;
	fboobj_set_pos(sv.fbo,x,y);
end
--从对象池中释放一个节点
local function f_pool_recycs(sv,targetObj)
	local data = sv.pool;
    for key, value in pairs(data) do
		if(value == targetObj) then
			value.used = 0;
			print("recycs");
			return;
		end
	end
end

--释放对象池中的所有节点
local function f_pool_recycs_all(sv)
    local data = sv.pool;
    for key, value in pairs(data) do
        value.used = 0;
	end
end

--根据索引获取该索引对应的数据
local function f_find_data_by_index(sv,index)
	local n = 0;
	for key, value in pairs(sv.dataList) do  
--		print(key,value);
		if(n == index) then
			return value;
		end
		n=n+1;
	end
	return nil;
end

--根据数据获取对应的node引用
local function f_find_node_by_data(sv,data)
	for key, node in pairs(sv.pool) do
		if(node.data == data)then
			return node;
		end
	end
end

--填加一个滚动条
local function f_add_scrollBar(sv)
	local sc;
	if(sv.dir == CONST_DIRECTION_VERTICAL) then
		sc = scrollBar_new(sv.x+sv.sw,sv.y,nil,15,sv.maxSize);
	else
		print("待实现");
	end
	sv.sc = sc;
   
	local curp = 0;
	
	local curIndex = 0;
	
	local _startIndex = 0;--最新的起始索引
	
	--滑动回调
    local function f_scHandle(_sc)
		--*************************纵向
		if(sv.sh >= sv.dataHeight) then
			return;
		end
		local v = _sc.value;
		
		local oy = (sv.dataHeight-sv.sh)*v;
        
		local itemH = f_getItemHeight(sv);
		
		local p = math.floor(oy/itemH);
		local _sub = nil;
		if(p~=curp) then
			--print("******************************* change sub index %d",curp - p);
			_sub = curp - p;
			curp = p;
		end
		
		local iy= oy/itemH-p;
	
		local itemy = -iy*itemH ;
		if(_sub~=nil) then
			curIndex = curIndex-_sub;
		end
		
		local _posList = {};--坐标列表
		for i=0,sv.needCnt-1,1 do
			_posList[i] = itemy;
			itemy = itemy + itemH;
		end
		
		local newList = {};
		local m = 0;
		
		f_pool_recycs_all(sv);--释放对象池中的所有的node
		
		for n = curIndex,curIndex + sv.needCnt - 1 do
			
			local _td = f_find_data_by_index(sv,n);
			
			if(n >= _startIndex and n <= _startIndex + sv.needCnt - 1) then
				--这里的节点只需要重置坐标即可
				
				--print("**************** same data index = ",n,"data",_td);
				local node = f_find_node_by_data(sv,_td);
				node.used = 1;
				sprite_setpos(node.view,0,_posList[m]);				
			else
				--print("need ==> index = ",n,"data",_td);
				newList[m]=_td;
			end
			m = m + 1;
		end
		--print("*********************************");
		for key, value in pairs(newList) do
			--这里的节点要重新刷新节点视图
			local node = f_pool_getItem(sv);
			node.data = value;
			sprite_setpos(node.view,0,_posList[key]);
			sv.itemRefreshFunc(node);
		end
		
		_startIndex = curIndex;
		--********************************************************************
		
		--print(sv.dataList);
--		print(string.format("起始索引为%d,节点数量为%d",_startIndex,sv.needCnt));
		
		--设计思路:对比起始数据和当前变化的数据,过滤求得需要refresh的itemRender节点重新刷新设置数据,偏移不需要重新刷新设置数据的节点
		--这样就要两个列表了
    end
	scrollBar_bind(sc,f_scHandle);	
end

--获取该滚动组件内需要组件item个数
local function f_need_cnt(sv)
	local h = f_getItemHeight(sv);
    local a = sv.sh % h;

    if(a > 0) then a = 1 end
	
    return (math.ceil(sv.sh/h) + a)+2;
end

--数据驱动填充srollView中的数据
function scrollView_set_data(sv,data)
	sv.dataList = data;
    local _realNeedCnt = f_need_cnt(sv);
    sv.needCnt = _realNeedCnt;
    f_pool_recycs_all(sv);


	local cur_h = 0;
	local n = 0;
    local cnt = 0;
	local h =  f_getItemHeight(sv);
	if(data) then
		for key, value in pairs(data) do      
			--print('key=['..key..']'..'value['..value..']')
			--print(value);
            if(f_checkItemOutside(sv,cur_h)~=0 and (cnt+1>_realNeedCnt)) then
                --print(value,"超出可见区域,渲染剔除!,当前已经创建的节点数;",cnt+1,"创建的渲染节点数:",_realNeedCnt);
            else
                local _node = f_pool_getItem(sv);
                _node.data = value;
				
                local itemView = _node.view;
			    
			    --***********************
			    --纵向
                sprite_setpos(itemView,0,cur_h);
				
				_node.index = n;
				
				--print("设置索引",n);
				
				sv.itemRefreshFunc(_node);--refresh data
				
                n = n + 1;
			    cur_h = cur_h + h;
				
--				print(string.format("n = %d",n));
			    --***********************
            end
            cnt = cnt + 1;	
		end
	end
    
	sv.dataHeight = cnt * h;
end

--销毁滚动条组件
function scrollView_dispose(sv)
	if(sv.itemDisposeFunc == nil) then
		--print(sv.itemDisposeFunc);
		print("itemDisposeFunc = nil");
	end
	
	--遍历itemRender对象池,根据itemDispose回调销毁itemRender对象
    for key, node in pairs(sv.pool) do
		sv.itemDisposeFunc(node);
	end
	
	if(sv.fbo) then
		fboobj_dispose(sv.fbo);
	end
	
	if(sv.sc) then
		scrollBar_del(sv.sc);
	end
	
	func_clearTableItem(sv);--清空sv表
end

--CONST_DIRECTION_HORIZONTAL = 0	--水平,横
--CONST_DIRECTION_VERTICAL   = 1 	--垂直,竖

--gap 自定义的间隔,默认是取itemRender的容器的高度
function scrollView_init(sw,sh,x,y,gap)
    local sv = {
		fbo,--FBO句柄对象
		sc,--滚动条句柄对象
		
		dataList,--数据列表
		itemFunc,			--item列表的创建方法回调函数
        itemRefreshFunc,	--设置数据,刷新视图的方法回调函数
		itemDisposeFunc,	--销毁itemRender的回调函数函数

        needCnt,--需要的元素节点数,此数量代表当前的pool对象池中需要的最大渲染节点数
		
		x,y,--当前的坐标,相对于窗口的绝对坐标,其实是fbo挂载的sprite的坐标,将帧缓冲渲染到这个sprite而已
		sw,--遮罩区域的宽
		sh,--遮罩区域的高
		dir,--滚动条的滚动方向,是横向还是纵向
		maxSize,--因为现在的sprite用的等宽等高的,所以这里取最大的maxSize作为宽高
		
		dataHeight,--容器总高度，跟dataList有关系

        itemHeight,--单个item渲染节点的高度
		
        pool = {},--itemRender列表的对象池
  --      poolindex = 0;
    };
	local dir = CONST_DIRECTION_VERTICAL;
	if(gap) then
		sv.itemHeight = gap;
	end
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

--遮罩滚动条使用案例
function example_srollView()
	--*********************************************************************************
	local itemHeight = 30;
	local cam;
	--local cnt = 0;
	local function f_create()
		--local sprite = sprite_create(nil,0,0,90,itemHeight,0,1,cam);	func_setIcon(sprite, "smallbtn.png");
		
		--local sprite = sprite_create_9grid("smallbtn.png",0,0,90,itemHeight,cam,3,3,3,3);	
		--sprite_set_hit_rect(sprite,0,0,90,itemHeight);
		 
		--********************************************************
		local x = 0;
		local y = 0;
		local w = 90;
		local h = itemHeight;
		local url = "smallbtn.png";
		
		local sprite = sprite_create_typical(nil,x,y,w,h);--string.format("sv%d",cnt)
		engine_addNode(sprite);
		loadtexs(url,func_texloadend, { sprite=sprite;url=url});
		sprite_set_hit_rect(sprite,x,y,w,h);
		set_cam(sprite,cam);

		--********************************************************
		
		local node ={};
	--	setv(sprite,FLAGS_DRAW_PLOYGON_LINE);
		
		--engine_addNode(sprite);


		if(true) then
			--节点特别多的时候,这里的渲染绘制会比较卡顿,可以考虑用分帧处理渲染
			local tf = ftext_create(64,64,13,12,cam);
			local con = ftext_get_container(tf);
			func_addchild(sprite,con);
			node.tf = tf;
		end
		node.view = sprite;
		--setv(sprite,FLAGS_DRAW_PLOYGON_LINE);

		node.data = nil;
		
		node.used = 1;
		node.index = nil;
		local function clickEvt()
			print("点击的节点index = ",node.index,"data = ",node.data);
		end
		
		evt_on(sprite,EVENT_ENGINE_SPRITE_CLICK,clickEvt);
		return node;
	end
	--节点销毁回调
	local function f_dispose(node)
		if(node.tf) then
			fext_dispose(node.tf);
			node.tf = nil;
		end
		if(node.view) then
			ptr_remove(node.view);
		end
	end

	--刷新视图
	local function itemRefreshFunc(node)
	--    print(node.data);
		if(node.tf) then
			fext_clear(node.tf);
			local str = string.format("i = %d,__%d",node.index,node.data);
			ftext_parse(node.tf,str);
		end
		--print("index",node.index,"刷新视图,设置数据",node.data);
	end

	local t = {}; --{10,20,30,40,50,60,70,80,90,100,110,120};

	for i = 1,10,1 do
		t[i] = i;
	end
	--print(#t);--table的长度
	
	--*************************************************************************************
	--初始化
	local sv = scrollView_init(100,itemHeight*4,10,0);
	sv.itemFunc = f_create;				--设置itemRende的创建回调
	sv.itemRefreshFunc = itemRefreshFunc;	--设置刷新视图的回调
	sv.itemDisposeFunc = f_dispose;		--设置itemRender销毁回调函数
	cam = scrollView_get_cam(sv);
	scrollView_set_data(sv,t);
	--*************************************************************************************
	return sv;
end