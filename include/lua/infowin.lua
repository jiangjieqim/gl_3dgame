function f_pick(p)
	--moduleUI_bind(p)
	--f_selectPick(p)
    --print("f_pick:"..p);
    --crl:bind(p);
	print("pick now!");--拾取的对象
end
--加载一个obj模型
local function f_loadObj(model)
	
	local name = func_create_name();
	model = model or 'tri'--'box'--'torus'
	--local obj = func_loadobj(model)--box	'torus'
	local obj = load_model(name,string.format("\\resource\\obj\\%s.obj",model));	

	engine_bind_default_3dcam(obj);
	
	setMaterial(obj,func_load("//resource//material//wolf.mat"));
	setv(obj,FLAGS_VISIBLE);
	---[[
	func_changeFlags(obj,FLAGS_RAY);				--设置为可拾取状态
	func_changeFlags(obj,FLAGS_DRAW_RAY_COLLISION);	--绘制射线盒子
	func_changeFlags(obj,FLAGS_DISABLE_CULL_FACE);	--显示双面显示
	--func_changeFlags(obj,FLAGS_GLSL_OUTLINE);
	func_changeFlags(obj,FLAGS_DRAW_PLOYGON_LINE);

	func_bind_collide(obj)
	--moduleUI_bind(obj)
	--f_add_model(obj)

	func_set_pick(obj,"f_pick");
	func_set_x(obj,1);
	--]]
	return obj
end

--------------------------------------------------------------------
-- 信息调试面板

--[[local mc = FpsView:new();
mc:show();--]]
local _stat;
local function f_select(self,index,param)
	
	
	
	--local index =listbox_get_index(list);
	---[[
    if (index == 0) then
        --func_ex_info();


--	    alert("窗口测试:引擎信息");
		print("windows test!");
		
		FpsView:getIns():show();
		
    elseif (index == 1) then
        func_gc();
--    elseif (index == 2) then
--        ex:switch_spriteline()
--        print("切换为Sprite线框状态:" .. ex:get_spriteline());
    elseif (index == 2) then
        --fps();
    elseif (index == 3) then
        _stat = not _stat;
--        print(_stat);

        if(_stat) then
            engine_setBg(0.1,0.1,0.1);
        else
            engine_setBg(0.4,0.4,0.4);
        end
	elseif (index == 4) then
		--创建一个mesh
		f_loadObj();
    elseif(index == 5) then
        cam_reset();
    end
	--]]
end



function infowin(x, y)
	
	
	--[[local list  = listbox_new(x or 0, y or 0);
	listbox_bind(list,f_select);
	
	
	listbox_add(list,"引擎信息");
	listbox_add(list,"gc");
	listbox_add(list,"fps");
	listbox_add(list,"背景颜色");
	listbox_add(list,"创建mesh");
	listbox_add(list,"重置cam");
	listbox_set_title(list,"infowin");--]]
	
	local list = NListBox:new(x,y,128);
	list:addItem("引擎信息");
	list:addItem("gc");
	list:addItem("fps");
	list:addItem("背景颜色");
	list:addItem("创建mesh");
	list:addItem("重置cam");
	list:bind(f_select);

	
	--listbox_del(list);
	--return list;
end