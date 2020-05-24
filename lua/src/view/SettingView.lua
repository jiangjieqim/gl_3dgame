local core = require("core");

-- local ENUM = require("enum");

local SettingView = {
	
};
SettingView.__index = SettingView;

setmetatable(SettingView, IPluginView);


local function onDisplay(self)
    
    -- -- e:get(ENUM.FPS);			--get_fps();
    --print( e:get(e.ENUM.FPS));
    -- print(self);


    -- local v =core.e:get(ENUM.FPS);
    -- self.nskin:find("info_label"):set_text(v);
    -- print('onDisplay...');
end
local function f_scale_handle(progress,self)
    local fps = math.ceil(progress*59) + 1;
    self.nskin:find("info_label"):set_text(fps);
    local e = JEngine:getIns();
    e:setfps(fps);
end

local function init(self)
    local namemap = self.nskin.namemap;
    local scale = namemap["scale"];
    scale:bindCallback(f_scale_handle,self);
    -- self:dispose();
    -- self:show();
end
function SettingView:new()
	local self = {
		nskin=nil,
    };
    
    local function f_cpmleteHandler(skin)
        init(self);
    end
	setmetatable(self, SettingView);
    local nskin = NSkin:new();
	self.nskin = nskin;
    evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmleteHandler);
    core.bindAddress(self);
    evt_on(self,core.EVENT.DISPLAY,onDisplay);

	nskin:load([[<ui name="1" type="NPanel" drag="1" center="1" width="200" height="30"/>
<ui name="scale" type="NScrollBar" x="0" y="0" parent="1"/>
<ui name="info_label" label="1" type="NLabel" x="128" y="0" parent="1"/>


]],


--		<ui name="pb1" type="ProgressBar" x="0" y="20" parent="1"/>

"gundi.png;checkbox.png;smallbtn.png");
	return self;
end


function SettingView:getName()
	return "SettingView";
end

function SettingView:dispose()
    self.nskin:dispose();
    func_clearTableItem(self);
end

-- function SettingView:onDisplay(v)
   
-- end



-- print("SettingView init loading==============>");
return SettingView;