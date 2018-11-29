﻿//配置工作：为支持头文件x64版<gl/glut.h>，需要在openGL官网下载freeglut压缩包，头文件放指定位置C:\Program Files (x86)\Windows Kits\8.1\Include\um\gl
//			lib文件夹复制到当前工程目录，然后在\链接器\附加依赖中加上‘.\lib’，\链接器\输出加上‘freeglut.lib’，最后在x64\Debug\复制‘freeglut.dll’

#include <gl/glut.h>
#include <Windows.h>
#include "Matrices.h"
#include "Vectors.h"

#define PI 3.1415926535898

bool mouseLeftDown, mouseRightDown, mouseMiddleDown;
int mouseX, mouseY;
float cameraAngleX, cameraAngleY;
const float CAMERA_DISTANCE = 5.0f;
float cameraDistance;
float times = 1;
Matrix4 matrixView;
Matrix4 matrixModel;
Matrix4 matrixModelView;
Matrix4 matrixProjection;
const float DEG2RAD = 3.141593f / 180;
int  cx = GetSystemMetrics(SM_CXFULLSCREEN);	//这两行获得屏幕大小，用来设置窗口
int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
static int year = 0, day = 0;						//静态全局变量，只在此文件内可用
const float wsSpead = 0.3f;							//前后两个按键的速度
const float adSpead = 0.1f;							//左右两个按键的速度

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);                            //启用阴影平滑
	glClearDepth(1.0);                                  //设置深度缓存//着色模式，一般分为两种，GL_FLAT为单调着色，即图形的颜色为内部第一（或最后一）个顶点的颜色，GL_SMOOTH为渐变着色，顾名思义就不说了
	glEnable(GL_DEPTH_TEST);                            //启用深度测试
	glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正		
}



void drawGrid(float size, float step)
{
	// disable lighting
	glDisable(GL_LIGHTING);

	// 20x20 grid
	glBegin(GL_LINES);

	glColor3f(0.5f, 0.5f, 0.5f);
	for (float i = step; i <= size; i += step)
	{
		glVertex3f(-size, 0, i);   // lines parallel to X-axis
		glVertex3f(size, 0, i);
		glVertex3f(-size, 0, -i);   // lines parallel to X-axis
		glVertex3f(size, 0, -i);

		glVertex3f(i, 0, -size);   // lines parallel to Z-axis
		glVertex3f(i, 0, size);
		glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
		glVertex3f(-i, 0, size);
	}

	// x-axis
	glColor3f(1, 0, 0);
	glVertex3f(-size, 0, 0);
	glVertex3f(size, 0, 0);

	// z-axis
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -size);
	glVertex3f(0, 0, size);

	glEnd();

	// enable lighting back
	glEnable(GL_LIGHTING);
}


void drawCircle(int n)							//画一个圆
{
	GLdouble angle;
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < n; ++i)
	{
		angle = ((2 * i) * PI) / n;
		glVertex2f(0.2*cos(angle), 0.2*sin(angle));
	}
	glEnd();
}


void drawPolygon(int n)							//画一个柱体
{
	GLdouble angle;
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= n; ++i)
	{
		angle = ((2 * i) * PI) / n;
		glVertex2f(0.2*cos(angle), 0.2*sin(angle));
		glVertex3f(0.2*cos(angle), 0.2*sin(angle),0.01f);
	}
	glEnd();
	drawCircle(n);
	//glTranslatef(0.1, 0.1, 0.05);
	drawCircle(n);
}


