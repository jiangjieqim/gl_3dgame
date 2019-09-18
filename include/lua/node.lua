function node_new()
	local a = {
		node,
		material,
	};	
	return a;
end

function node_loadModel(avatar,url)
	url = url or "\\resource\\obj\\box.obj";--"\\resource\\md2\\bauul.md2";box
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

--**************************************************************************
--初始化动作(处理成动态加载配置文件,可以做一个编辑器编辑这些缩放和偏移有问题的md2文件)
local function f_split_init(md2)
	func_anim_push(md2,"stand",0,39);
	func_anim_push(md2,"run",40,45);
	func_anim_push(md2,"jump",66,71);
	func_set_anim(md2,"stand");
	func_play(md2);
end


function node_fbo(avtar)
	local n = node_new();

	local useAvtar = avtar or 0;
	
	if(useAvtar == 1) then
    --加载一个角色模型
		node_loadModel(n,"\\resource\\md2\\bauul.md2");  node_setScale(n,0.02);
		node_loadMaterial(n,"//resource//material//bauul.mat");
	else
		node_loadModel(n);
		node_loadMaterial(n,"//resource//material//triangle.mat");--bauul.mat
		setv(node_getNode(n),FLAGS_DRAW_PLOYGON_LINE);
		setv(node_getNode(n),FLAGS_REVERSE_FACE);--反转三角面
	end

	
	node_setRX(n,PI/2);
	f_split_init(n.node);
		--setv(node_getNode(n),FLAGS_REVERSE_FACE);--反转三角面
	
	engine_fboPushNode(node_getNode(n));
    
    local sprite = sprite_create("name1",10,10,100,50,0,1,engine_get_fbocam());
    func_setIcon(sprite, "smallbtn.png");
      
    --setv(sprite,FLAGS_VISIBLE);
    local z = engine_newPosZ();
    --print(z);
    func_sprite_set_z(sprite,z);
    engine_fboPushNode(sprite);
    --setv(sprite,FLAGS_REVERSE_FACE);--反转三角面 
    --setv(sprite,FLAGS_DISABLE_CULL_FACE);
    
    --engine_addNode(sprite);--增加到主舞台
	return sprite;
end