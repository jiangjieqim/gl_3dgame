//用VBO,VAO综合实例.可以参考扩展

#include <gl/glew.h>
#include <gl/glut.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")

#include <stdio.h>

#include "jgl.h"

GLchar* vs = 
"#version 330\n "
"in vec3 Position;"
"in vec3 Color ;"
"in vec2 TexCoord ;"
"uniform mat4 gWVP;"
"out vec3 out_color;"
"out vec2 out_texcoord;"

"void main(){"
"	gl_Position = gWVP*vec4(Position, 1.0);"
"	out_color = Color;"
"	out_texcoord = TexCoord;"
"}";

GLchar* ps = 
"#version 330\n "
"out vec4 FragColor;"
"in vec3 out_color;"
"in vec2 out_texcoord;"
"uniform sampler2D gSampler;"

"void main(){"
"	vec4 c= texture2D(gSampler,out_texcoord);"
"	FragColor = vec4(out_color+c.rgb, 1.0);"
"}";

const char* bmppath = "d:\\gl_source\\resource\\texture\\tex.bmp";

GLuint vShader,fShader;//顶点着色器对象

//顶点位置数组
float positionData[] = {
	-50,-50,0.5f,
	50,-50,0.5f,
	-50,50,0.5f,
	50,50,0.5f};
float uvData[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f,1.0f,
	1.0f,1.0f};
GLuint indexs[]={0,1,2,2,3,1};
//颜色数组
float colorData[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f};


float l= -100.0f,r=100.0f,t=100.0f,b=-100.0f,n=-100.0f,f=100.0f;

float mat[16];

GLuint  samplerA,gWVPLocation,textureID,VertexID,UVID,ColorID,IndexID;
GLuint VAO;

void initShader(const char *VShaderFile,const char *FShaderFile)
{
	GLchar *vShaderCode = vs;
	const GLchar *vCodeArray[1] = {vShaderCode};
	GLint compileResult;

	//查询链接的结果
	GLint linkStatus;

	const GLchar *fShaderCode = ps;
	const GLchar *fCodeArray[1] = {fShaderCode};

	//1、查看GLSL和OpenGL的版本
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	
	GLuint programHandle;

	const GLubyte *glslVersion = 
		glGetString( GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	//cout << "GL Vendor    :" << vendor << endl;
	//cout << "GL Renderer  : " << renderer << endl;
	//cout << "GL Version (string)  : " << version << endl;
	//cout << "GL Version (integer) : " << major << "." << minor << endl;
	//cout << "GLSL Version : " << glslVersion << endl; 

	//2、编译着色器
	//创建着色器对象：顶点着色器
	vShader = glCreateShader(GL_VERTEX_SHADER);
	//错误检测
	if (0 == vShader)
	{
		/*cerr << "ERROR : Create vertex shader failed" << endl;*/
		printf("ERROR : Create vertex shader failed\n");
		exit(1);
	}

	//把着色器源代码和着色器对象相关联
	//vShaderCode = textFileRead(VShaderFile);
	
	glShaderSource(vShader,1,vCodeArray,NULL);

	//编译着色器对象
	glCompileShader(vShader);


	//检查编译是否成功
	
	glGetShaderiv(vShader,GL_COMPILE_STATUS,&compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度
		glGetShaderiv(vShader,GL_INFO_LOG_LENGTH,&logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			//得到日志信息并输出
			glGetShaderInfoLog(vShader,logLen,&written,log);
			//cerr << "vertex shader compile log : " << endl;
			//cerr << log << endl;

			printf("vertex shader compile log : %s\n",log);

			free(log);//释放空间
		}
	}

	//创建着色器对象：片断着色器
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//错误检测
	if (0 == fShader)
	{
		printf( "ERROR : Create fragment shader failed" );
		exit(1);
	}

	//把着色器源代码和着色器对象相关联
	
	glShaderSource(fShader,1,fCodeArray,NULL);

	//编译着色器对象
	glCompileShader(fShader);

	//检查编译是否成功
	glGetShaderiv(fShader,GL_COMPILE_STATUS,&compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度
		glGetShaderiv(fShader,GL_INFO_LOG_LENGTH,&logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			//得到日志信息并输出
			glGetShaderInfoLog(fShader,logLen,&written,log);
			/*cerr << "fragment shader compile log : " << endl;
			cerr << log << endl;*/
			printf("fragment shader compile log :%s\n",log);
			free(log);//释放空间
		}
	}

	//3、链接着色器对象
	//创建着色器程序
	programHandle = glCreateProgram();
	if (!programHandle)
	{
		printf( "ERROR : create program failed\n");
		exit(1);
	}
	//将着色器程序链接到所创建的程序中
	glAttachShader(programHandle,vShader);
	glAttachShader(programHandle,fShader);
	glBindAttribLocation(programHandle, 0, "Position");
	glBindAttribLocation(programHandle, 1, "TexCoord");
	glBindAttribLocation(programHandle, 2, "Color");
	
	//将这些对象链接成一个可执行程序
	glLinkProgram(programHandle);
	
	glGetProgramiv(programHandle,GL_LINK_STATUS,&linkStatus);
	if (GL_FALSE == linkStatus)
	{
		GLint logLen;

		printf( "ERROR : link shader program failed\n" );
		
		glGetProgramiv(programHandle,GL_INFO_LOG_LENGTH,
			&logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle,logLen,
				&written,log);
			

			printf("Program log : %s\n",log);
		}
	}
	else//链接成功，在OpenGL管线中使用渲染程序
	{
		gWVPLocation = glGetUniformLocation(programHandle,"gWVP");
		//assert(gWVPLocation != 0xFFFFFFFF);
		glUseProgram(programHandle);
		samplerA = glGetUniformLocation(programHandle,"gSampler");
		glUniform1i(samplerA, 0);
	}
}

