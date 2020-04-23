local core = require("core");

--一个飘动的字
local FlyLabel = {
	
};
FlyLabel.__index = FlyLabel;
setmetatable(FlyLabel, IPluginView);

function FlyLabel:new()
	local self = {
		nskin=nil,
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

local function fc(self)
    self:dispose();
end

--v:设置的文本
--ms:延迟删除的毫秒数
function FlyLabel:set_label(v,ms)
    local label = self.nskin:find("info_label");
    label:set_text(v);
    local o = core.setTimeout(ms or 1000,fc,self);
end

function FlyLabel:getName()
	return "FlyLabel";
end

function FlyLabel:dispose()
    self.nskin:dispose();
    func_clearTableItem(self);
end
return FlyLabel;