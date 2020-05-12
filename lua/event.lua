event = {
	DISPLAY = 1,--显示
    UNDISPLAY = 2,--隐藏
    COMPLETE = 3,--完成事件

    ---* 计时器evtid=21
    ---* 该计时器从C内核层evt_dispatch过来的事件,我们只在Lua模块这边做监听.
    TIMER = 201,
}
return event;