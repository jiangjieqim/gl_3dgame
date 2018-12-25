//glsl遮罩
#pragma warning(disable:4996) 

#include <stdio.h>
#include <stdlib.h>

#include <gl/glew.h>
#include <gl/glut.h>

#include "tools.h"

#include <stdio.h>     
#include <stdlib.h>   

#pragma comment (lib,"glew32.lib")


static struct {
	GLuint vertex_buffer, element_buffer, color_buffer,textcoord_buffer;
	GLuint vertex_shader, fragment_shader, program;
	//用于保存CPU端的object名称
 
	struct {
		GLint position;
		GLint inColor;
		GLint textcoord;
	} attributes;
	//用于保存GPU端attribute变量的地址
} names;
 

static const GLfloat position_data[] = { 
	-1.0, -1.0,
	1.0, -1.0,
	1.0, 1.0,
	-1.0, 1.0
};
static const GLfloat color_data[] = {
	1.0, 0.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	0.0, 1.0, 1.0, 1.0
};

static const GLfloat textcoord_data[] = {
	0.0f,0.0f,
	1.0f,0.0f,
	1.0f,1.0f,
	0.0f,1.0f
};

static const GLushort element_data[] = { 0, 1, 2,3};
 
static void infoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
	GLint log_length;
	char *log;
 
	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log = (char *)malloc(log_length);
	glGet__InfoLog(object, log_length, NULL, log);
	fprintf(stderr, "%s", log);
	free(log);
}
 
void *readShader(const char *filename, GLint *length)
{
	FILE *f = fopen(filename, "r");
	void *buffer;
 
	if (!f) {
		fprintf(stderr, "Unable to open %s for reading\n", filename);
		return NULL;
	}
 
	fseek(f, 0, SEEK_END);
	*length = ftell(f);
	fseek(f, 0, SEEK_SET);
 
	buffer = malloc(*length+1);
	*length = fread(buffer, 1, *length, f);
	fclose(f);
	((char*)buffer)[*length] = '\0';
 
	return buffer;
}
 
static GLuint initShader(GLenum type, const char *filename)
{
	GLint length;
	GLchar *source = (GLchar *)readShader(filename, &length);
	GLuint shader;
	GLint shader_ok;
 
	if (!source)
		return 0;
 
	shader = glCreateShader(type);
	//创建shader object
	glShaderSource(shader, 1, (const GLchar**)&source, &length);
	//导入shader的代码
	//count - string的行数
	//length - 指向包含string每行字数的数组
	free(source);
	glCompileShader(shader);
	//编译shader代码
 
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	//查询shader的状态，导出可能的编译错误
	if (!shader_ok) {
		fprintf(stderr, "Failed to compile %s:\n", filename);
		infoLog(shader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(shader);
		getchar();
	}
	return shader;
}
 
static void installShaders(void)
{
	GLint program_ok;

	names.vertex_shader = initShader(GL_VERTEX_SHADER, "D:\\gl_source\\sample\\mask.vs");
	names.fragment_shader = initShader(GL_FRAGMENT_SHADER, "D:\\gl_source\\sample\\mask.fs");
 
	tl_memInit("64,100@512,200@8192,200@81920,200@262144,100@5242880,100");

	names.program = glCreateProgram();
	glAttachShader(names.program, names.vertex_shader);
	glAttachShader(names.program, names.fragment_shader);
	//把shader依附在同一个program上，以连接两个shader
	glLinkProgram(names.program);
	//链接program，在GPU端创建相应可执行文件，并初始化uniform变量及其地址
	glGetProgramiv(names.program, GL_LINK_STATUS, &program_ok);
	//查询program的状态，并导出可能的错误
	if (!program_ok) {
		fprintf(stderr, "Failed to link shader program:\n");
		infoLog(names.program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(names.program);
		getchar();
	}
	glUseProgram(names.program);
	//激活program后才能为shader指定uniform变量的值
}
unsigned int img = 0;
unsigned int mask_img = 0;
static void initBuffers(void)
{
	GLint t1,t2;
	names.attributes.position = glGetAttribLocation(names.program, "position");
	names.attributes.inColor = glGetAttribLocation(names.program, "inColor");
	names.attributes.textcoord = glGetAttribLocation(names.program, "textcoord");
	//获取GPU端attribute变量的地址保存在本地变量中，用于值的传递
 
	glGenBuffers(1, &names.vertex_buffer);
	//产生1个buffer object的名称，并分配显存空间
	glBindBuffer(GL_ARRAY_BUFFER, names.vertex_buffer);
	//把产生的buffer object与相应target绑定，以改变其值
	glBufferData(GL_ARRAY_BUFFER, sizeof(position_data), position_data, GL_STATIC_DRAW);
	//GL_STATIC_DRAW其他可用参数：
	//STATIC - 长时间不更改的值		DYNAMIC - 需要频繁改变的值		STREAM - 需要偶尔重写整个buffer的值
	//DRAW - 保存于GPU用于绘制的值		READ - 保存于CPU用于读取的值		COPY - 折衷
	glVertexAttribPointer(names.attributes.position, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, (void*)0);
	glEnableVertexAttribArray(names.attributes.position);
 
	glGenBuffers(1, &names.color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, names.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);
	glVertexAttribPointer(names.attributes.inColor, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)0);
	glEnableVertexAttribArray(names.attributes.inColor);

	glGenBuffers(1, &names.textcoord_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, names.textcoord_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textcoord_data), textcoord_data, GL_STATIC_DRAW);
	glVertexAttribPointer(names.attributes.textcoord, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
	glEnableVertexAttribArray(names.attributes.textcoord);
 
	glGenBuffers(1, &names.element_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, names.element_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(element_data), element_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, names.element_buffer);

	//纹理贴图
	t1 = glGetUniformLocation(names.program, "tex1");
	t2 = glGetUniformLocation(names.program, "tex2");

	img = jgl_loadTex("D:\\gl_source\\resource\\texture\\horse.bmp");
	mask_img = jgl_loadTex("D:\\gl_source\\resource\\texture\\maskTest.tga");//loadPNGTexture
	
	//printf("%d,%d\n",img,mask_img);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, img);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mask_img);

	glUniform1i(t1, 0);
	glUniform1i(t2, 1);
}
 
static void idleFunc(void)
{
}
 
static void displayFunc(void)
{
	glClearColor(0.5, 0.5, 0.5, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
 
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (void*)0);
 
	glutSwapBuffers();
}
 
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Hello World");
	glutIdleFunc(&idleFunc);
	glutDisplayFunc(&displayFunc);
 
	glewInit();
	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "OpenGL 2.0 not available\n");
		getchar();
	}
	//与glew扩展库相关的函数要在glewInit()后执行
	installShaders();
	initBuffers();
 
	glutMainLoop();
	return 0;
}