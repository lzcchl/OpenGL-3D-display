//���ù�����Ϊ֧��ͷ�ļ�x64��<gl/glut.h>����Ҫ��openGL��������freeglutѹ������ͷ�ļ���ָ��λ��C:\Program Files (x86)\Windows Kits\8.1\Include\um\gl
//			lib�ļ��и��Ƶ���ǰ����Ŀ¼��Ȼ����\������\���������м��ϡ�.\lib����\������\������ϡ�freeglut.lib���������x64\Debug\���ơ�freeglut.dll��

#include <gl/glut.h>
#include <Windows.h>
#include "Matrices.h"
#include "Vectors.h"

bool mouseLeftDown, mouseRightDown, mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX, cameraAngleY;
const float CAMERA_DISTANCE = 5.0f;
float cameraDistance;
float times = 1;
Matrix4 matrixView;
const int  cx = GetSystemMetrics(SM_CXFULLSCREEN);	//�����л����Ļ��С���������ô���
const int  cy = GetSystemMetrics(SM_CYFULLSCREEN);
static int year = 0, day = 0;						//��̬ȫ�ֱ�����ֻ�ڴ��ļ��ڿ���

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);			//�����ɫΪ��ɫ
	glShadeModel(GL_FLAT);						//��ɫģʽ��һ���Ϊ���֣�GL_FLATΪ������ɫ����ͼ�ε���ɫΪ�ڲ���һ�������һ�����������ɫ��GL_SMOOTHΪ������ɫ������˼��Ͳ�˵��
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	matrixView.translate(0, 0, cameraDistance);
	matrixView.rotate(cameraAngleX, 1, 0, 0);     //cameraAngleX
	matrixView.rotate(cameraAngleY, 0, 1, 0);     //cameraAngleY
	matrixView.translate(0, 0, -cameraDistance);
	cameraAngleX = 0;
    cameraAngleY = 0;
    glLoadMatrixf(matrixView.get());

	glClear(GL_COLOR_BUFFER_BIT);				//�������ɫ�������
	glColor3f(1.0, 1.0, 1.0);					//��ǰ������ɫΪ��ɫ
	glPushMatrix();								//��ס��ǰ��ͼλ�ã���ǰλ����������ԭ�㣩
	glutWireSphere(1.0, 20, 16);				//��һ��̫���������ֱ�Ϊ�뾶��γ����������������ǰ������̫��
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);	//��ǰ���壨̫�������ţ�0.0��0.0��0.0��->��0.0��1.0��0.0��������ת�������м�����Y����ת������ʼ��ת�Ƕ�(GLfloat)yearΪ0������λ��δ�ı�
	glTranslatef(2.0, 0.0, 0.0);				//��ͼλ����X�����������������ȣ�X������������Ļ���ҷ���Y�������������Ļ���Ϸ�����ʱ����λ�øı�
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);		//����ͬ��
	glutWireSphere(0.2, 10, 8);					//��һ��С���ǣ�����ͬ�ϣ�״̬���е�ǰ����ΪС����


	glTranslatef(2.0, 0.0, 0.0);				//�⼸�����������ģ�����������ת
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);		//��������һ�У���ôday�ı䣬�������幫ת��ͬʱ������ת��ֻ�������ѿ�������ת
	glutWireSphere(0.2, 10, 8);


	glPopMatrix();								//�ص���ס��λ�ã�������ԭ�㣩

	//glutWireTeapot(0.6f);						//�������ֱ�Ӹ���������һ�����
	glDepthFunc(GL_ALWAYS);
	glutSwapBuffers();
}


void reshape(int w, int h)
{
	//glViewport(0, 0, (GLsizei)w, (GLsizei)h);					//ָ���ӿڣ�ǰ�����������ӿ����£��ǵ����꣬��������ָ�ӿڵĿ�Ⱥ͸߶ȣ�д��reshape()�ڣ����Ŵ��ڴ�С�ı䣬�ӿڲ��ܶ�Ӧ�ĸı䣬�����С��Ըı�
	//															//����д���У��ӿ�Ĭ�������½ǣ�0��0����������õĴ��ڴ�С�Ŀ�ߣ������Ļ����ڴ�С�ı䣬��������岻��䣬�����������Կ���

	////�޸��Ӿ���Ĳ���
	//glMatrixMode(GL_PROJECTION);								//ָ����ǰ�޸ĵľ�������ΪͶӰ����GL_MODELVIEW��ʾ�ӵ����GL_PROJECTION��ʾͶӰ����GL_TEXTURE��ʾ�������
	//glLoadIdentity();											//����ǰ������գ�������Ϊ4x4�ĵ�λ����
	//gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);	//�����Ӿ��壬�˺���Ϊ͸��ͶӰ����������ΪY�������Ұ�ǣ���߱ȣ����ü����Զ�ü��棬��ϸ����3.3.1

	////�޸��ӵ�Ĳ���
	//glMatrixMode(GL_MODELVIEW);									//ͬ��
	//glLoadIdentity();
	//gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);		//�����ӵ㣬ǰ�����������ӵ�����꣬�м������ǳ��򣬺������������Ϸ��򣨿������Ϊ�۲���������Ϸ���ֻ�еڶ�����������Ļ����嵹��Ҳ���ԣ�

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(80.0f, (float)(w) / h, 0.5f, 10.0f);				// FOV, AspectRatio, NearClip, FarClip

	// switch to modelview matrix in order to set scene
	glMatrixMode(GL_MODELVIEW);
	matrixView.identity();
	matrixView.translate(0, 0, -cameraDistance);
	glLoadMatrixf(matrixView.get());
}


