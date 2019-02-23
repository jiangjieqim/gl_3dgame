local m_name = "triangle"

--初始化动作(处理成动态加载配置文件,可以做一个编辑器编辑这些缩放和偏移有问题的md2文件)
local function f_split_init(md2)
	
	if(m_name == "bauul")  then
		func_set_scale(md2,1/50);--这里设置一个缩放价值让模型在单位1以内
		func_set_y(md2,0.5);	 --设置地板y轴向偏移
		func_setRotateX(md2,PI/2)--旋转一个轴向
	end
	
	
	--print(md2);
	func_anim_push(md2,"stand",0,39);
	func_anim_push(md2,"run",40,45);
	func_anim_push(md2,"jump",66,71);
	func_set_anim(md2,"stand");
	func_play(md2);
end

--创建一个基本单位,默认在1个单位区间内
function unit_create()
	local u = {
		p = nil;--资源对象
	}
	--drfreak
	--bauul
	--triangle
	local url = m_name;
	
	local md2=load_VBO_model(func_create_name(),string.format("\\resource\\md2\\%s.md2",url));
	setMaterial(md2,func_load(string.format("//resource//material//%s.mat",url)));
	setv(md2,FLAGS_VISIBLE);
	
	

	f_split_init(md2);
	--setv(md2,FLAGS_RAY);
	--setv(md2,FLAGS_DRAW_RAY_COLLISION);
	setv(md2,FLAGS_DISABLE_CULL_FACE)--取消双面渲染
	----------------------------
	u.p = md2;
	return u;
end
--移动到目标
function unit_move_to(u,x,z,moveEndCallBack)
	
end