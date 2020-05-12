local core = require("core");

--一个飘动的字
local FlyLabel = {
	
};
FlyLabel.__index = FlyLabel;
setmetatable(FlyLabel, IPluginView);

function FlyLabel:new()
	local self = {
        nskin=nil,
        timer = 0,
        endtime = 0,
        sy = 0,
        ms = 0,--需要使用的时间
        starttime = 0,
        ty = 0,--目标y轴值
    };

    setmetatable(self, FlyLabel);
    local nskin = NSkin:new();
    self.nskin = nskin;
    
    --evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmleteHandler);
	nskin:load([[<ui name="1" type="NPanel" drag="1" center="1" width="200" height="20"/>
<ui name="info_label" label="1" type="NLabel" parent="1"/>
]],
"gundi.png");
	return self;
end

-- local function fc(self)
    -- self:dispose();
-- end
local function f_time(data,self)
    if(self.endtime<core.now()) then
        evt_off(self.timer,core.EVENT.TIMER,f_time);
        self:dispose();
    else
        local x,y = self.nskin:get_pos();
        local p = (core.now()-self.starttime)/self.ms;
        local ny = (self.sy - self.ty) * p;

        self.nskin:set_pos(x,self.sy - ny);--每次移动像素
    end
end
--v:设置的文本
--ms:延迟删除的毫秒数
--ty:移动到的目标Y轴坐标
function FlyLabel:set_label(v,ms,ty)
    ms = ms or 500;
    ty = ty or 0;
    local label = self.nskin:find("info_label");
    label:set_text(v);
    --print(EVENT_TIMER);
    -- local o = core.setTimeout(ms or 500,fc,self);
    local curtime = core.now();
    self.starttime = curtime;
    self.endtime = curtime + ms;
    self.timer = timelater_new(10);
    self.ms = ms;
    self.ty = ty;
    -- print(self.timer);


    -- local sx,sy = core.e:screen_size();
    local x,y = self.nskin:get_pos();
    -- print(sx,sy,x,y);
    self.sy = y;--起始y轴坐标

    evt_on(self.timer,core.EVENT.TIMER,f_time,self);
end

function FlyLabel:getName()
	return "FlyLabel";
end

function FlyLabel:dispose()
    self.nskin:dispose();
    func_clearTableItem(self);
end
return FlyLabel;