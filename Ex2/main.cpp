// main.cpp : 定义控制台应用程序的入口点。
//

#include "gl/glut.h"
#include "iostream"
#include "bitmap.h"
#include "stdlib.h"

using namespace std;

#pragma warning(disable: 4996)

bool run = 1;
bool lm = 1;					// Polygon line mode
bool save = 0;

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

int frame = 1;

int delay = 8;

enum {
	Xmax,
	Ymax,
	Xmin,
	Ymin
};

enum {
	NOTHING,
	RED,
	GREEN,
	BLUE,
	DEFAULT,
	HIDE,
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

void DrawCube(GLfloat x, GLfloat y, GLfloat z, GLfloat xlength, GLfloat ylength, GLfloat zlength) {
	glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(xlength, ylength, zlength);
		glutSolidCube(1);
	glPopMatrix();
}

void Draw_Table_by_vertex_array() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vt);//死活没显示长方形居然是因为没改“pt”
	GLubyte vertexIndex[] = {
		1,3,2,0, 37,39,3,1, 36,38,39,37, 0,2,38,36, 38,2,3,39, 36,37,1,0,	//桌面
		5,7,6,4, 13,15,7,5, 12,14,15,13, 4,6,14,12, 4,12,13,5,				//左上
		9,11,10,8, 17,19,11,9, 16,18,19,17, 8,10,18,16, 8,16,17,9,			//右上
		21,23,22,20, 29,31,23,21, 28,30,31,29, 20,22,30,28, 20,28,29,21,	//左下
		25,27,26,24, 33,35,27,25, 32,34,35,33, 24,26,34,32, 24,32,33,25		//右下
	};
	glDrawElements(GL_QUADS, 104, GL_UNSIGNED_BYTE, vertexIndex);
}

void Draw_Table() {
	DrawCube(0.0, 0.0, -1.5, 5.0, 4.0, 1.0);
	DrawCube(1.5, 1.0, 0.5, 0.67, 0.67, 3.0);
	DrawCube(1.5, -1.0, 0.5, 0.67, 0.67, 3.0);
	DrawCube(-1.5, 1.0, 0.5, 0.67, 0.67, 3.0);
	DrawCube(-1.5, -1.0, 0.5, 0.67, 0.67, 3.0);
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
	case 'x':
		cout << "x pressed. Switch line mode: " << lm << "." << endl;
		lm = !lm;
		break;
	case 's':
		screenshot();
		cout << "s pressed. Screenshot Saved." << endl;
		break;
	default:
		cout << "Some key is pressed, but nothing happened." << endl;
		break;
	}
}

void exportBmp(void) {
	GLint viewPort[4] = { 0 };
	glGetIntegerv(GL_VIEWPORT, viewPort);
	GLbyte * buffer = (GLbyte *)malloc(viewPort[2] * viewPort[3] * sizeof(GLbyte) * 3);
	glReadPixels(viewPort[0], viewPort[1], viewPort[2], viewPort[3],
		GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
	long fileSize = viewPort[2] * viewPort[3] * 3 + 54;
	//int i=0;  

	fileHeader.bfType[0] = 0x42;
	fileHeader.bfType[1] = 0x4d;
	LongToByte(fileSize, fileHeader.bfSize);
	LongToByte(54, fileHeader.bfOffBits);

	LongToByte(sizeof(infoHeader), infoHeader.biSize);
	LongToByte(viewPort[2], infoHeader.biWidth);
	LongToByte(viewPort[3], infoHeader.biHeight);

	infoHeader.biPlanes[0] = 0x01;
	infoHeader.biPlanes[1] = 0x00;
	infoHeader.biBitCount[0] = 0x18;
	infoHeader.biBitCount[1] = 0x00;
	LongToByte(0, infoHeader.biCompression);

	LongToByte((viewPort[2] * viewPort[3]), infoHeader.biSizeImage);

	char filename[30];
	sprintf(filename, "./screenshots/%d.bmp", frame);
	FILE * fp = fopen(filename, "w+");
	//cout << "1" << endl;
	fwrite(&fileHeader, sizeof(fileHeader), 1, fp);
	//cout << "2" << endl;
	fwrite(&infoHeader, sizeof(infoHeader), 1, fp);
	//cout << "3" << endl;
	fwrite(buffer, 1, (viewPort[2] * viewPort[3] * 3), fp);
	//cout << "4" << endl;
	fclose(fp);
	//cout << "5" << endl;

	free(buffer);
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
	case HIDE:
		glutDestroyMenu(glutGetMenu());
		break;
	}
}

void transform(void) {
	if (run) {
		calTranslate();

		fRotate += 0.5f;

		CalScale();

		CalColor(&fRed, &CvR);
		CalColor(&fGreen, &CvG);
		CalColor(&fBlue, &CvB);
	}
}

void timer(int value) {
	transform();

	glutPostRedisplay();

	glutTimerFunc(delay, timer, 1);
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

void redraw()
{
	// Display in wireframe mode
	if (lm)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();							// Reset The Current Modelview Matrix

	// 3 in 1
	glPushMatrix();
		glTranslatef(fTranslateX, fTranslateY, -6.0f);		// Place the table at Center
		glRotatef(fRotate, 30 * fRotate, 20 * fRotate, 40 * fRotate);
		glScalef(fScale, fScale, fScale);				// Scale the Table
		glColor3f(fRed, fGreen, fBlue);
		Draw_Table();							// Draw table
	glPopMatrix();

	glutSwapBuffers();

	//cout << frame << endl;
	/*if ( frame <= 10000 && save) {
		exportBmp();
		frame++;
	}*/
}

void initMenu(void) {
	glutCreateMenu(menu);

	glutAddMenuEntry("Please select as quick as possible!", NOTHING);
	glutAddMenuEntry("---------Change color here---------", NOTHING);
	glutAddMenuEntry("    Red", RED);
	glutAddMenuEntry("    Green", GREEN);
	glutAddMenuEntry("    Blue", BLUE);
	glutAddMenuEntry("    Default", DEFAULT);
	glutAddMenuEntry("------------------------------------------", NOTHING);
	glutAddMenuEntry("Hide menu. --You cannot cancel it!", HIDE);
	glutAddMenuEntry("------------------------------------------", NOTHING);
	glutAddMenuEntry("Exit", EXIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640,480);                                                  
	glutCreateWindow("Exercise2");

	// Initiate the menu
	initMenu();
	// Set the background color - dark grey
	glClearColor(0.2, 0.2, 0.2, 0.0);
	// Set the line width
	glLineWidth(fLineWidth);
	// Set the timer
	glutTimerFunc(delay, timer, 1);

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);		
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}