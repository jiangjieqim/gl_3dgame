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
        evt_off(self.timer,EVENT_TIMER,f_time);
        self:dispose();
    else
        local x,y = self.nskin:get_pos();
        self.nskin:set_pos(x,y-5);--每次移动像素
    end
end
--v:设置的文本
--ms:延迟删除的毫秒数
function FlyLabel:set_label(v,ms)
    ms = ms or 500;
    local label = self.nskin:find("info_label");
    label:set_text(v);
    --print(EVENT_TIMER);
    -- local o = core.setTimeout(ms or 500,fc,self);
    local curtime = core.now();
    self.endtime = curtime + ms;
    self.timer = timelater_new(10);
    -- print(self.timer);
    evt_on(self.timer,EVENT_TIMER,f_time,self);
end

function FlyLabel:getName()
	return "FlyLabel";
end

function FlyLabel:dispose()
    self.nskin:dispose();
    func_clearTableItem(self);
end
return FlyLabel;