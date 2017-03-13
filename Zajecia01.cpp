#include "stdafx.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//Patryk Kiszczak 1b

void drawShape()
{
	glColor3f(0.0f, 10.0f, 0.0f);
	glLineWidth(2.0); 

	glBegin(GL_POLYGON);
	glVertex2f(0, -50);
	glVertex2f(-32, -38);
	glVertex2f(-49, -9);
	glVertex2f(-43, 25);
	glVertex2f(-17, 47);
	glVertex2f(17, 47);
	glVertex2f(43, 25);
	glVertex2f(49, -9);
	glVertex2f(32, -38);

	glEnd();
}

void RenderScene(void) {   
	glClear(GL_COLOR_BUFFER_BIT);    
	drawShape(); 
	glFlush();
}

void TimerFunction(int value) {
	glRotated(10, 0, 0, 1);
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunction, 1);
}

void SetupRC(void) {   
	glClearColor(0.647059f, 0.164706f, 0.0f, 1.0f);
}

void ChangeSize(int w, int h) {
	GLfloat aspectRatio;

	if (h == 0)   h = 1;
   
	glViewport(0, 0, w, h);
 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)    glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
	else    glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Mój pierwszy program w GLUT");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(100, TimerFunction, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}