void drawOneFloor(int n)						//用类似插值的效果画一层
{
	for (int i = 0; i < 5; ++i)
	{
		drawPolygon(n);
		glTranslatef(0.005, 0.005, 0.01);
	}
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//drawGrid(15.0f,2.0f);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//改进第三步，引掉下面两行，使得视点在主人公身上而不是在物体上
	//matrixView.translate(0, 0, cameraDistance);
	matrixView.rotate(cameraAngleX, 1, 0, 0);     //cameraAngleX
	matrixView.rotate(cameraAngleY, 0, 1, 0);     //cameraAngleY
	//matrixView.translate(0, 0, -cameraDistance);
	cameraAngleX = 0;
    cameraAngleY = 0;
    glLoadMatrixf(matrixView.get());
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	

	//模拟多层的神经元
	glPushMatrix();
	glBegin(GL_TRIANGLES);                              //开始绘制金字塔
	glColor3f(1.0f, 0.0f, 0.0f);                    //红色
	glVertex3f(0.0f, 1.0f, 0.0f);                   //上顶点(前侧面)
	glColor3f(0.0f, 1.0f, 0.0f);                    //绿色
	glVertex3f(-1.0f, -1.0f, 1.0f);                 //左下(前侧面)
	glColor3f(0.0f, 0.0f, 1.0f);                    //蓝色
	glVertex3f(1.0f, -1.0f, 1.0f);                  //右下(前侧面)

	glColor3f(1.0f, 0.0f, 0.0f);                    //红色
	glVertex3f(0.0f, 1.0f, 0.0f);                   //上顶点(右侧面)
	glColor3f(0.0f, 0.0f, 1.0f);                    //蓝色
	glVertex3f(1.0f, -1.0f, 1.0f);                  //左下(右侧面)
	glColor3f(0.0f, 1.0f, 0.0f);                    //绿色
	glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(右侧面)

	glColor3f(1.0f, 0.0f, 0.0f);                    //红色
	glVertex3f(0.0f, 1.0f, 0.0f);                   //上顶点(后侧面)
	glColor3f(0.0f, 1.0f, 0.0f);                    //绿色
	glVertex3f(1.0f, -1.0f, -1.0f);                 //左下(后侧面)
	glColor3f(0.0f, 0.0f, 1.0f);                    //蓝色
	glVertex3f(-1.0f, -1.0f, -1.0f);                //右下(后侧面)

	glColor3f(1.0f, 0.0f, 0.0f);                    //红色
	glVertex3f(0.0f, 1.0f, 0.0f);                   //上顶点(左侧面)
	glColor3f(0.0f, 0.0f, 1.0f);                    //蓝色
	glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(左侧面)
	glColor3f(0.0f, 1.0f, 0.0f);                    //绿色
	glVertex3f(-1.0f, -1.0f, 1.0f);                 //右下(左侧面)
	glEnd();                                            //金字塔绘制结束


	glTranslatef(1.5f, 0.0f, -6.0f);                    //右移1.5单位，并移入屏幕6.0单位

	glBegin(GL_QUADS);                                  //开始绘制立方体
	glColor3f(0.0f, 1.0f, 0.0f);                    //绿色
	glVertex3f(1.0f, 1.0f, -1.0f);                  //右上(顶面)
	glVertex3f(-1.0f, 1.0f, -1.0f);                 //左上(顶面)
	glVertex3f(-1.0f, 1.0f, 1.0f);                  //左下(顶面)
	glVertex3f(1.0f, 1.0f, 1.0f);                   //右下(顶面)

	glColor3f(1.0f, 0.5f, 0.0f);                    //橙色
	glVertex3f(1.0f, -1.0f, 1.0f);                  //右上(底面)
	glVertex3f(-1.0f, -1.0f, 1.0f);                 //左上(底面)
	glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(底面)
	glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(底面)

	glColor3f(1.0f, 0.0f, 0.0f);                    //红色
	glVertex3f(1.0f, 1.0f, 1.0f);                   //右上(前面)
	glVertex3f(-1.0f, 1.0f, 1.0f);                  //左上(前面)
	glVertex3f(-1.0f, -1.0f, 1.0f);                 //左下(前面)
	glVertex3f(1.0f, -1.0f, 1.0f);                  //右下(前面)

	glColor3f(1.0f, 1.0f, 0.0f);                    //黄色
	glVertex3f(1.0f, -1.0f, -1.0f);                 //右上(后面)
	glVertex3f(-1.0f, -1.0f, -1.0f);                //左上(后面)
	glVertex3f(-1.0f, 1.0f, -1.0f);                 //左下(后面)
	glVertex3f(1.0f, 1.0f, -1.0f);                  //右下(后面)

	glColor3f(0.0f, 0.0f, 1.0f);                    //蓝色
	glVertex3f(-1.0f, 1.0f, 1.0f);                  //右上(左面)
	glVertex3f(-1.0f, 1.0f, -1.0f);                 //左上(左面)
	glVertex3f(-1.0f, -1.0f, -1.0f);                //左下(左面)
	glVertex3f(-1.0f, -1.0f, 1.0f);                 //右下(左面)

	glColor3f(1.0f, 0.0f, 1.0f);                    //紫色
	glVertex3f(1.0f, 1.0f, -1.0f);                  //右上(右面)
	glVertex3f(1.0f, 1.0f, 1.0f);                   //左上(右面)
	glVertex3f(1.0f, -1.0f, 1.0f);                  //左下(右面)
	glVertex3f(1.0f, -1.0f, -1.0f);                 //右下(右面)
	glEnd();
	//for (int i = 12; i < 62; ++i)
	//{

	//	//drawOneFloor(50);
	//	////drawOneFloor(i);
	//}
	glPopMatrix();


	//glClear(GL_COLOR_BUFFER_BIT);				//用清除颜色清除窗口
	glColor3f(1.0, 1.0, 1.0);					//当前绘制颜色为白色
	glPushMatrix();								//记住当前绘图位置（当前位置是坐标轴原点）

    //glutWireTeapot(0.6f);						//这个代码直接给我生成了一个茶壶
	//glutWireSphere(1.0, 20, 16);				//画一个太阳，参数分别为半径、纬线数、经线数，当前物体是太阳
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);	//当前物体（太阳）绕着（0.0，0.0，0.0）->（0.0，1.0，0.0）向量旋转，此例中即绕着Y轴旋转，但初始旋转角度(GLfloat)year为0，绘制位置未改变
	glTranslatef(2.0, 0.0, 0.0);				//绘图位置向X轴正方向移两个长度，X轴正方向是屏幕的右方，Y轴的正方向是屏幕的上方，此时绘制位置改变
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);		//解释同上
	glutWireSphere(0.2, 10, 8);					//画一个小行星，参数同上，状态机中当前物体为小行星


	glTranslatef(2.0, 0.0, 0.0);				//这几行是我新增的，用来试验旋转
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);		//如果这个加一行，那么day改变，新增球体公转的同时还会自转，只是肉眼难看出有自转
	glutWireSphere(0.2, 10, 8);
	glTranslatef(2.0, 2.0, 0.0);
	glutWireSphere(0.2, 10, 8);
	glTranslatef(2.0, 2.0, 2.0);
	glutWireSphere(0.2, 20, 8);
	glTranslatef(2.0, -2.0, 0.0);
	glutWireSphere(0.2, 10, 8);
	glTranslatef(2.0, 2.0, -2.0);
	glutWireSphere(0.2, 10, 10);



	glPopMatrix();								//回到记住的位置（即坐标原点）


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//和下面reshape函数中一并属于改进第一步，在这里加glLoadMatrixf，窗口大小改动时候不会重置，而后引掉glDepthFunc，否则透视了
	//glDepthFunc(GL_ALWAYS);
	glutSwapBuffers();
	glLoadMatrixf(matrixView.get());
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


