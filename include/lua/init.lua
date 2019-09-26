dofile("..\\include\\lua\\core.lua");

--########################################
--node_fbo(0);

--*********************************************
local itemHeight = 30;
local cam;
local function f_create()
	local sprite = sprite_create(nil,0,0,99,itemHeight,0,1,cam);

    local node ={};
--	setv(sprite,FLAGS_DRAW_PLOYGON_LINE);
    func_setIcon(sprite, "smallbtn.png");
    engine_addNode(sprite);


	if(true) then
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
   
    return node;
end

local function itemRefreshFunc(node)
--    print(node.data);
	if(node.tf) then
		fext_clear(node.tf);
		ftext_parse(node.tf,node.data);
	end
--	print("设置数据",node.data);
end

local t = {10,20,30,40,50,60,70,80,90,100,110,120};
local sv = scrollView_init(100,itemHeight*4,20,30);
sv.itemFunc = f_create;--设置itemRende的创建回调
sv.itemRefreshFunc = itemRefreshFunc;
cam = scrollView_get_cam(sv);
scrollView_set_data(sv,t);
--*******************************************

function f_onkey(data)
	local key = tonumber(data);
	print("key = "..key);
	if(key == KEY_ESC) then
         engine_exit();
	elseif(key == 13) then
		func_ex_info();
	elseif(key == 49) then
		func_ex_info();
	
		--setv(btnspr,FLAGS_DRAW_PLOYGON_LINE );
		--setv(btnspr,FLAGS_DISABLE_CULL_FACE );
		if(btnspr) then
			if(getv(btnspr,FLAGS_REVERSE_FACE) == 1) then
				resetv(btnspr,FLAGS_REVERSE_FACE );
			else
				setv(btnspr,FLAGS_REVERSE_FACE );
			end
		end
	elseif(key == 50) then
		if(btnspr) then
			setv(btnspr,FLAGS_DRAW_PLOYGON_LINE );
		end
	end
end
evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);

engine_setBg(0.3,0.3,0.3);

--fps(0,15);
--infowin(0,0);

--创建一个输入组件
local function f_init_input()

	--作为输入框的背景
	local sprite =sprite_create("bg11",100,50,128,14);
	func_setIcon(sprite,"gundi.png");
	--setv(sprite,FLAGS_DRAW_PLOYGON_LINE);


	--local sprite1 =func_sprite_createNoHit(100,100,256,256);
	--func_setIcon(sprite1,"gundi.png");
	--setv(sprite1,FLAGS_DRAW_PLOYGON_LINE);

	local _in = input_create();
	input_set_pos(_in,100,50);
	local function f_onkey(data)
		local key = tonumber(data);
		--print('my key = '..key);
		if(key == 13) then
			--print(input_isFocusIn(_in));
			if(input_isFocusIn(_in)) then
				print("inout str = ["..input_get_str(_in).."]");
				input_clear(_in);
			end
		elseif(key == KEY_B)then
		   
		elseif(key == KEY_ESC) then
			
		end
	end
	evt_on(_in,EVENT_ENGINE_KEYBOARD,f_onkey);

end

--f_init_input();


--alert("aa");


	






--input_dispose(_in);
--func_fixed_load("\\resource\\obj\\torus.obj");
--dofile("..\\include\\lua\\quaternion.lua");--

--btn_create(0,0,128,128);


--dofile("..\\include\\lua\\7.lua");



--//汉字第一个字节的范围
--static int
--f_is_first_code(unsigned char c){
--	return (0x80 <= c) && (0xF7 >= c);
--}
--//汉字第二个字节的范围
--static int
--f_is_second_code(unsigned char c){
--	return (0xA1 <= c) && (0xFE > c);
--}


--print(0x80,0xF7,0xA1,0xFE);


--local t = func_get_longTime();
--for i=1,1000000,1 do

--    math.random();
--end
--print(func_get_longTime()-t);


engine_refreshCam3d();