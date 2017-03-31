// main.cpp : 定义控制台应用程序的入口点。
//

#include "gl/glut.h"
#include "iostream"

using namespace std;

#define Xmax 0
#define Ymax 1
#define Xmin 2
#define Ymin 3

int run = 1;

float fLineWidth = 2.0f;

float fTranslateX;
float fTranslateY;
float fRotate;
float fScale = 0.5f;
float fRed = 1.0f;
float fGreen = 1.0f;
float fBlue = 1.0f;

float TvX = 0.02f;
float TvY = 0.02f;
float Sv = 0.0005f;
float CvR = 0.006f;
float CvG = 0.008f;
float CvB = 0.002f;
float Border[4] = {3.0f, 2.5f, -3.0f, -2.5f};//Xmax, Ymax, Xmin, Ymin

int ScaleMark = 1;
int ColorMark = 1;

enum {
	NOTHING,
	RED,
	GREEN,
	BLUE,
	DEFAULT,
	EXIT
};

typedef GLfloat vertex3[3];
vertex3 vt[40] = { 
	//0-3
	{ -2.5,2,-1 },{ 2.5,2,-1 },{ -2.5,2,-2 },{ 2.5,2,-2 },
	//4-7
	{ -1.84,1.33,2 },{ -1.18,1.33,2 },{ -1.84,1.33,-1 },{ -1.18,1.33,-1 },
	//8-11
	{ 1.18,1.33,2 },{ 1.84,1.33,2 },{ 1.18,1.33,-1 },{ 1.84,1.33,-1 },
	//12-15
	{ -1.84,0.66,2 },{ -1.18,0.66,2 },{ -1.84,0.66,-1 },{ -1.18,0.66,-1 },
	//16-19
	{ 1.18,0.66,2 },{ 1.84,0.66,2 },{ 1.18,0.66,-1 },{ 1.84,0.66,-1 },
	//20-23
	{ -1.84,-0.66,2 },{ -1.18,-0.66,2 },{ -1.84,-0.66,-1 },{ -1.18,-0.66,-1 },
	//24-27
	{ 1.18,-0.66,2 },{ 1.84,-0.66,2 },{ 1.18,-0.66,-1 },{ 1.84,-0.66,-1 },
	//28-31
	{ -1.84,-1.33,2 },{ -1.18,-1.33,2 },{ -1.84,-1.33,-1 },{ -1.18,-1.33,-1 },
	//22-35
	{ 1.18,-1.33,2 },{ 1.84,-1.33,2 },{ 1.18,-1.33,-1 },{ 1.84,-1.33,-1 },
	//36-39
	{ -2.5,-2,-1 },{ 2.5,-2,-1 },{ -2.5,-2,-2 },{ 2.5,-2,-2 } 
};

void calTranslate(void) {
	if (fTranslateY == 0)
		if (fTranslateX <= 0)
			if (fTranslateX > Border[Xmin])
				fTranslateX -= TvX;
			else
				fTranslateY += TvY;
		else
			fTranslateY -= TvY;
	else if (fTranslateY <= Border[Ymax] && fTranslateY >= Border[Ymin])
		if (fTranslateX <= 0)
			fTranslateY += TvY;
		else
			fTranslateY -= TvY;
	else if (fTranslateY > Border[Ymax])
		if (fTranslateX >= Border[Xmax])
			fTranslateY -= TvY;
		else
			fTranslateX += TvX;
	else
		if (fTranslateX <= Border[Xmin])
			fTranslateY += TvY;
		else
			fTranslateX -= TvX;
}

void CalScale(void) {
	if (fScale <= 0.5f && fScale >= 0.3f)
		if (ScaleMark == 1)
			fScale -= Sv;
		else
			fScale += Sv;
	else if (fScale > 0.5f) {
		fScale -= Sv;
		ScaleMark = 1;
	}
	else if (fScale < 0.3f) {
		fScale += Sv;
		ScaleMark = 0;
	}
}

void CalColor(float* Color, float* v) {
	if (*Color < 1.0f && *Color > 0.1f)
		if (ColorMark == 1)
			*Color -= *v;
		else
			*Color += *v;
	else if (*Color >= 1.0f) {
		*Color -= *v;
		ColorMark = 1;
		*v *= 1.2;
	}
	else if (*Color <= 0.1f) {
		*Color += *v;
		ColorMark = 0;
		*v *= 0.8;
	}
}

void Draw_Table() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vt);//死活没显示长方形居然是因为没改“pt”
	glColor3f(fRed, fGreen, fBlue);
	GLubyte vertexIndex[] = {
		1,3,2,0, 37,39,3,1, 36,38,39,37, 0,2,38,36, 38,2,3,39, 36,37,1,0,	//桌面
		5,7,6,4, 13,15,7,5, 12,14,15,13, 4,6,14,12, 4,12,13,5,				//左上
		9,11,10,8, 17,19,11,9, 16,18,19,17, 8,10,18,16, 8,16,17,9,			//右上
		21,23,22,20, 29,31,23,21, 28,30,31,29, 20,22,30,28, 20,28,29,21,	//左下
		25,27,26,24, 33,35,27,25, 32,34,35,33, 24,26,34,32, 24,32,33,25		//右下
	};
	glDrawElements(GL_QUADS, 104, GL_UNSIGNED_BYTE, vertexIndex);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
	case 'q':
		cout << "q pressed. Pause." << endl;
		run = 0;
		break;
	case 'c':
		cout << "c pressed. Continue." << endl;
		run = 1;
		break;
	}
}

void menu(int value) {
	switch (value) {
	case RED:
		glClearColor(0.8, 0.0, 0.0, 0.0);
		break;
	case GREEN:
		glClearColor(0.0, 0.8, 0.0, 0.0);
		break;
	case BLUE:
		glClearColor(0.0, 0.0, 0.8, 0.0);
		break;
	case DEFAULT:
		glClearColor(0.2, 0.2, 0.2, 0.0);
		break;
	case EXIT:
		exit(0);
	case NOTHING:
		glutDestroyMenu(glutGetMenu());
		break;
	}
}

void timer(int value) {
	if (run) {
		calTranslate();

		fRotate += 0.5f;

		CalScale();

		CalColor(&fRed, &CvR);
		CalColor(&fGreen, &CvG);
		CalColor(&fBlue, &CvB);
	}

	glutPostRedisplay();

	glutTimerFunc(16, timer, 1);
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);//调整第一个参数可以改变视口大小

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

void redraw()
{
	// Display in wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();							// Reset The Current Modelview Matrix

	// 3 in 1
	glPushMatrix();
		glTranslatef(fTranslateX, fTranslateY, -6.0f);		// Place the table at Center
		glRotatef(fRotate, 30 * fRotate, 20 * fRotate, 40 * fRotate);
		glScalef(fScale, fScale, fScale);				// Scale the Table
		Draw_Table();							// Draw table
	glPopMatrix();

	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640,480);                                                  
	glutCreateWindow("Exercise2");

	// Set the background color - dark grey
	glClearColor(0.2, 0.2, 0.2, 0.0);
	glLineWidth(fLineWidth);

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);		
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutCreateMenu(menu);
	glutAddMenuEntry("Please select as quick as possible!", NOTHING);
	glutAddMenuEntry("-----------------------------------", NOTHING);
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Default", DEFAULT);
	glutAddMenuEntry("-----------------------------------", NOTHING);
	glutAddMenuEntry("Exit", EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutTimerFunc(16, timer, 1);

	glutMainLoop();

	return 0;
}


