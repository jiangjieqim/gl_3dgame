//使用VBO绘制两个三角形
#include <GL/glew.h> 
#include <gl/glut.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")

void userInit();  //自定义初始化
void reshape(int w, int h);   //重绘
void display(void);
void keyboardAction(unsigned char key, int x, int y);   //键盘退出事件

GLuint vboId=0;//vertex buffer object句柄    
//GLuint vaoId;//vertext array object句柄    
GLuint programId;//shader program 句柄    

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(100, 100);
	glutCreateWindow("Rectangle demo");

	//使用glew，需要执行glewInit，不然运行过程会报错
	//glewInit要放在glut完成了基本的初始化之后执行
	glewInit();

	//自定义初始化，生成VAO，VBO对象
	userInit();

	//重绘函数
	glutReshapeFunc(reshape);

	glutDisplayFunc(display);

	//注册键盘按键退出事件
	glutKeyboardFunc(keyboardAction);

	glutMainLoop();

	return 0;
}

//自定义初始化函数    
void userInit()
{
	//创建顶点数据    
	const GLfloat vertices[] = {
		/*-0.5f,-0.5f,0.0f,1.0f,
		0.5f,-0.5f,0.0f,1.0f,
		0.5f,0.5f,0.0f,1.0f,
		-0.5f,0.5f,0.0f,1.0f,*/

		
	
		/*两个三角形*/
		0.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		
		0.0f,1.0f,0.0f,
		0.75f,0.75f,0.0f,
		1.0f,0.0f,0.0f,
	};
	printf("按键盘a删除VBO对象\n");

	glClearColor(0.0, 0.0, 0.0, 0.0);

	//创建VAO对象
	/*glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);*/

	printf("未创建之前vboId= %d\n",vboId);
	//创建VBO对象	
	glGenBuffers(1, &vboId);
	printf("创建完之后vboId= %d\n",vboId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//传入VBO数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//解除VBO绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

//调整窗口大小回调函数    
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

//绘制回调函数    
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

	//绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);//解释顶点数据方式
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);//绘制模型
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);//解释顶点数据方式
	glDrawArrays(GL_TRIANGLES, 0, 6);//绘制6个顶点

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

	glutSwapBuffers();
	glutPostRedisplay ();
}

//键盘按键回调函数    
void keyboardAction(unsigned char key, int x, int y)
{
	int kv = (int)key;
	
	switch (key)
	{
		case 033:  // Escape key    
			exit(0);
			break;
	}
	if(kv==97){
		//按a释放VBO,删除模型数据
		//printf("glDeleteBuffers %d\n",vboId);
		glDeleteBuffers(1,&vboId);
		//printf("glDeleteBuffers %d\n",vboId);
	}
}