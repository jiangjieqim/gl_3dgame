#include <gl/glew.h>
#include <gl/glut.h>
#include <tools.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")


int g_vertLen= 0;//顶点个数
GLfloat* g_vertex = 0;//顶点缓存区,需要释放

//struct GMaterial* g_mat = 0;//当前的材质

//加载一个四边形面片,返回一块缓存区引用,存储顶点数据
static GLfloat* loadquad(int* pLength)
{
	int vertexCount = 0;//顶点个数

	int _len=0;
	int verts = 0,obj = 0;
	char* _objStr;

	//printf("内存池:%d\n",tl_memByteSize());
	_objStr=tl_loadfile("\\resource\\obj\\quad.obj",0);
	
	obj_parse(_objStr,&_len,&verts,&obj);
	
	vertexCount = _len / 8;
	//printf("%d,%d,vertexCount=%d\n",verts,obj,vertexCount);

	//释放字符引用
	tl_free(_objStr);
	//tl_free(verts);

	//printf("内存池:%d\n",tl_memByteSize());

	*pLength = vertexCount;
	return (GLfloat*)verts;
}

//渲染一个四边形
static void drawQuad(int mode)
{
	//tmat_render(g_mat,0.0f,0.0f,0.0f,1.0f);

	//if(!mode){
		glPolygonMode (GL_BACK,GL_LINE);
	//}else{
	//	glPolygonMode (GL_BACK,GL_FILL);
	//}
	//printf("g_vertLen:%d , g_vertex:%d",g_vertLen,g_vertex);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glInterleavedArrays(GL_T2F_N3F_V3F,0,g_vertex);
	glDrawArrays(GL_TRIANGLES,0,(GLsizei)g_vertLen);
}

//////////////////////////////////////////////////
//global
float g_fWidth = 320;
float g_fHeight = 240;
float g_fDepth = 100;
float g_fAngle = .0;

struct Button{
	float m_fPosX;		//表示在正交投影坐标系(左下角为坐标原点)的坐标，
	float m_fPosY;
	float m_fWidth;		//屏幕像素单位
	float m_fHeight;
	int m_bPressed;
};

struct Button* pBtn;

void Render()
{
	glPushMatrix();
	
	//将中心位于原点的cube移动到使cube左下角坐标为m_fPosX,m_fPosY的位置
	//必须考虑cube的自身长宽
	glTranslatef(
		pBtn->m_fPosX+pBtn->m_fWidth/2, 
		pBtn->m_fPosY+pBtn->m_fHeight/2, -2.0);		//-2.0只是为了按钮可见

	if( pBtn->m_bPressed )
	{
		float _scale = 0.9;//点击时,按钮宽高比变化
		//double scaleAmt = 10.0 * sin( (double)rand() );
		//double scaleAmt = sin( (double)rand() );
		glScalef(pBtn->m_fWidth * _scale, pBtn->m_fHeight * _scale, 1.0);
		printf("%f,%f\n",pBtn->m_fWidth,pBtn->m_fHeight);
	}
	else{
		//cube中心位于原点
		glScalef (pBtn->m_fWidth, pBtn->m_fHeight, 5.0);  
	}

	//绘制了一个正投实体立方体,边长为1(注:这里使用绘制一个正四边形)
	//glutSolidCube(1.0);

	drawQuad(0);


	//quad.obj

}
int OnMouseDown(int mousex, int mousey)
{
	//鼠标的位置：mousex，mousey坐标系是原点位于左上角
	//必须将mousey变换到原点位于左下角的坐标系中
	mousey = g_fHeight-mousey;
	if( mousex > pBtn->m_fPosX && mousex < pBtn->m_fPosX+pBtn->m_fWidth &&
		mousey > pBtn->m_fPosY && mousey < pBtn->m_fPosY+pBtn->m_fHeight )
	{
		printf("button is pressed .... \n");
		pBtn->m_bPressed = 1;
		
		return 1;
	}
	return 0;
}
void OnMouseUp()
{
	pBtn->m_bPressed = 0;
}

void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	pBtn =	(struct Button*)malloc(sizeof(struct Button));//new Button;
	pBtn->m_bPressed = 0;
	pBtn->m_fPosX = 100;
	pBtn->m_fPosY = 50;
	pBtn->m_fWidth = 50;
	pBtn->m_fHeight = 20;
	printf("**********button pos: 40/t480\n");

	tl_memInit("64,100@512,200@8192,200@81920,200@262144,100@5242880,100");
	tl_memState(FALSE);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, g_fWidth, 0, g_fHeight, 0, g_fDepth);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Render();

	// 绘制cube物体，
	glMatrixMode (GL_PROJECTION);		//回复原有的设置
	glLoadIdentity ();
	gluPerspective(60,1.0,1.5,20);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();           
	/* viewing transformation  */
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	//glRotatef(g_fAngle, 0.0, 1.0, 0.0);
	glRotatef(g_fAngle, 1.0, 0.0, 0.0);
	//glRotatef(g_fAngle, 0.0, 0.0, 1.0);

	glScalef (1.0, 1.0, 1.0);      /* modeling transformation */ 
	//g_fAngle+=10.0f;
	
	//绘制一个线框渲染立方体
	//glutWireCube (0.5);
	
	//绘制一个四边形
	drawQuad(1);

	//glFlush();
	glutSwapBuffers();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60,1.0,1.5,20);
	glMatrixMode (GL_MODELVIEW);
	printf("g_fWidth = %f,g_fHeight = %f\n",g_fWidth,g_fHeight);
	g_fWidth = w;
	g_fHeight = h;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
case 27:
	exit(0);
	break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if(button==GLUT_LEFT_BUTTON)
	{
		switch(state)
		{
		case GLUT_DOWN:
			//左键按下：
			printf("Mouse pos : %d/t%d\n", x, g_fHeight-y);
			if( OnMouseDown(x, y) )
				g_fAngle += 2.0;
			break;

		case GLUT_UP:
			OnMouseUp();
			break;
		}
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	const char* shader = "diffuse";


	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (g_fWidth, g_fHeight); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc( mouse );


	tl_setLocalPath("d:\\gl_source");
	g_vertex = loadquad(&g_vertLen);
	
	//g_mat = tmat_create(2);
	//tmat_pushTex(g_mat,"\\resource\\texture\\timg.bmp");
	//g_mat->glslType = shader;

	//printf("g_vertLen=%d , %d\n",g_vertLen,g_mat);
	//创建VBO数据
	//f_createVBO(g_vertex,g_vertLen);


	glutMainLoop();
	return 0;
}
