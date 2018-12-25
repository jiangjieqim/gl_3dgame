//使用EBO绘制矩形(两个三角形)
#include <gl/glew.h>
#include <gl/glut.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")

#include "tools.h"

void userInit();  //自定义初始化
void reshape(int w, int h);   //重绘
void display(void);
void keyboardAction(unsigned char key, int x, int y);   //键盘退出事件

GLuint eboId;//element buffer object句柄    
GLuint vboId;//vertext buffer object句柄    
GLuint vaoId;//vertext array object句柄    


//创建顶点数据    
const GLfloat vertices[] = {
	-0.5f,-0.5f,0.0f,1.0f,
	0.5f,-0.5f,0.0f,1.0f,
	0.5f,0.5f,0.0f,1.0f,
	-0.5f,0.5f,0.0f,1.0f,
};
// 索引数据
GLshort indices[] = {
	0, 1, 3,  // 第一个三角形
	1, 2, 3   // 第二个三角形
};

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Rectangle demo");

	//使用glew，需要执行glewInit，不然运行过程会报错
	//glewInit要放在glut完成了基本的初始化之后执行
	glewInit();

	//自定义初始化，生成VAO，VBO,EBO
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
	//printf("sizeof(indices) = %d,%d\n",sizeof(indices),sizeof(GLshort));

	glClearColor(0.0, 0.0, 0.0, 0.0);

	//创建VAO对象
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	//创建VBO对象，把顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//创建EBO对象	
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	//传入EBO数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//解释顶点数据方式
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//解绑VAO
	glBindVertexArray(0);
	//解绑EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//解绑VBO
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
	int	indices_num;
	
	indices_num = sizeof(indices)/sizeof(GLshort);//索引个数

	glClear(GL_COLOR_BUFFER_BIT);
	//绑定VAO
	glBindVertexArray(vaoId);
	//绘制模型
	glDrawElements(GL_TRIANGLES, indices_num, GL_UNSIGNED_SHORT, NULL);
	glutSwapBuffers();
}

//键盘按键回调函数    
void keyboardAction(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033:  // Escape key    
		exit(0);
		break;
	}
}