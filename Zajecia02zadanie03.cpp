

#include "stdafx.h"
//#include "math.h"

#include "stdafx.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "gltools_extracted.h"
#include "VectorMath.cpp"



static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void ChangeSize(int w, int h)
{
	GLfloat fAspect;
	
	if (h == 0)
		h = 1;
	
	glViewport(0, 0, w, h);
	fAspect = (GLfloat)w / (GLfloat)h;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(35.0f, fAspect, 1.0, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



void SetupRC()
{
	GLuint texture;
	int x, y;
	char pixels[256 * 256];
	GLint iWidth = 256;
	GLint iHeight = 256;
	GLint iComponents = GL_RGB;
	GLenum eFormat = {
		GL_RGBA
	};

	GLfloat whiteLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat lightPos[] = { -10.f, 5.0f, 5.0f, 1.0f };
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); 
	glFrontFace(GL_CCW); 
						 
	glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);


	for (y = 0; y < 256; y++)
	{
		for (x = 0; x < 256; x++)
			pixels[y * 256 + x] = rand() % 256;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 256, 256, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}


void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;
	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;
	xRot = (GLfloat)((const int)xRot % 360);
	yRot = (GLfloat)((const int)yRot % 360);

	glutPostRedisplay();
}

void RenderScene(void)
{
	GLTVector3 vNormal;
	GLTVector3 zero = { 0,0,0 };
	GLTVector3 vCorners[10] = { { 0.0f, .80f, 0.0f }, // Góra 0
	{ 0.0f, 0.0f, -1.0f }, // Lewy ty³ 1
	{ -0.64f, 0.0f, -0.77f }, // Prawy ty³ 2
	{ -0.98f, 0.0f, -0.17f }, // Prawy przód 3
	{ -0.87f, 0.0f, 0.5f },//4
	{ -0.34f, 0.0f, 0.94f },//5
	{ 0.34f, 0.0f, 0.94f },//6
	{ 0.87f, 0.0f, 0.50f },//7
	{ 0.98f, 0.0f, -0.17f },//8
	{ 0.64f, 0.0f, -0.77f },//9
	}; 
	   
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	
	glTranslatef(0.0f, -0.25f, -4.0f);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	// Podstawa piramidy 


	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(zero);
	glTexCoord2f(0.0f, -1.0f);
	glVertex3fv(vCorners[2]);
	glTexCoord2f(-0.6f, -0.8f);
	glVertex3fv(vCorners[1]);


	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(zero);
	glTexCoord2f(-0.6f, -0.8f);
	glVertex3fv(vCorners[3]);
	glTexCoord2f(-1.0f, -0.2f);
	glVertex3fv(vCorners[2]);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(zero);
	glTexCoord2f(-1.0f, -0.2f);
	glVertex3fv(vCorners[4]);
	glTexCoord2f(-0.9f, 0.5f);
	glVertex3fv(vCorners[3]);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(zero);
	glTexCoord2f(-0.9f, 0.5f);
	glVertex3fv(vCorners[5]);
	glTexCoord2f(-0.3f, 0.9f);
	glVertex3fv(vCorners[4]);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(zero);
	glTexCoord2f(-0.3f, 0.9f);
	glVertex3fv(vCorners[6]);
	glTexCoord2f(0.3f, 0.9f);
	glVertex3fv(vCorners[5]);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(zero);
	glTexCoord2f(0.3f, 0.9f);
	glVertex3fv(vCorners[7]);
	glTexCoord2f(0.9f, 0.5f);
	glVertex3fv(vCorners[6]);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(zero);
	glTexCoord2f(0.9f, 0.5f);
	glVertex3fv(vCorners[8]);
	glTexCoord2f(1.0f, -0.2f);
	glVertex3fv(vCorners[7]);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(zero);
	glTexCoord2f(1.0f, -0.2f);
	glVertex3fv(vCorners[9]);
	glTexCoord2f(0.6f, -0.8f);
	glVertex3fv(vCorners[8]);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(zero);
	glTexCoord2f(0.6f, -0.8f);
	glVertex3fv(vCorners[1]);
	glTexCoord2f(0.0f, -1.0f);
	glVertex3fv(vCorners[9]);

	// Bok1
	gltGetNormalVector(vCorners[0], vCorners[1], vCorners[2], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[0]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[1]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[2]);
	// Bok2
	gltGetNormalVector(vCorners[0], vCorners[2], vCorners[3], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[0]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[3]);
	// Bok3
	gltGetNormalVector(vCorners[0], vCorners[3], vCorners[4], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[0]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[3]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[4]);
	// Bok4
	gltGetNormalVector(vCorners[0], vCorners[4], vCorners[5], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[0]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[4]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[5]);
	// Bok5 itd....
	gltGetNormalVector(vCorners[0], vCorners[5], vCorners[6], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[0]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[5]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[6]);

	gltGetNormalVector(vCorners[0], vCorners[6], vCorners[7], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[0]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[6]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[7]);

	gltGetNormalVector(vCorners[0], vCorners[7], vCorners[8], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[0]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[7]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[8]);

	gltGetNormalVector(vCorners[0], vCorners[8], vCorners[9], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[0]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[8]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[9]);

	gltGetNormalVector(vCorners[0], vCorners[9], vCorners[1], vNormal);
	glNormal3fv(vNormal);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3fv(vCorners[0]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(vCorners[9]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(vCorners[1]);
	glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Textured Pyramid");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();
	return 0;
}