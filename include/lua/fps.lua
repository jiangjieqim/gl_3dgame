local tf;
local DELAT_TIME = 1000;--Ë¢ÐÂÑÓ³Ù
local ticket = 0;

local function f_render()
	--if(tf)then		tf_setText(tf,string.format(%.3f),func_fps());end
	local _time = func_get_longTime()
	if(_time - ticket < DELAT_TIME) then
		return
	end
	
	ticket = _time
	if(tf) then
		local s = string.format("fps %d %d",ex:fps(),ex:delayTime())
		tf_setText(tf,s);
	end
end

--ÏÔÊ¾fps
function fps(x,y)
	x = x or 0
	y = y or 0
	if(tf == nil) then
		tf = tf_create(128,x,y,0,0,0);
		evt_on(tf,EVENT_ENGINE_RENDER_3D,f_render);
	end
	tf_setPos(tf,x,y)
end