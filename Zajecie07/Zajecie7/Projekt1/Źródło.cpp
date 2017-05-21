

/*
(c) Janusz Ganczarski
http://www.januszg.hg.pl
JanuszG@enter.net.pl
*/

#include <GL/glut.h>
#include <stdlib.h>
#include "colors.h"

// sta³e do obs³ugi menu podrêcznego

enum
{
	FULL_WINDOW = GL_ALWAYS + 100, // aspekt obrazu - ca³e okno
	ASPECT_1_1, // aspekt obrazu 1:1
	EXIT // wyjœcie
};

// aspekt obrazu

int aspect = FULL_WINDOW;

// rozmiary bry³y obcinania

const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near_ = 3.0;
const GLdouble far_ = 7.0;

// k¹ty obrotu

GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;

// wskaŸnik naciœniêcia lewego przycisku myszki

int button_state = GLUT_UP;

// po³o¿enie kursora myszki

int button_x, button_y;

// rodzaj testu bufora g³êbokoœci

GLenum depth_test = GL_LESS;

// funkcja generuj¹ca scenê 3D

GLfloat vertex[11 * 3] =
{

	0.0f, 1.0f, 0.0f,
	0.0f, -1.0f, -1.0f,
	-0.6f, -1.0f, -0.80f,
	-1.0f, -1.0f, -0.2f,
	-0.9f, -1.0f, 0.5f,
	-0.3f, -1.0f, 0.9f,
	0.3f, -1.0f, 0.9f,
	0.9f, -1.0f, 0.5f,
	1.0f, -1.0f, -0.2f,
	0.6f, -1.0f, -0.8f,
	0.0, -1.0,  0.0,
};

int triangles[18 * 3] =
{
	1,2,0,
	2,3,0,
	3,4,0,
	4,5,0,
	5,6,0,
	6,7,0,
	7,8,0,
	8,9,0,
	9,1,0,
	2,1,10,
	3,2,10,
	4,3,10,
	5,4,10,
	6,5,10,
	7,6,10,
	8,7,10,
	9,8,10,
	1,9,10,
};

