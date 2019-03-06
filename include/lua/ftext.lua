--创建文本对象
function func_ftext_create(name)
    return ftext(nil,"create",name);
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

FText = {
    str,--文本数据
};