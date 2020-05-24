local core = require("core");

-- local ENUM = require("enum");
---@class AvatarView :IPluginView
local AvatarView = {
	
};
AvatarView.__index = AvatarView;

setmetatable(AvatarView, IPluginView);

-- local function f_load(o,cam)
--     local url = o.url;
--     local mat = o.mat;
--     local m = UnitBase:new();
--     m:loadvbo(url,mat,cam);
--     m:scale(o.scale);
--     m:set_position(o.x,o.y,o.z);
-- end

function AvatarView:refreshView()
    local o = self.data;
    -- print(debug.traceback());
--    print(o);
    if(o) then
       
        if(self.m==nil) then
            local cam = self.fbo:get_cam3d();
            
            -- f_load(o,cam);

            -- cam = nil;
            local url = o.url;
            local mat = o.mat;
            local m = UnitBase:new();
            m:loadvbo(url,mat,cam);
            m:scale(o.scale);

            m:set_position(o.x,o.y,o.z);
            self.mScale = o.scale;
            self.m = m;
        end
    end

end
local function onDisplay(self)
    if(self.fbo) then
        self.fbo:visible(true);
    end
end
local function onUnDisplay(self)
    if(self.fbo) then
        self.fbo:visible(false);
    end
end

local function f_rx_handle(progress,self)
    -- local fps = math.ceil(progress*59) + 1;
    -- self.nskin:find("info_label"):set_text(fps);
    -- local e = JEngine:getIns();
    -- e:setfps(fps);

    -- print(math.pi);
    --  print(progress*PI);

    --self.m:rz(math.pi * progress);
    self.m:scale(progress * self.mScale);
end
local function btnClicktri2(self)
    -- self.m:rx( -math.pi/2);
    local a = self.m:get_angle();
    self.m:rz(a + math.pi * 0.125);

    -- local bg = self.nskin:find("bg");
    -- bg:visible(true);
end
local function btnClicktri3(self)
    -- self.m:rx( -math.pi/2);
    local a = self.m:get_angle();
    self.m:rz(a - math.pi * 0.125);
end

--根据bg(Shape)创建Fbo
local function f_createFboByShape(self)
    
    --构造一个FBO渲染区域的Sprite
    local bg = self.nskin:find("bg");
    -- bg:drawPloygonLine(true);
    local x,y = self.nskin:getLocalPos(bg);
    local w,h = bg:get_size();
    -- print(x,y);
    local fbo = FboRender:new(w,h);
    self.fbo = fbo;
    
    local c = self.nskin:get_panel():get_container();
    func_addchild(c, self.fbo:get_container(),x,y);
end

function AvatarView:initialize()
    local namemap = self.nskin.namemap;
    local scale = namemap["rotate"];
    scale:bindCallback(f_rx_handle,self);

    -- self:dispose();
    -- self:show();
    f_createFboByShape(self);

    local tri2 =self.nskin:find("tri2");
    tri2:bind_click(btnClicktri2,self);
    
    local tri3 =self.nskin:find("tri3");
	tri3:bind_click(btnClicktri3,self);
end

function AvatarView:new()
	local self = {
        nskin=nil,
        fbo = nil,
        m = nil,
        mScale = nil,--初始化的模型缩放值
    };
    
    -- local function f_cpmleteHandler(skin)
    --     init(self);
    -- end

    setmetatable(self, AvatarView);
    
    -- local nskin = NSkin:new();
    -- print("AvatarView:new====>",nskin:isSkinLoaded());
	-- self.nskin = nskin;
    -- evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmleteHandler);

    core.bindAddress(self);
    evt_on(self,core.EVENT.DISPLAY,onDisplay);
    evt_on(self,core.EVENT.UNDISPLAY,onUnDisplay);
    
	self:load(
[[
<ui name="1" type="NPanel" drag="1" center="1" width="200" height="300"/>
<ui name="bg" type="Shape" x="10" y="0" w="180" h="180" r="0" g="0" b="0"  parent="1"/>
<ui name="rotate" type="NScrollBar" x="10" y="180" parent="1"/>
<ui name="tri2" type="NButton" x="0" y="200" parent="1" url="tri2.png" w="15" h="15" rotatez="3.14"/>
<ui name="tri3" type="NButton" x="20" y="200" parent="1" url="tri2.png" w="15" h="15"/>
]]
,"gundi.png;checkbox.png;smallbtn.png"
);

--[[

<ui name="bg" type="Shape" x="10" y="0" w="180" h="180" r="0" g="0" b="0"  parent="1"/>
<ui name="rotate" type="NScrollBar" x="10" y="180" parent="1"/>
<ui name="tri2" type="NButton" x="0" y="200" parent="1" url="tri2.png" w="15" h="15" rotatez="3.14"/>

--]]

--      "gundi.png;checkbox.png;smallbtn.png"
--<ui name="info_label" label="1" type="NLabel" x="128" y="0" parent="1"/>
--<ui name="pb1" type="ProgressBar" x="0" y="20" parent="1"/>

--);
	return self;
end

function AvatarView:getName()
	return "AvatarView";
end

function AvatarView:dispose()
    self.fbo:dispose();
    self.nskin:dispose();
    func_clearTableItem(self);
end

return AvatarView;