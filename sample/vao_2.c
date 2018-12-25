#include <GL/glew.h> 
#include <gl/glut.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")

#include <stdio.h>	

#define VERTICES 0
#define COLOR 1
#define INDICES 2
#define NUM_BUFFER 3
GLuint buffers[NUM_BUFFER];
float quard=0;

GLdouble sw = 300;
GLdouble sh = 240;

 GLfloat vertices[][3]={{0,0,50},//0
{100,0,50},//1
{100,100,50},//2
{0,100,50},//3
{0,0,-50},//4
{100,0,-50},//5
{100,100,-50},//6
{0,100,-50}//7
};

GLfloat colors[][3]={{1.0,0.2,0.2}, {0.2,0.2,1.0}, {0.8,1.0,0.2}, {0.75,0.75,0.75}, {0.35,0.35,0.35}, {0.5,0.5,0.5}, {0.3,0.4,0.5}, {0.5,0.6,0.7}};
GLubyte indices[][4]={{3,2,1,0},{7,6,5,4},{7,4,0,3},{6,5,1,2},{7,3,2,6},{0,4,5,1}};

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(-50,-50,-500);
	glRotatef(quard,1,1,0);
	glDrawElements(GL_QUADS,24,GL_UNSIGNED_BYTE,0);
	glFlush();
	glutSwapBuffers();
}
void rotate(void)
{
	quard+=0.1;
	glutPostRedisplay();
}

void mouse(int button,int state,int x,int y)
{
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		if(GLUT_DOWN==state)
		{
			glutIdleFunc(rotate);
		}else
		{
			glutIdleFunc(0);
		}
	}
}

void init()
{
	

	glClearColor(0.0,1.0,0.0,0.0);
	glClearDepth(1.0);
	glViewport(0,0,sw,sh);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-300,300,-100,380,-100,100);
	gluPerspective (45.0, sw/sh, 0.1, 10000);//10000表示景深大小

	glMatrixMode(GL_MODELVIEW);

	glewInit();

	glGenBuffers(NUM_BUFFER,buffers);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[VERTICES]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	glVertexPointer(3,GL_FLOAT,0,0);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER,buffers[COLOR]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);
	glColorPointer(3,GL_FLOAT,0,0);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffers[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);

	glutInitWindowSize(sw,sh);
	glutInitWindowPosition(100,100);
	glutCreateWindow("缓冲区对象");

	init();

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMainLoop();

	return 0;
}