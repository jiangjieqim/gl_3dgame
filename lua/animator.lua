--动作管理器
Animator = {
	p,--	TYPE_OBJ_VBO_FILE引用
};
Animator.__index = Animator;
function Animator:new(p)
	local s = {};
    setmetatable(s,Animator);
	s.p = p;
    return s;
end

function Animator:dispose()
	func_clearTableItem(self);
end

--获取动作总数
function Animator:total()
	return change_attr(self.p,"animtor_total");
end

function Animator:cur_frame()
	return change_attr(self.p,"animtor_curFrame");
end

--是否在播放
function Animator:isPlaying()
	if(change_attr(self.p,"animtor_isPlaying") == 1) then
		return true;
	end
end
--设置fbs
function Animator:set_fps(v)
	change_attr(self.p,"fps",tostring(v))
end

--[[
	初始化动作(处理成动态加载配置文件,可以做一个编辑器编辑这些缩放和偏移有问题的md2文件)
	播放指定的动画
	"stand",0,39
	"run",40,45
	"jump",66,71
--]]
function Animator:play(anim)
	local o = self.p;
	--self:pause();
	if(anim) then
		change_attr(o,"animtor_setcur",anim);--指定当前的动作
	end
	change_attr(o,"animtor_play");
end

--分割动画
function Animator:push(animname,s,e)
	change_attr(self.p,"animtor_push",animname,string.format('%s,%s',s,e));
end

--根据区间播放动画
function Animator:play_to(s,e)
	change_attr(self.p,"animtor_play_start_end",string.format('%d,%d',s,e));
	self:play();
end

--暂停
function Animator:pause()
	change_attr(self.p,"animtor_pause");
end