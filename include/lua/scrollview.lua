--检测item对象是否在遮罩区域外
local function f_checkItemOutside(sv,pos_y)
    if(pos_y   >= sv.sh) then
        return 1;--向下超出
    end

    if(pos_y + sv.itemHeight <= 0 ) then
        return 2;--向上超出
    end

    return 0;
end

--当前的2dcam句柄
function scrollView_get_cam(sv)
	return sv.fbo.cam2d;
end
--重新设置node节点的index的值
local function f_rebuildIndex(list,pnode,ptype)
    local cnt = 0;
    for key, value in pairs(list) do
        local node = value;
        cnt = cnt + 1;
        if(node~=pnode) then
            if(ptype == 1) then
                node.index = node.index + 1;
            else
                node.index = node.index - 1;
            end
           
        end
    end
    if(ptype == 1) then
        pnode.index = 0;
    else
        pnode.index = cnt-1;
    end
    pnode.ptype = ptype;
   -- print(list);
end

local function f_getPos(list,index)
 for key, value in pairs(list) do
        local node = value;
        if(node.index == index)then
            local spr = node.view;
            local x,y = func_get_sprite_xy(spr);
            return y;
        end
    end
end

local function f_offset_xy(sv,oy,changeSub)
--    print(changeSub);
--    oy = oy % sv.sh;

	local cur_h = oy;
	local data = sv.pool;
--	local noChangeList = {};

    local cnt = sv.needCnt;
--  cnt = math.floor(cnt);
--  print("cnt:",cnt);
    local outNode = nil;
    local _myNode = nil;
    for key, value in pairs(data) do
	    --print('key=['..key..']'..'value['..value..']')
	    --print(key,value);
		
        local node = value;
        
        if(node.used == 1) then
            --node.view = view;
            --node.index = n;
            local spr = node.view;
            local x,y = func_get_sprite_xy(spr);
            local ptype=f_checkItemOutside(sv,y+changeSub);
            if( ptype~= 0) then
                --剔除
                if ptype == 1  then
                    --向下超出了
                    y = y - cnt * sv.itemHeight;
                elseif(ptype == 2) then
                    --向上超出了
                    y = y + cnt * sv.itemHeight;
                    --node.index = cnt;
                end
                f_rebuildIndex(sv.pool,node,ptype);
                outNode = node;
            else
                y = y + changeSub;
                sprite_setpos(spr,0,y);           
            end 
        --    node.y = y;
	    end
        -- print(oy,node.index,node,y);
    end
 
 ---[[
    if(outNode) then
        local i = 0;
        local a = 1;
        if(outNode.ptype == 1) then
            --向下超出了
            i = i + 1;
            a = -1;
        else
            i = cnt-1;
            i = i -1;
        end
        local y = f_getPos(data,i);
       
        local view = outNode.view;
        --print("*****",i,y,view);
        sprite_setpos(view,0,y+a*sv.itemHeight); 
    end
--]]
end

function scrollView_set_pos(sv,x,y)
	sv.x = x;
	sv.y = y;
	fboobj_set_pos(sv.fbo,x,y);
end

local function f_pool_getItem(sv)
    local data = sv.pool;
    for key, value in pairs(data) do
            --local obj = value.obj;
            --local used = value.used;
            if(value.used == 0) then
                value.used = 1;
                return value;
            end
	end
    local node = sv.itemFunc();
    
    --print(tostring(node));

    sv.pool[tostring(node)] = node;
    
    return node;

    --[[
    local p = {};
    p.obj =  sv.itemFunc();
    p.used = 1;
    sv.pool[sv.poolindex] = p;
    --]]
--      print("new node",sv.poolindex);ret
--      sv.poolindex = sv.poolindex + 1;
--      return p.obj;

end