void keyboard(unsigned char key, int x, int y)	//��һ�������Ǽ��룬������������������λ��
{
	switch (key) {
	case 'd':
		day = (day + 10) % 360;			//day��ֵ�޸ģ�display(void)���ߵ�glRotatef((GLfloat)day, 0.0, 1.0, 0.0)ʱ����ǰλ����(2��0��0)����ǰ������С���ǣ��������������
										//��ǰ�������š���ǰλ��->(0.0, 1.0, 0.0)���������ת����ʱ����ת��ûë��
		glutPostRedisplay();			//���������޸��ˣ��������ܸ���
		break;
	case 'D':							//��д����ĸ���ױ����ֺ���
		day = (day - 10) % 360;
		glutPostRedisplay();
		break;
	case 'y':
		year = (year + 5) % 360;		//year��ֵ�޸ģ�display(void)���ߵ�glRotatef((GLfloat)year, 0.0, 1.0, 0.0)ʱ����ǰλ����(0��0��0)����ǰ������С���ǣ�������ǲ���������
										//��ǰ�������š���ǰλ��->(0.0, 1.0, 0.0)���������ת����ʱ�ǹ�ת��ûë��
		glutPostRedisplay();
		break;
	case 'Y':
		year = (year - 5) % 360;
		glutPostRedisplay();
		break;
	case '=':
		matrixView.translate(0, 0, cameraDistance);
		cameraDistance *= 0.9f;
		matrixView.translate(0, 0, -cameraDistance);
		break;
	case '-':
		matrixView.translate(0, 0, cameraDistance);
		cameraDistance *= 1.1f;
		matrixView.translate(0, 0, -cameraDistance);
		break;
	case 27:              //��Esc�˳�
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

	if (button == GLUT_LEFT_BUTTON)			//���
	{
		if (state == GLUT_DOWN)				//�������ʱ
		{
			mouseLeftDown = true;			//�ı�״ֵ̬
		}
		else if (state == GLUT_UP)			//����ɿ�ʱ
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
	if (mouseLeftDown)								//����������������ĳ��ѡ��
	{
		cameraAngleY = (x - mouseX);
		cameraAngleX = (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)								//����Ҽ����������ӵ��ƶ������ú͡�=����-��һ��
	{
		matrixView.translate(0, 0, cameraDistance);
		cameraDistance += (y - mouseY) * 0.01f;
		mouseY = y;
		matrixView.translate(0, 0, -cameraDistance);
	}
}


//���û����������ͷǳ���������������ֻ��ÿ�������ĺ����ػ�
void timer(int millisec)
{
	glutTimerFunc(millisec, timer, millisec);
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	//��ʼ��ȫ�ֱ���
	mouseLeftDown = mouseRightDown = false;
	mouseX = mouseY = 0;
	cameraAngleX = cameraAngleY = 0;
	cameraDistance = CAMERA_DISTANCE;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);	//˫���棬����glutSwapBuffers()ȥ�������棬��Ϊ���еĻ��ƶ��ں�̨���棬�������Ϳ���������
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	//�����棬����glFlush()ȥǿ��ִ�У���Ϊ�������˲Ż�ִ�������û��ִ��ǰ��Ļ��ƾ�Ҳ����������
	glutInitWindowSize(cx, cy);					//�½����Ĵ��ڵĴ�С
	glutInitWindowPosition(0, 0);				//��������Ļ����!�ǵ�λ��
	glutCreateWindow(argv[0]);
	init();											//�������ݳ�ʼ��
	glutDisplayFunc(display);						//������ʾͼ�εĻص�����

	glutTimerFunc(33, timer, 33);

	glutReshapeFunc(reshape);						//���ڴ�С�����ı�ʱ���õĺ���
	glutKeyboardFunc(keyboard);						//��Ӧ�ض������ĺ���
	glutMouseFunc(mousePress);
	glutMotionFunc(mouseMotion);
	glutMainLoop();									//������ѭ�����������ڲŲ���ֱ�Ӿ��˳�
	return 0;
}