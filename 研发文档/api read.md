### glutSwapBuffers,glFlush
简单地说glFlush就是强制刷新吧，要知道OPENGL是使用一条渲染管线线性处理命令的，一般情况下，我们提交给OPENGL的指令并不是马上送到驱动程序里执行的，而是放到一个缓冲区里面，等这个缓冲区满了再一次过发到驱动程序里执行；很多时候只有几条指令是填充不满那个缓冲区的，这就是说这些指令根本没有被发送到驱动里，所以我们要调用glFlush来强制把这些指令送到驱动里进行处理。
glutSwapBuffers其实就是交换缓冲区啦，估计是你的程序使用的是双缓冲技术吧；用了这种技术的话，你所有的绘制都是绘制到一个后台的缓冲区里面的，如果不交换缓冲区，就看不到绘制的东西了，就这么回事~
```
glutPostRedisplay
//显示
glutDisplayFunc(void (*func)(void));　　

//改变窗口大小时调用
glutReshapeFunc(void (*func)(void));

//键盘事件，key指按下的键，x和y表示按下key键时鼠标的位置
glutKeyboardFunc(void (*func)(unsigned char key,int x,int y));　　

//鼠标点击事件,button表示鼠标的某个键,state指当前键的状态(按下或释放),x和y表示鼠标位置
glutMouseFunc(void (*func)(int button,int state,int x,int y));　　

//鼠标点击且移动时调用
glutMotionFunc(void (*func)(int x,int y));　

//鼠标移动时调用　
glutPassiveMotionFunc(void (*func)(int x,int y));　　

//空闲时调用
glutIdleFunc(void (*func)(void));　　
```
### FBO,RBO
FBO:Frame Buffer Object
RBO:Render Buffer Object
一个FBO可以绑定多个RBO,渲染缓冲区对象是应用程序分配的2D图像缓冲区。
渲染缓冲区被用来存储颜色、深度或模板值，能作为颜色、深度或模板关联到一个帧缓冲区对象。
渲染缓冲区和操作系统提供的离屏的绘制窗口pbuffer 有很多相似的地方。渲染缓冲区不能直接被GL 作为纹理使用。
FBO是一个颜色，深度和模板纹理或渲染目标的集合。各种2D 图像能被关联到帧缓冲区对象的颜色点上。
