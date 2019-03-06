--创建文本对象
function func_ftext_create(name,fw,fh)
    fw = fw or 12;
    fh = fh or 11;
    return ftext(nil,"create",name,string.format("%s,%s",fw,fh));
end
--设置文本对象坐标
function func_ftext_setpos(txt,x,y)
    ftext(txt,"setpos",string.format("%s,%s",x,y));
end
--设置文本对象中的文本坐标
function func_ftext_setchar(txt,s,x,y)
    x = x or 0;
    y = y or 0;
    return ftext(txt,"setchar",s,string.format("%s,%s",x,y));
end
--显示单行连续的文本
function func_ftext_set_much_chars(ft,s,x,y)
    x = x or 0;
    y = y or 0;
    local arr =   func_split(s,",");
    local i = 0;
    local w,h;
    for key, value in pairs(arr) do		
		
	--	if(size) then
	--		print("table size = "..(getDddress(value) - size).." bytes\n")
	--	end
	
		
		--print("\n***** index = "..i..",key="..tostring(key)..",value="..tostring(value)..'-->'..getDddress(value).." *****")
		--func_printTable(value)
		i = i + 1
		
	--	size = getDddress(value)
		print(i,value);

        w,h = func_ftext_setchar(ft,value,x,y);
        x=x+w;
	end
end


FText = {
    str,--文本数据
};