void initVBO()
{
	//glGenVertexArrays(1,&VAO);
	//glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);//顶点坐标
	glEnableVertexAttribArray(1);//顶点uv
	glEnableVertexAttribArray(2);//顶点color
	glGenBuffers(1, &VertexID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionData),positionData,GL_STATIC_DRAW);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &UVID);
	glBindBuffer(GL_ARRAY_BUFFER,UVID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(uvData),uvData,GL_STATIC_DRAW);
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	glGenBuffers(1, &ColorID);
	glBindBuffer(GL_ARRAY_BUFFER,ColorID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(colorData),colorData,GL_STATIC_DRAW);
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glGenBuffers(1, &IndexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs),indexs,GL_STATIC_DRAW);
}
void initTexture()
{
	/*GLsizei w,h;
	GLubyte* image=tl_bmp24(bmppath,&w,&h);
	textureID=tlgl_createTex(w,h,image);
	tl_free((void*)image);*/

	//printf("[%s]\n",bmppath);
	textureID = jgl_loadTex(bmppath);

	//
}
void init()
{
	//初始化glew扩展库
	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		printf("%s\n",glewGetErrorString(err));
	} 
	initVBO();
	initShader("basic.vert","basic.frag");
	glUniformMatrix4fv(gWVPLocation, 1, GL_FALSE, (const GLfloat*)mat);
	initTexture();
	glClearColor(0.0,0.0,0.0,0.0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexID);
	//glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
	
	//glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindTexture(GL_TEXTURE_2D,0);
	
	glutSwapBuffers();
}

int main(int argc,char** argv)//,int argc, _TCHAR* argv[]
{
	mat[0] = 2/(r-l);
	mat[1] = 0;
	mat[2] = 0;
	mat[3] = -(r+l)/(r-l);
	mat[4] = 0;
	mat[5] = 2/(t-b);
	mat[6] = 0;
	mat[7] = -(t+b)/(t-b);
	mat[8] = 0;
	mat[9] = 0;
	mat[10] = 2/(n-f);
	mat[11] = (n+f)/(n-f);
	mat[12] = 0;
	mat[13] = 0;
	mat[14] = 0;
	mat[15] = 1;

	tl_memInit("64,100@512,200@8192,200@81920,200@262144,100@5242880,100");
	glutInit(&argc,(char**)argv);  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);  
	glutInitWindowSize(400,300);  
	glutInitWindowPosition(100,100);  
	glutCreateWindow("GLSL Test : Draw a triangle"); 
	init(); 
	glutDisplayFunc(display);  
	glutMainLoop();  
	return 0;  
}