Matrix4 setFrustum(float l, float r, float b, float t, float n, float f)
{
	Matrix4 mat;
	mat[0] = 2 * n / (r - l);
	mat[5] = 2 * n / (t - b);
	mat[8] = (r + l) / (r - l);
	mat[9] = (t + b) / (t - b);
	mat[10] = -(f + n) / (f - n);
	mat[11] = -1;
	mat[14] = -(2 * f * n) / (f - n);
	mat[15] = 0;
	return mat;
}


Matrix4 setFrustum(float fovY, float aspectRatio, float front, float back)
{
	float tangent = tanf(fovY / 2 * DEG2RAD);   // tangent of half fovY
	float height = front * tangent;           // half height of near plane
	float width = height * aspectRatio;       // half width of near plane

											  // params: left, right, bottom, top, near, far
	return setFrustum(-width, width, -height, height, front, back);
}


void reshape(int w, int h)
{
	//这是书上的版本
	//glViewport(0, 0, (GLsizei)w, (GLsizei)h);					//指定视口，前两个参数是视口左下！角的坐标，后面两个指视口的宽度和高度，写在reshape()内，随着窗口大小改变，视口才能对应的改变，物体大小相对改变
	//															//若不写此行，视口默认是左下角（0，0）和最初设置的窗口大小的宽高，这样的话窗口大小改变，里面的物体不会变，可以引掉调试看看

	////修改视景体的步骤
	//glMatrixMode(GL_PROJECTION);								//指定当前修改的矩阵类型为投影矩阵，GL_MODELVIEW表示视点矩阵，GL_PROJECTION表示投影矩阵，GL_TEXTURE表示纹理矩阵
	//glLoadIdentity();											//将当前矩阵清空，即设置为4x4的单位矩阵
	//gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);	//设置视景体，此函数为透视投影，参数依次为Y方向的视野角，宽高比，近裁剪面和远裁剪面，详细见书3.3.1

	////修改视点的步骤
	//glMatrixMode(GL_MODELVIEW);									//同上
	//glLoadIdentity();
	//gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);		//设置视点，前三个参数是视点的坐标，中间三个是朝向，后面三个是向上方向（可以理解为观测物体的向上方向，只有第二个参数朝向的话物体倒着也可以）





	//这个版本改变窗口大小出bug
	//glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	//glMatrixMode(GL_PROJECTION);
	//gluPerspective(80.0f, (float)(w) / h, 0.5f, 10.0f);				// FOV, AspectRatio, NearClip, FarClip

	//// switch to modelview matrix in order to set scene
	//glMatrixMode(GL_MODELVIEW);
	//matrixView.identity();
	//matrixView.translate(0, 0, -cameraDistance);
	//glLoadMatrixf(matrixView.get());








	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//属于改进的第二步，这样的话，左右移动后，物体绕点旋转的点也随之移动，之前旋转点都不会动
	//matrixView.identity();
	//matrixProjection = setFrustum(45.0f, (float)w / h, 1.0f, 100.0f);
	//glLoadMatrixf(matrixProjection.get());
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//属于改进的第一步，glLoadMatrixf如果加在这里，窗口大小改变会重置情景
	//matrixView.identity();
	//matrixView.translate(0, 0, -cameraDistance);
	//glLoadMatrixf(matrixView.get());
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void keyboard(unsigned char key, int x, int y)	//第一个参数是键码，后面两个参数是鼠标的位置
{
	switch (key) {
	case 't':
		day = (day + 10) % 360;			//day的值修改，display(void)中走到glRotatef((GLfloat)day, 0.0, 1.0, 0.0)时，当前位置是(2，0，0)，当前物体是小行星（这个不懂！？）
										//当前物体绕着‘当前位置->(0.0, 1.0, 0.0)’这个轴旋转，此时是自转！没毛病
		glutPostRedisplay();			//窗口内容修改了，有它才能更新
		break;
	case 'T':							//大写的字母容易被新手忽略
		day = (day - 10) % 360;
		glutPostRedisplay();
		break;
	case 'y':
		year = (year + 5) % 360;		//year的值修改，display(void)中走到glRotatef((GLfloat)year, 0.0, 1.0, 0.0)时，当前位置是(0，0，0)，当前物体是小行星（这个还是不懂！？）
										//当前物体绕着‘当前位置->(0.0, 1.0, 0.0)’这个轴旋转，此时是公转！没毛病
		glutPostRedisplay();
		break;
	case 'Y':
		year = (year - 5) % 360;
		glutPostRedisplay();
		break;
	case 'w':
		matrixView.translate(0, 0, cameraDistance);
		cameraDistance -= wsSpead;
		matrixView.translate(0, 0, -cameraDistance);
		break;
	case 's':
		matrixView.translate(0, 0, cameraDistance);
		cameraDistance += wsSpead;
		matrixView.translate(0, 0, -cameraDistance);
		break;
	case 'a':
		matrixView.translate(cameraDistance, 0, 0);
		cameraDistance -= adSpead;
		matrixView.translate(-cameraDistance, 0, 0);
		break;
	case 'd':
		matrixView.translate(cameraDistance, 0, 0);
		cameraDistance += adSpead;
		matrixView.translate(-cameraDistance, 0, 0);
		break;
	case 27:              //按Esc退出
	    exit(0);
	    break;
	default:
		break;
	}
}


void mousePress(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)			//左键
	{
		if (state == GLUT_DOWN)				//左键按下时
		{
			mouseLeftDown = true;			//改变状态值
		}
		else if (state == GLUT_UP)			//左键松开时
			mouseLeftDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
}


void mouseMotion(int x, int y)
{
	if (mouseLeftDown)								//鼠标左键的作用是绕某点选择
	{
		cameraAngleY = (x - mouseX);
		cameraAngleX = (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)								//鼠标右键的作用是视点移动，按住右键后上下移动才有效，因为用了y - mouseY，按键也有这样的效果
	{
		//matrixView.translate(0, 0, cameraDistance);
		//cameraDistance += (y - mouseY) * 0.01f;
		//mouseY = y;
		//matrixView.translate(0, 0, -cameraDistance);
	}
}


//如果没有这个函数就非常卡，函数作用是只对每个给定的毫秒重绘
void timer(int millisec)
{
	glutTimerFunc(millisec, timer, millisec);
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	//初始化全局变量
	mouseLeftDown = mouseRightDown = false;
	mouseX = mouseY = 0;
	cameraAngleX = cameraAngleY = 0;
	cameraDistance = CAMERA_DISTANCE;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	//双缓存，调用glutSwapBuffers()去交换缓存，因为所有的绘制都在后台缓存，不交换就看不到东西
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	//单缓存，调用glFlush()去强制执行，因为缓存满了才会执行命令，而没有执行前面的绘制就也看不到东西
	glutInitWindowSize(cx, cy);					//新建立的窗口的大小
	glutInitWindowPosition(0, 0);				//窗口离屏幕左上!角的位置
	int handle = glutCreateWindow(argv[0]);
	init();											//窗口内容初始化
	glutDisplayFunc(display);						//用于显示图形的回调函数

	glutTimerFunc(33, timer, 33);

	glutReshapeFunc(reshape);						//窗口大小发生改变时调用的函数
	glutKeyboardFunc(keyboard);						//响应特定按键的函数
	glutMouseFunc(mousePress);
	glutMotionFunc(mouseMotion);
	glutMainLoop();									//主函数循环，这样窗口才不会直接就退出
	return handle;
}