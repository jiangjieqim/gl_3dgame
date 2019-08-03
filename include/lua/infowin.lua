















function f_pick(p)
	--moduleUI_bind(p)
	--f_selectPick(p)
    --print("f_pick:"..p);
    --crl:bind(p);
	print(p);--拾取的对象
end
--加载一个obj模型
local function f_loadObj(model)
	
	local name = func_create_name();
	model = model or 'tri'--'box'--'torus'
	--local obj = func_loadobj(model)--box	'torus'
	local obj = load_model(name,string.format("\\resource\\obj\\%s.obj",model));	
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
	--func_set_x(obj,1);
	--]]
	return obj
end

--------------------------------------------------------------------
-- 信息调试面板
local list;
local _stat;
local function f_select(index)
    -- local label = listbox_get_label(_l)
    -- func_set_anim(m,label)
    -- string.format('%s vbo=%s',label,tostring(func_is_vbo(obj)) )
    -- listbox_set_label(_l,label)
    -- listbox_set_label(_l,label)

    if (index == 0) then
        func_ex_info();
	alert("窗口测试:引擎信息");
    elseif (index == 1) then
        func_gc();
--    elseif (index == 2) then
--        ex:switch_spriteline()
--        print("切换为Sprite线框状态:" .. ex:get_spriteline());
    elseif (index == 2) then
        fps(0,15);
    elseif (index == 3) then
        _stat = not _stat;
--        print(_stat);

        if(_stat) then
            ex:bgColor(0.1,0.1,0.1);
        else
            ex:bgColor(0.4,0.4,0.4);
        end
	elseif (index == 4) then
		--创建一个mesh
		f_loadObj();
    end
end

function infowin(x, y)
    if (list == nil) then
        list = ListBox:new(x or 0, y or 0, f_select);
		list:isSetTitle(true);
        list:add("引擎信息,gc,fps,背景颜色,创建mesh");
        
        -- list:add("线框ui");--切换线框渲染sprite
    end

    -- listbox_bind(list,f_select)
    -- listbox_select(list,0)--默认选0号索引
    --    listbox_refresh(list);
end