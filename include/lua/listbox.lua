--------------------------------------------------------------------------------------
--下拉列表组件
ListBox={}

--显示隐藏文本
local function showList(listBox,v)
	if(listBox) then
		for i = 0, listBox.length-1 do
			--table.insert(arr,1,i)
			local tf=listBox.tflist[i];			
			
			if(v == 1) then
				setv(tf,flags.FLAGS_VISIBLE)
			else
				--if(i > 0) then
					resetv(tf,flags.FLAGS_VISIBLE);
				--end
			end
			
		end
	end
end

function ListBox.selectByIndex(listBox,index)
	if(index >=0 and listBox.clickEvtCallBack)	then
		listBox.selectIndex = index;
		
		--设置文本
		dofunc("tf_copy",listBox.tf,listBox.tflist[index]);
		local str =	dofunc("tf_get_str",listBox.tf);
		listBox.selectLabel = str;
		listBox.clickEvtCallBack(listBox);
	end
end

function listBoxCallBack(name)
	local listBox = instance[name];
	
	local size = listBox.width..","..listBox.gap;
	local isShowList = 0;
	if(listBox.isOpen) then
		size = listBox.width..","..tostring(listBox.gap*(listBox.length+1));
		isShowList = 1;
	end
	showList(listBox,isShowList);
	change_attr(listBox.sprite,"sprite_resize",size);
	
	------------
	local localX , localY=get_attr(listBox.sprite,"spriteLocalXY");
	--print(localX..","..localY);
	local index = localY/listBox.gap-1;
	--print(index)--取index的整数部分
	index = tl_getIntegralPart(index)
	
	ListBox.selectByIndex(listBox,index);
	
	listBox.isOpen=not listBox.isOpen;
end

function ListBox.new(_x,_y,callBack)
	local listBox=
	{
		length = 0;
		--list={};
		tflist={};
		sprite=nil;
		clickEvtCallBack = nil;
		gap = 15;	--(一行的间隔)
		width = 60; --(宽度)
		x=0;
		y=0;
		isOpen = 0;
		selectIndex=-1;
		selectLabel=nil;
		tf=nil;
	};
	listBox.x = _x;
	listBox.y = _y;
	local name = tableToInt(listBox);--tostring(sc);
	instance[name] = listBox;
	listBox.sprite=sprite_create(name,_x,_y,listBox.width,listBox.gap,"listBoxCallBack","","");
	listBox.clickEvtCallBack = callBack;
	
	func_setIcon(listBox.sprite,"gundi.png")
	
	

	listBox.tf=tf_create(128,_x,_y,1.0,1.0,1.0);
	
	return listBox;
end

function ListBox.push(p,str)
	--p.list[p.length] = str;
	local tf=tf_create(128,p.x,p.y+p.gap*(p.length+1),1.0,1.0,1.0);
	p.tflist[p.length] = tf;
	tf_setText(tf,str);
	
	resetv(tf,flags.FLAGS_VISIBLE);
	
	p.length=p.length+1;
end

local function disposeTflist(listBox,v)
	if(listBox) then
		for i = 0, listBox.length-1 do
			--table.insert(arr,1,i)
			local tf=listBox.tflist[i];			
			ptr_remove(tf);
		end
	end
end

function ListBox.dispose(listBox)
	disposeTflist(listBox);
	ptr_remove(listBox.sprite);
	ptr_remove(listBox.tf);
	listBox.list = nil;
	listBox = nil;
end