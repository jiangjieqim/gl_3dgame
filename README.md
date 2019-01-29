# gl_3dgame
基于Opengl的3d引擎,C做的底层接口,Lua封装的组件<br>
1.优化精简tools.h接口<br>
2.2019.1.24 vbo模式的渲染的无法拾取,勿忘修复之.<br>
3.为3d对象添加一个接口targetToVec(x,y);让对象转向一个指向向量.<br>
4.可以参考www.3dgamestudio.com的接口设计引擎.<br>
5.20:04 2019/1/29	example01:实现一个点击地面旋转角色移动到指定目的地.