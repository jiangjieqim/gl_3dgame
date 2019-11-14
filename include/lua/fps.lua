local tf;
local isShow = false;
local DELAT_TIME = 1000;--刷新延迟
local ticket = 0;

local maxValue = 0;

--local _default = false;--是否使用旧的字体渲染

local function f_render(data,param)
	--if(tf)then		tf_setText(tf,string.format(%.3f),func_fps());end
	local _time = func_get_longTime()
	if(_time - ticket < DELAT_TIME) then
		return
	end
	
	ticket = _time
	if(tf) then
		local curFbs = engine_get_fps();
		if(curFbs > maxValue) then
			maxValue = curFbs;
		end
		local bytes ,emptycnt,allbytes= memory_get_info();
		local s = string.format("fps=%d延迟%d-%d-%d-[%d,%d,(_%.0f_kb)]",curFbs,engine_getDelayTime(),maxValue,_time/1000,
					bytes,emptycnt,allbytes / 1024);
        --if(_default) then
		--    tf_setText(tf,s);
        --else
        ftext_reset(tf,s);
        --end
		--print(s);
	end
	
	
	
	
	--print(data,param);
	
	--timelater_remove(param);
	--local timer = timelater_new(1000);

end

--显示fps
function fps(x,y)
	x = x or 0
	y = y or 0
   
   --[[ if(_default) then
	    if(tf == nil) then
		    tf = tf_create(128,x,y,0,0,0);
		    evt_on(tf,EVENT_ENGINE_RENDER_3D,f_render);
	    end
	    tf_setPos(tf,x,y)
    else--]]
        if(tf == nil) then
		    tf = ftext_create(128,128);
            ftext_set_buffer(tf,128);
		    --evt_on(tf,EVENT_ENGINE_RENDER_3D,f_render);
			local timer = timelater_new(1000);
			evt_on(timer,EVENT_TIMER,f_render,timer);
	    end
        local _stat = isShow == false and 1 or 0;
        ftext_vis(tf,_stat);
	    ftext_setpos(tf,x,y);
        isShow = not isShow;
   -- end
end

--##################################################################

FpsView = {label,timer,formatstr};

FpsView.__index = FpsView;
function FpsView:new()
	local self = {};
	setmetatable(self, FpsView);
	
	--tf = ftext_create(128,128);
    --ftext_set_buffer(tf,128);
	--self.tf = tf;
	self.label = label_create(64,64);
	return self;
end
local function f_fps_timer(data,param)
	local self = param;
	local _fps = engine_get_fps();
	local str = _fps;
	if(self.formatstr) then
		str = string.format(self.formatstr,_fps);
	end
	
	label_set_text(self.label,str);
end

function FpsView:dispose()
	if(self.timer) then
		timelater_remove(self.timer);
	end
	self:hide();
	label_dispose(self.label);	
end
function FpsView:show(x,y,formatstr)
	label_set_pos(self.label,x or 0,y or 0);
	label_set_visible(self.label,1);
	
	self.formatstr = formatstr;
	if(self.timer==nil) then
		local timer = timelater_new(1000);
		self.timer = timer;
		evt_on(timer,EVENT_TIMER,f_fps_timer,self);
	end
end

function FpsView:hide()
	--ftext_vis(self.tf,0);
	label_set_visible(self.label,0);
	local timer = self.timer;
	evt_off(timer,EVENT_TIMER,f_fps_timer);
end

--Fbs的容器
function FpsView:container()
	return self.label;
end

--local mc = FpsView:new();
--mc:show();
--mc:hide();
--mc:dispose();


--fps();