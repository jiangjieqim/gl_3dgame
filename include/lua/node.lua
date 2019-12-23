function node_new()
	local a = {
		node,
		material,
	};	
	return a;
end

function node_loadModel(avatar,url)
	url = url or "\\resource\\obj\\sphere.obj";--"\\resource\\md2\\bauul.md2";box
	local name = func_create_name();
	--print(name,url);
	avatar.node = func_ex_loadVBO(name,url);
	setv(avatar.node,FLAGS_VISIBLE);
end

function node_loadMaterial(avatar,url)
	url = url or "//resource//material//triangle.mat";
	avatar.material = func_load(url);
	setMaterial(avatar.node,avatar.material);
end

function node_getNode(avatar)
	return avatar.node;
end

function node_setScale(n,value)
	func_set_scale(n.node,value);
end

function node_setRX(n,value)
	   func_setRotateX(n.node,value)
end

function node_setRY(n,value)
	   func_setRotateY(n.node,value)
end

function node_setRZ(n,value)
	   func_setRotateZ(n.node,value)
end
--获取节点的名字
function node_getName(n)
	return func_get_name(n.node);
end
--获取节点所在的cam的矩阵空间
function node_setCam(n,value)
	--change_attr(o,"setcam",tostring(value));
	set_cam(n.node,value);
end

--**************************************************************************
--初始化动作(处理成动态加载配置文件,可以做一个编辑器编辑这些缩放和偏移有问题的md2文件)
local function f_split_init(md2)
	func_anim_push(md2,"stand",0,39);
	func_anim_push(md2,"run",40,45);
	func_anim_push(md2,"jump",66,71);
	func_set_anim(md2,"stand");
	func_play(md2);
end

local function add_small_node(parent)
   local cam = get_cam(parent);
    local sprite = sprite_create(nil,0,0,64,64,0,1,cam);
--	setv(sprite,FLAGS_DRAW_PLOYGON_LINE);
    func_setIcon(sprite, "smallbtn.png");
    func_addchild(parent,sprite);
    JEngine:getIns():add(sprite);
end