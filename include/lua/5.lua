-- dofile("..\\include\\lua\\core.lua")

print('#######################\t材质测试\t#######################')

--设置glsl参数
--有可能当前材质的着色器对象里没有关键字key,那么这时候设置值的时候就会失败
local function func_set_glsl_parms(o,key,v)
	change_attr(o,"glslParms",string.format('%s,%s',key,tostring(v)))
end

--加载一个固定管线模型数据md2,obj
local function func_fixed_load(url,scale,texpath)
	local o=load_model(func_create_name(),url or "\\resource\\obj\\torus.obj",0,0,0,scale or 1.0)
	local mat1=createMaterial("diffuse",texpath or ("\\resource\\texture\\1.tga"),"")
	setMaterial(o,mat1);
	setv(o,FLAGS_VISIBLE);
	return o;
end

--是否采用的是VBO模式渲染的
local function func_is_vbo(obj)
	--if(func_get_type_str(obj)=='vbo') then
    if(JEngine:getIns():get_type_str(obj)=="vbo") then
		return true
	end
	return false
end

--local _model_ptr;

-- 初始化标示
local function f_set_obj_flags(o)
    setv(o, FLAGS_VISIBLE);
    -- setv(o,FLAGS_GLSL_OUTLINE);
    --  setv(o,FLAGS_DRAW_PLOYGON_LINE);--线框渲染
    -- setv(o,FLAGS_DRAW_NORMAL);


    -- 射线拾取
    --- [[
    setv(o, FLAGS_RAY);
    setv(o, FLAGS_DRAW_RAY_COLLISION);
    setv(o, FLAGS_RENDER_BOUND_BOX);
    -- ]]

end

---- 创建茶壶列表
-- local function createObjList(cnt)
--    cnt = cnt or 1
--    local offset = 3

--    local seedNumber = - offset

--    for i = 0, cnt - 1, 1 do

--        local time = func_get_longTime()
--        seedNumber = seedNumber + offset;
--        local _m = func_loadobj("torus")
--        -- load_objVBO("torus");	--"teapot"
--        setv(_m, FLAGS_DRAW_PLOYGON_LINE);

--        func_set_x(_m, seedNumber)

--        func_print(string.format('load_objVBO 消耗%d ms', func_get_longTime() - time))

--    end

-- end
-- 加载一个带透明通道的树叶模型
local function
    load_alpha_model(objName)
    local time = func_get_longTime()
    objName = objName or "quad"
    local o = func_loadobj(objName)
    -- load_VBO_model(name,"\\resource\\obj\\"..objName..".obj");
    local m = createMaterial("vboDiffuse", "\\resource\\texture\\leaf.tga");
    setMaterial(o, m);
    f_set_obj_flags(o);
    func_print(string.format('load_alpha_model 消耗 %d ms', func_get_longTime() - time))
    return o;
end
local function f_setLabel(label, obj)

    if (label == 'diffuse') then

    elseif (label == 'ploygonLine') then

        setv(obj, FLAGS_DRAW_PLOYGON_LINE)

    elseif (label == 'outline') then

        -- setv(obj,FLAGS_DISABLE_CULL_FACE)--先设置双面
        if (func_is_vbo(obj)) then
            func_error("vbo不支持!!!")
        end
        setv(obj, FLAGS_OUTLINE)
    elseif (label == 'point') then

        setv(obj, FLAGS_DRAW_PLOYGON_POINT)

    elseif (label == 'glslOutline') then
        -- setv(obj,FLAGS_DRAW_PLOYGON_LINE)

        -- 有一些情况着色器中没有这些变量就会设置无效

        func_set_glsl_parms(obj, 'lineWidth', 0.05)
        func_set_glsl_parms(obj, 'alpha', 0.5)

        setv(obj, FLAGS_GLSL_OUTLINE)
        -- print('***')
    elseif (label == 'normal') then
        if (func_is_vbo(obj)) then
            func_error("vbo不支持!!!")
        end
        setv(obj, FLAGS_DRAW_NORMAL)
        -- 绘制法线

    elseif (label == 'drawCollison') then
        if (func_is_vbo(obj)) then
            func_error("vbo不支持!!!")
        end

        setv(obj, FLAGS_DRAW_RAY_COLLISION)
    end

end

local g_model;
local function f_listCallBack(self,index,param)
	--	list
	
	--local index = listbox_get_index(list);
	 --local _l = listbox;
		
	--print(g_model);

	if (g_model) then
		func_ptr_remove(g_model);
		--return;
	end


	-- local obj =func_loadobj("torus")
	local t = func_get_longTime();
	local obj = func_fixed_load()
	JEngine:getIns():bind_3dcam(obj);



	--print("&&&",obj);

	-- func_fixed_load()--func_loadmd5('wolf',0.01)--func_loadobj("torus")
	
	if (obj) then

		g_model = obj;
		
		--ex_addnode(obj);
		
		--            _model_ptr = g_model
		--local arr = func_split(config);
		local label = self:getLabelByIndex(index);--listbox_get_select_label(list);
		
		local s = string.format('index = %d\tlabel = [%s]\t	vbo:%s',index,label,tostring(func_is_vbo(obj)))

		print(s..","..(func_get_longTime() - t));

		f_setLabel(label, g_model);
	end
end

-- ###############################################################
-- 初始化listbox,用来测试不同的材质
local function f_shader_init()
	--local list = listbox_new(0,50);
		local list = NListBox:new(200,50,128);
	list:addItem("diffuse");
	list:addItem("ploygonLine");
	list:addItem("outline");
	list:addItem("point");
	list:addItem("drawCollison");
	list:addItem("glslOutline");
	list:addItem("normal");
	list:addItem("diffuse");
	list:addItem("材质测试");

	list:bind(f_listCallBack);

    return list;
end

-- #########################################################

-- cam:position(0,0,-2)
-- load_alpha_model()


local cam3d = JEngine:getIns():get_cam();
cam3d:set_pos(0,0,-10);
local list = f_shader_init();

-- listbox_select(list,0)--默认选择一个
