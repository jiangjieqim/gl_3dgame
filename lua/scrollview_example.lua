--********************************************************
local function clickEvt(name,p)
	local node = p;
	print("点击的节点index = ",node.index,"data = ",node.data);
	--scrollView_dispose(sv);
end

local function f_create(cam)
	local x = 0;
	local y = 0;
	local w = 90;
	local h = 30;
	local url = "smallbtn.png";
	
	local sprite =  Image:new(w,h);
	sprite:set_pos(x,y);
	sprite:mouseEnable(true);
	sprite:set_cam(cam);
	sprite:seticon(url);

	--********************************************************
	
	local node ={};

	if(true) then
		--节点特别多的时候,这里的渲染绘制会比较卡顿,可以考虑用分帧处理渲染
		local tf = NLabel:new(nil,nil,cam);			
		sprite:addChild(tf:get_container());
		node.tf = tf;
	end
	node.view = sprite:get_container();
	node.skin = sprite;

	node.data = nil;
	
	node.used = 1;
	node.index = nil;

	sprite:on(EVENT_ENGINE_SPRITE_CLICK,clickEvt,node);
	
	return node;
end


--节点销毁回调
local function f_dispose(node)
	if(node.tf) then
		node.tf:dispose();
		node.tf = nil;
	end
	
	node.skin:dispose();
end

--刷新视图
local function itemRefreshFunc(node)
--    print(node.data);
	if(node.tf) then
		--fext_clear(node.tf);
		local str = string.format("i = %d,__%d",node.index,node.data);
		--ftext_parse(node.tf,str);
		node.tf:set_text(str);
	end
	--print("index",node.index,"刷新视图,设置数据",node.data);
end

--遮罩滚动条使用案例
function example_srollView(posx,posy)
	posx = posx or 0;
	posy = posy or 0;
	--*********************************************************************************
	
	--local cam;--渲染所属的camera

	local t = {}; --{10,20,30,40,50,60,70,80,90,100,110,120};

	for i = 1,10,1 do
		t[i] = i;
	end
	--print(#t);--table的长度
	
	--*************************************************************************************
	--初始化
	local sv = ScrollView:new(100,120,posx,posy);
	sv:bind(f_create,itemRefreshFunc,f_dispose);
	sv:set_data(t);
	
	--*************************************************************************************
	return sv;
end
--*********************************************************


--案例
--[[
dofile("..\\include\\lua\\scrollview_example.lua")
local function f_tex_complete(p)
	
	local sc = example_srollView(100,100);
	--sc:dispose();
end
loadtexs("checkbox.png;smallbtn.png",f_tex_complete);
--]]