void Normal(GLfloat *n, int i)
{
	GLfloat v1[3], v2[3];
	// obliczenie wektorów na podstawie wspó³rzêdnych wierzcho³ków trójk¹tów
	v1[0] = vertex[3 * triangles[3 * i + 1] + 0] - vertex[3 * triangles[3 * i + 0] + 0];
	v1[1] = vertex[3 * triangles[3 * i + 1] + 1] - vertex[3 * triangles[3 * i + 0] + 1];
	v1[2] = vertex[3 * triangles[3 * i + 1] + 2] - vertex[3 * triangles[3 * i + 0] + 2];
	v2[0] = vertex[3 * triangles[3 * i + 2] + 0] - vertex[3 * triangles[3 * i + 1] + 0];
	v2[1] = vertex[3 * triangles[3 * i + 2] + 1] - vertex[3 * triangles[3 * i + 1] + 1];
	v2[2] = vertex[3 * triangles[3 * i + 2] + 2] - vertex[3 * triangles[3 * i + 1] + 2];
	// obliczenie waktora normalnego przy pomocy iloczynu wektorowego
	n[0] = v1[1] * v2[2] - v1[2] * v2[1];
	n[1] = v1[2] * v2[0] - v1[0] * v2[2];
	n[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

// wskaŸnik przesuniêcia wartoœci g³êbi
bool polygon_offset = true;

// k¹t obrotu kuli
GLfloat angle = 0.0;

// wskaŸnik rysowania p³aszczyzna przekroju
bool cutting_plane = true;


void Display()
{
	// kolor t³a - zawartoœæ bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// czyszczenie bufora koloru i bufora g³êbokoœci
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// przesuniêcie uk³adu wspó³rzêdnych szeœcianu do œrodka bry³y odcinania
	glTranslatef(0, 0, -(near_ + far_) / 2);

	// obroty szeœcianu
	glRotatef(rotatex, 1.0, 0, 0);
	glRotatef(rotatey, 0, 1.0, 0);

	// niewielkie powiêkszenie szeœcianu
	glScalef(2, 2, 2);

	// w³¹czenie testu bufora g³êbokoœci
	glEnable(GL_DEPTH_TEST);

	// wybór funkcji do testu bufora g³êbokoœci
	glDepthFunc(depth_test);

	// najpierw rysujemy kulê obracaj¹c¹ siê wewn¹trz szeœcianu;
	// z uwagi na celowy brak efektów oœwietlenia, obrót kuli
	// podkreœla druga kula w wersji "szkieletowej"
	glPushMatrix();
	angle += 0.1;
	glRotatef(angle, 0.5, 0.5, 0.0);
	glColor3fv(Red);
	if (polygon_offset)
		glEnable(GL_POLYGON_OFFSET_FILL);

	glPolygonOffset(0.5, 0.5);
	glutSolidSphere(0.3, 10, 10);
	glColor3fv(Black);
	glutWireSphere(0.3, 10, 10);
	if (polygon_offset)
		glDisable(GL_POLYGON_OFFSET_FILL);

	glPopMatrix();

	// rysowanie p³aszczyzny otworu w szeœcianie

	if (cutting_plane)

	{
		// wy³¹czenie rysowania w buforze kolorów
		glDrawBuffer(GL_NONE);
		// rysowanie kwadratu czêœciowo ods³aniaj¹cego wnêtrze szeœcianu
		// (kwadrat jest po³o¿ony o 0,001 jednostki nad bokiem szeœcianu)
		glBegin(GL_QUADS);
		glVertex3f(-0.5, -1.001f, -0.5);
		glVertex3f(0.5, -1.001f, -0.5);
		glVertex3f(0.5, -1.001f, 0.5);
		glVertex3f(-0.5, -1.001f, 0.5);
		glEnd();


		// w³¹czenie rysowania w buforze kolorów

		glDrawBuffer(GL_BACK);

	}


	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 18; i++)
	{
		GLfloat n[3];
		Normal(n, i);
		// uœredniony wektor normalny jest normalizowany tylko, gdy biblioteka
		// obs³uguje automatyczne skalowania jednostkowych wektorów normalnych
		glNormal3fv(n);
		glColor3fv(Lime);
		glVertex3fv(&vertex[3 * triangles[3 * i + 0]]);
		glColor3fv(Yellow);
		glVertex3fv(&vertex[3 * triangles[3 * i + 1]]);
		glColor3fv(Blue);
		glVertex3fv(&vertex[3 * triangles[3 * i + 2]]);
	}
	// koniec definicji szeœcianu RGB
	glEnd();

	// skierowanie poleceñ do wykonania
	glFlush();

	// zamiana buforów koloru
	glutSwapBuffers();
}

// zmiana wielkoœci okna

void Reshape(int width, int height)
{
	// obszar renderingu - ca³e okno
	glViewport(0, 0, width, height);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bry³y obcinania
	if (aspect == ASPECT_1_1)
	{
		// wysokoœæ okna wiêksza od wysokoœci okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom * height / width, top * height / width, near_, far_);
		else

			// szerokoœæ okna wiêksza lub równa wysokoœci okna
			if (width >= height && height > 0)
				glFrustum(left * width / height, right * width / height, bottom, top, near_, far_);

	}
	else
		glFrustum(left, right, bottom, top, near_, far_);

	// generowanie sceny 3D
	Display();
}

// obs³uga klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		// kursor w lewo
	case GLUT_KEY_LEFT:
		rotatey -= 1;
		break;

		// kursor w górê
	case GLUT_KEY_UP:
		rotatex -= 1;
		break;

		// kursor w prawo
	case GLUT_KEY_RIGHT:
		rotatey += 1;
		break;

		// kursor w dó³
	case GLUT_KEY_DOWN:
		rotatex += 1;
		break;
	}

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obs³uga przycisków myszki

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// zapamiêtanie stanu lewego przycisku myszki
		button_state = state;

		// zapamiêtanie po³o¿enia kursora myszki
		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}

// obs³uga ruchu kursora myszki

