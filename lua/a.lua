	--local old =  collectgarbage("count");
	--func_print("==> GC之前 LUA VM使用的总内存数:"..(collectgarbage("count")*1024).." bytes");
	--collectgarbage("collect");
	--local n = collectgarbage("count");


--require("listbox_get_select_labelcore");

function testfun()
	print("hello world");
end
testfun();

