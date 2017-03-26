// main.cpp : 定义控制台应用程序的入口点。
//

#include "gl/glut.h"

float fTranslate;
float fRotate;
float fScale;

typedef GLfloat vertex3[3];
//vertex3 pt[40] = { {0,0,0},{0,1,0} ,{1,0,0} ,{1,1,0} ,{0,0,1} ,{0,1,1} ,{1,0,1} ,{1,1,1} };
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

void Draw_Table() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vt);//死活没显示长方形居然是因为没改“pt”
	glColor3f(1.0, 0.0, 0.0);
	//GLubyte vertIndex[] = { 6, 2, 3, 7, 5, 1, 0, 4, 7, 3, 1, 5, 4, 0, 2, 6, 2, 0, 1, 3, 7, 5, 4, 6 };
	GLubyte vertexIndex[] = {
		1,3,2,0, 37,39,3,1, 36,38,39,37, 0,2,38,36, 38,2,3,39, 36,37,1,0,	//桌面
		5,7,6,4, 13,15,7,5, 12,14,15,13, 4,6,14,12, 4,12,13,5,				//左上
		9,11,10,8, 17,19,11,9, 16,18,19,17, 8,10,18,16, 8,16,17,9,			//右上
		21,23,22,20, 29,31,23,21, 28,30,31,29, 20,22,30,28, 20,28,29,21,	//左下
		25,27,26,24, 33,35,27,25, 32,34,35,33, 24,26,34,32, 24,32,33,25		//右下
	};
	glDrawElements(GL_QUADS, 104, GL_UNSIGNED_BYTE, vertexIndex);
}

void Draw_Triangle() // This function draws a triangle with RGB colors
{
	glBegin(GL_TRIANGLES);
		 glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		 glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);
		 glColor3f(0.0f, 0.0f,1.0f);
		glVertex3f( 1.0f,-1.0f, 0.0f);			
	glEnd();
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
	
	// Translate
	glPushMatrix();
		glTranslatef(-3.0f, 0.0f,-6.0f);		// Place the table Left
		glTranslatef(0.0f, fTranslate, 0.0f);	// Translate in Y direction
		glScalef(0.4f, 0.4f, 0.4f);				// Scale the Table
		Draw_Table();							// Draw table					
	glPopMatrix();

	// Rotate
    glPushMatrix();
		glTranslatef(0.0f, 0.0f,-6.0f);			// Place the table at Center
		//glRotatef(fRotate, 1.0f, 1.0f, 0);	// Rotate around Y axis
		glRotatef(fRotate, 30 * fRotate, 20 * fRotate, 40 * fRotate);
		glScalef(0.4f, 0.4f, 0.4f);				// Scale the Table
		Draw_Table();							// Draw table
	glPopMatrix();

	// Scale
	glPushMatrix();
		glTranslatef(3.0f, 0.0f, -6.0f);		// Place the table at Center
		glScalef(0.4 * (1 - fScale), 0.4 * (1 - fScale), 0.4 * (1 - fScale));	// Scale the Table
		Draw_Table();							// Draw table
	glPopMatrix();

	fTranslate += 0.005f;
	fRotate    += 0.5f;
	fScale     += 0.005f;

	if (fTranslate > 2.5f)
		fTranslate = 0.0f;
	if (fScale > 0.5f)
		fScale = 0.0f;

	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640,480);                                                  
	glutCreateWindow("Exercise2");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);		
	glutIdleFunc(idle);					

	glutMainLoop();

	return 0;
}