void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		rotatey += 30 * (right - left) / glutGet(GLUT_WINDOW_WIDTH) *(x - button_x);
		button_x = x;
		rotatex -= 30 * (top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) *(button_y - y);
		button_y = y;
		glutPostRedisplay();
	}
}

// obs³uga menu podrêcznego

void Menu(int value)
{
	switch (value)
	{
		// obszar renderingu - ca³e okno
	case FULL_WINDOW:
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// test bufora g³êbokoœci: GL_NEVER
	case GL_NEVER:
		depth_test = GL_NEVER;
		Display();
		break;

		// test bufora g³êbokoœci: GL_LESS
	case GL_LESS:
		depth_test = GL_LESS;
		Display();
		break;

		// test bufora g³êbokoœci: GL_EQUAL
	case GL_EQUAL:
		depth_test = GL_EQUAL;
		Display();
		break;

		// test bufora g³êbokoœci: GL_LEQUAL
	case GL_LEQUAL:
		depth_test = GL_LEQUAL;
		Display();
		break;

		// test bufora g³êbokoœci: GL_GREATER
	case GL_GREATER:
		depth_test = GL_GREATER;
		Display();
		break;

		// test bufora g³êbokoœci: GL_NOTEQUAL
	case GL_NOTEQUAL:
		depth_test = GL_NOTEQUAL;
		Display();
		break;

		// test bufora g³êbokoœci: GL_GEQUAL
	case GL_GEQUAL:
		depth_test = GL_GEQUAL;
		Display();
		break;

		// test bufora g³êbokoœci: GL_ALWAYS
	case GL_ALWAYS:
		depth_test = GL_ALWAYS;
		Display();
		break;

		// wyjœcie
	case EXIT:
		exit(0);
	}
}

int main(int argc, char * argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// rozmiary g³ównego okna programu
	glutInitWindowSize(500, 500);

	// utworzenie g³ównego okna programu
#ifdef WIN32

	glutCreateWindow("Szeœcian 5");
#else

	glutCreateWindow("Szescian 5");
#endif

	// do³¹czenie funkcji generuj¹cej scenê 3D
	glutDisplayFunc(Display);

	// do³¹czenie funkcji wywo³ywanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// do³¹czenie funkcji obs³ugi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);

	// obs³uga przycisków myszki
	glutMouseFunc(MouseButton);

	// obs³uga ruchu kursora myszki
	glutMotionFunc(MouseMotion);

	// utworzenie menu podrêcznego
	glutCreateMenu(Menu);

	// utworzenie podmenu - aspekt obrazu
	int MenuAspect = glutCreateMenu(Menu);
#ifdef WIN32

	glutAddMenuEntry("Aspekt obrazu - ca³e okno", FULL_WINDOW);
#else

	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#endif

	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);

	// utworzenie podmenu - Test z-bufora
	int MenuZbuffer = glutCreateMenu(Menu);
	glutAddMenuEntry("GL_NEVER", GL_NEVER);
	glutAddMenuEntry("GL_LESS", GL_LESS);
	glutAddMenuEntry("GL_EQUAL", GL_EQUAL);
	glutAddMenuEntry("GL_LEQUAL", GL_LEQUAL);
	glutAddMenuEntry("GL_GREATER", GL_GREATER);
	glutAddMenuEntry("GL_NOTEQUAL", GL_NOTEQUAL);
	glutAddMenuEntry("GL_GEQUAL", GL_GEQUAL);
	glutAddMenuEntry("GL_ALWAYS", GL_ALWAYS);

	// menu g³ówne
	glutCreateMenu(Menu);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
	glutAddSubMenu("Test z-bufora", MenuZbuffer);
#ifdef WIN32

	glutAddMenuEntry("Wyjœcie", EXIT);
#else

	glutAddMenuEntry("Wyjscie", EXIT);
#endif

	// okreœlenie przycisku myszki obs³uguj¹cej menu podrêczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// wprowadzenie programu do obs³ugi pêtli komunikatów
	glutMainLoop();
	return 0;
}

//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////////