local function f_pool_recycs(sv,targetObj)
	local data = sv.pool;
    for key, value in pairs(data) do
          --  local obj = value.obj;
         --   local used = value.used;
			
			--print('key=['..key..']'..'value['..value..']');
	        --print(value,obj,targetObj);		
			
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
       -- local obj = value.obj;
       -- local used = value.used;
        value.used = 0;
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

    local temp = 0;
	--滑动
    local function f_scHandle(_sc)
	---[[	
	
		--*************************纵向
		if(sv.sh >= sv.dataHeight) then
			return;
		end
		local v = _sc.value;
		
		local oy = (sv.sh - sv.dataHeight)*v;
        local stat = oy - temp;
--        if(oy < temp) then
--            stat = 0;
--		else
--        end
        temp = oy;
        
        f_offset_xy(sv,oy,stat);
        --func_setPos(sprite,0,(h-ch)*v);--具有矩形遮罩区域的滚动条
		--]]
		--**************************
    end
	scrollBar_bind(sc,f_scHandle);	
end

----存储节点
--local function f_saveNode(sv,view,n)
--    local node = {};
--    node.view = view;
--    node.index = n;
--    node.ptype = 0;
--   -- node.y = 0;
--    sv.sprList[n] = node;
--end
--获取该滚动组件内需要组件item个数
local function f_need_cnt(sv)
     local a = sv.sh % sv.itemHeight;
    --print(sv.sh,sv.itemHeight);
--    print(sv.sh/sv.itemHeight,"a=",a);
    if(a > 0) then a = 1 end
    return (math.ceil(sv.sh/sv.itemHeight) + a)+1;
end

--数据驱动填充srollView中的数据
function scrollView_set_data(sv,data)
	sv.dataList = data;
	--local cam = f_get_cam(sv);
--	if(sv.sprList==nil) then
--        --初始化构造对象池
--		sv.sprList = {}
--	end
    local _realNeedCnt = f_need_cnt(sv);
    sv.needCnt = _realNeedCnt;
    f_pool_recycs_all(sv);


	local cur_h = 0;
	local n = 0;
    local cnt = 0;
	if(data) then
		for key, value in pairs(data) do      
			--print('key=['..key..']'..'value['..value..']')
			--print(value);
            if(f_checkItemOutside(sv,cur_h)~=0 and (cnt+1>_realNeedCnt)) then
                
                print(value,"超出可见区域,渲染剔除!,当前已经创建的节点数;",cnt+1,"需要的节点数:",_realNeedCnt);

            else
                local _node = f_pool_getItem(sv);
                _node.data = value;
                local itemView = _node.view;
			    sv.itemRefreshFunc(_node);--refresh data
			    local w,h = func_get_sprite_size(itemView);
			    --print(w,h);
			    sv.itemHeight = h;
			    --***********************
			    --纵向
                sprite_setpos(itemView,0,cur_h);

                --sv.sprList[n]= itemView;
                --f_saveNode(sv,itemView,n);

                n = n + 1;
			    cur_h = cur_h + h;
			    --***********************
            end		
            cnt = cnt + 1;	
		end
	end


    
	sv.dataHeight = cnt * sv.itemHeight;
    sv.datacnt = cnt;
end

--CONST_DIRECTION_HORIZONTAL = 0	--水平,横
--CONST_DIRECTION_VERTICAL   = 1 	--垂直,竖
function scrollView_init(sw,sh,x,y,dir)
    local sv = {
		fbo,--FBO句柄
		sc,--滚动条句柄
		
--		sprList,--sprite列表
		dataList,--数据列表
        datacnt, --数据列表长度
		itemFunc,--item列表的创建方法
        itemRefreshFunc,--设置数据,刷新视图的方法

        needCnt,--需要的元素节点数
		
		x,
		y,
		sw,--遮罩区域的宽
		sh,--遮罩区域的高
		dir,
		maxSize,
		
		dataHeight,--容器总高度，跟dataList有关系

        itemHeight,--单个item渲染节点的高度

        pool = {},--对象池
  --      poolindex = 0;
    };
	dir = dir or CONST_DIRECTION_VERTICAL;
	
    sv.itemHeight = 64;

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
