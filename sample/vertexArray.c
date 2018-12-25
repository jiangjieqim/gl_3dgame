//
//  main.cpp
//  OpenGL_05_VertexArray
//
//  Created by apple on 14/12/30.
//  Copyright (c) 2014年 cc. All rights reserved.
//

#include <GL/glew.h> 
#include <gl/glut.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glut32.lib")

#define POINTER 1           //使用分别指定坐标和颜色顶点数组方式
#define INTERLEAVED 2       //使用混合指定坐标和颜色顶点数组方式


#define DRAWARRAY 1         //使用DrawArray方式
#define ARRAYELEMENT  2     //使用ArrayElement方式
#define DRAWELEMENTS 3      //使用DrawElement方式

//当前设置的模式
int setupMethod = POINTER;
int derefMethod = DRAWARRAY;

/**
 *  设置顶点坐标数组和顶点颜色数组(分别指定)
 */
void setupPointers() {
    
    //顶点坐标数组
    static GLint vertices[] = {25, 25,
        100, 325,
        175, 25,
        175, 325,
        250, 25,
        325, 325
    };
    
    //顶点颜色RBG数组
    static GLfloat colors[] = {1.0, 0.2, 0.2,
        0.2, 0.2, 1.0,
        0.8, 1.0, 0.2,
        0.75, 0.75, 0.75,
        0.35, 0.35, 0.35,
        0.5, 0.5, 0.5
    };
    
    //启用顶点坐标数组
    glEnableClientState(GL_VERTEX_ARRAY);
    //启用顶点颜色数组
    glEnableClientState(GL_COLOR_ARRAY);
    
    //指定顶点坐标数据
    glVertexPointer(2, GL_INT, 0, vertices);
    //指定顶点颜色数据
    glColorPointer(3, GL_FLOAT, 0, colors);
    
}

/**
 *  设置顶点坐标和顶点颜色数组(混合数组)
 */
void setupInterleave() {
    
    /**
     *  intertwined[0~2]* n 为顶点坐标
     *  intertwined[3~5]* n 为顶点颜色
     */
    static GLfloat intertwined[] = {
        1.0, 0.2, 1.0, 100.0, 100.0, 0.0,
        1.0, 0.2, 0.2, 0.0, 200.0, 0.0,
        1.0, 1.0, 0.2, 100.0, 300.0, 0.0,
        0.2, 1.0, 0.2, 200.0, 300.0, 0.0,
        0.2, 1.0, 1.0, 300.0, 200.0, 0.0,
        0.2, 0.2, 1.0, 200.0, 100.0, 0.0
    };
    
    //指定混合顶点数组
    glInterleavedArrays (GL_C3F_V3F, 0, intertwined);
}

/**
 *  初始化操作
 */
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //设置着色模式
    //GL_SMOOTH 制定的两点颜色进行插值，绘制之间的其他点
    //如果两点的颜色相同，使用两个参数效果相同
    //如果两点颜色不同，会出现过渡效果
    glShadeModel(GL_SMOOTH);
    //设置顶点数组，分别指定颜色和坐标
    setupPointers();
}

/**
 *  展示绘制效果
 */
void display(void) {
    
    //清理颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (derefMethod == DRAWARRAY) {
        //从顶点数组中的第一个顶点开始连续取6个顶点，绘制三角形
        glDrawArrays(GL_TRIANGLES, 0, 6);
    } else if (derefMethod == ARRAYELEMENT) {
        //取顶点坐标数组中的任意3个点进行绘制三角形
        glBegin(GL_TRIANGLES);
        glArrayElement(2);
        glArrayElement(3);
        glArrayElement(5);
        glEnd();
    } else if (derefMethod == DRAWELEMENTS) {
        //指定需要绘制的顶点的索引，绘制多边形
        GLuint indices[4] = {0, 1, 3, 4};
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, indices);
    }
    
    glFlush ();
    
}

/**
 *  调整窗口尺寸
 *
 *  @param width  宽度
 *  @param height 高度
 */
void reshape(int width, int height) {
    //设置视口矩形区域，在默认情况下，视口被设置为占据打开窗口的整个像素矩形
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    //对投影矩阵应用随后的矩阵操作
    glMatrixMode(GL_PROJECTION);
    //等于是将之前矩阵变换导致变化过的栈顶矩阵重新归位，置为单位矩阵！等于是之前的矩阵变换带来的影响到此为止了！
    glLoadIdentity();
    //指定2D裁剪坐标系，naer和far使用默认值-1和1
    gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);
}

/**
 *  键盘事件回调
 *
 *  @param key 键位
 *  @param x   x坐标
 *  @param y   y坐标
 */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
            //ESC
        case 27:
            exit(0);
            break;
    }
}

/**
 *  鼠标事件回调
 *
 *  @param button 按钮类型
 *  @param state  点击状态
 *  @param x      x坐标
 *  @param y      y坐标
 */
void mouse (int button, int state, int x, int y) {
    switch (button) {
        //左键
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                //点击左键，(使用颜色和坐标分离的数组方式) 和 (使用混合数组的方式) 切换
                if (setupMethod == POINTER) {
                    setupMethod = INTERLEAVED;
                    setupInterleave();
                } else if (setupMethod == INTERLEAVED) {
                    setupMethod = POINTER;
                    setupPointers();
                }
                //重绘
                glutPostRedisplay();
            }
            break;
        //右键
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                if (derefMethod == DRAWARRAY) {
                    derefMethod = ARRAYELEMENT;
                } else if (derefMethod == ARRAYELEMENT) {
                    derefMethod = DRAWELEMENTS;
                } else if (derefMethod == DRAWELEMENTS) {
                    derefMethod = DRAWARRAY;
                }
                //重绘
                glutPostRedisplay();
            }
            break;
        default:
            break;
    }
}

int main(int argc, const char * argv[]) {
    
    //初始化GLUT库
    glutInit(&argc, (char**)argv);
    //设置单缓冲，RGB像素格式的窗口
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //设置窗口大小
    glutInitWindowSize(350, 350);
    //设置窗口坐标
    glutInitWindowPosition (100, 100);
    //创建窗口
    glutCreateWindow("VertexArray");
    
    //初始化操作
    init();
    
    //设置展示的回调方法
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    //绘制线程开始循环
    glutMainLoop();
    
    return 0;
}