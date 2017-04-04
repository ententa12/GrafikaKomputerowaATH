#define _CRT_SECURE_NO_WARNINGS

/*
(c) Janusz Ganczarski
http://www.januszg.hg.pl
JanuszG@enter.net.pl
*/

#include <GL/glut.h>
#include "glext.h"
#ifndef WIN32
#define GLX_GLXEXT_LEGACY
#include <GL/glx.h>
#define wglGetProcAddress glXGetProcAddressARB
#endif
#include <stdlib.h>
#include <stdio.h>
#include "colors.h"
#include "targa.h"

// wskaźnik na funkcję glWindowPos2i

PFNGLWINDOWPOS2IPROC glWindowPos2i = NULL;

// stałe do obsługi menu podręcznego

enum
{
	TEXTURE_COMPRESSION_FASTEST,     // kompresja tekstur - GL_FASTEST
	TEXTURE_COMPRESSION_DONT_CARE,   // kompresja tekstur - GL_DONT_CARE
	TEXTURE_COMPRESSION_NICEST,      // kompresja tekstur - GL_NICEST
	TEXTURE_LENA,                    // tekstura lena
	TEXTURE_LENA_UNC,                // teksura lena nieskompresowana
	TEXTURE_LENA_GRAY,               // tekstura lena_gray
	TEXTURE_LENA_GRAY_UNC,           // tekstura lena_gray nieskompresowana
	FULL_WINDOW,                     // aspekt obrazu - całe okno
	ASPECT_1_1,                      // aspekt obrazu 1:1
	EXIT                             // wyjście
};

// aspekt obrazu

int aspect = FULL_WINDOW;

// usunięcie definicji makr near i far

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

// rozmiary bryły obcinania

const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near = 3.0;
const GLdouble far = 7.0;

// identyfikatory tekstur

GLuint LENA, LENA_UNC, LENA_GRAY, LENA_GRAY_UNC;

// identyfikator bieżącej tekstury;

GLuint texture;

// wskazówki do kompresji tekstur

GLint texture_compression_hint = GL_DONT_CARE;

// funkcja rysująca napis w wybranym miejscu
// (wersja korzystająca z funkcji glWindowPos2i)

void DrawString(GLint x, GLint y, char *string)
{
	// położenie napisu
	glWindowPos2i(x, y);

	// wyświetlenie napisu
	int len = strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
}

// funkcja generująca scenę 3D

void DisplayScene()
{
	// kolor tła - zawartość bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// przesunięcie układu współrzędnych obiektów do środka bryły odcinania
	glTranslatef(0.0, 0.0, -(near + far) / 2);

	// przesunięcie obiektu z teksturą do góry okna
	glTranslatef(0.0, 1.0, 0.0);

	// włączenie teksturowania dwuwymiarowego
	glEnable(GL_TEXTURE_2D);

	// ustawienie parametów środowiska tekstur
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// dowiązanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, texture);

	// filtr powiàkszający
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// filtr pomniejszający
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// narysowanie kwadratu z teksturą
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(-1.5, -1.5);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(-1.5, 1.5);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(1.5, 1.5);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(1.5, -1.5);
	glEnd();

	// wyłączenie teksturowania dwuwymiarowego
	glDisable(GL_TEXTURE_2D);

	// wyświetlenie wybranych informacji
	char string[200];
	GLint var;
	glColor3fv(Black);

	// informacja czy tekstura jest skompresowana
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &var);
	if (var == GL_FALSE)
		sprintf(string, "GL_TEXTURE_COMPRESSED = GL_FALSE");
	else
		sprintf(string, "GL_TEXTURE_COMPRESSED = GL_TRUE");
	DrawString(2, 2, string);

	// rozmiar danych tekstury
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &var);
	sprintf(string, "GL_TEXTURE_COMPRESSED_IMAGE_SIZE = %i", var);
	DrawString(2, 16, string);

	// wewnętrzny format tekstury
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &var);
	switch (var)
	{
		// formaty rozszerzenia EXT_texture_compression_s3tc
	case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGB_S3TC_DXT1_EXT");
		break;
	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT");
		break;
	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT");
		break;
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT");
		break;

		// formaty rozszerzenia 3DFX_texture_compression_FXT1
	case GL_COMPRESSED_RGB_FXT1_3DFX:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGB_FXT1_3DFX");
		break;
	case GL_COMPRESSED_RGBA_FXT1_3DFX:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGBA_FXT1_3DFX");
		break;

		// format rozszerzenia ATI_texture_compression_3dc (nie występuje w pliku glext.h)
	case 0x8837:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_COMPRESSED_RGB_3DC_ATI");
		break;

		// wybrane formaty nieskompresowane
	case GL_RED:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_RED");
		break;
	case GL_GREEN:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_GREEN");
		break;
	case GL_BLUE:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_BLUE");
		break;
	case GL_ALPHA:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_ALPHA");
		break;
	case GL_RGB:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_RGB");
		break;
	case GL_RGBA:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_RGBA");
		break;
	case GL_LUMINANCE:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_LUMINANCE");
		break;
	case GL_LUMINANCE_ALPHA:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = GL_LUMINANCE_ALPHA");
		break;

		// pozostałe formaty
	default:
		sprintf(string, "GL_TEXTURE_INTERNAL_FORMAT = nieznany");
		break;
	}
	DrawString(2, 30, string);

	// informacja o wskazówkach do kompresji tekstur
	glGetIntegerv(GL_TEXTURE_COMPRESSION_HINT, &var);
	switch (var)
	{
	case GL_FASTEST:
		sprintf(string, "GL_TEXTURE_COMPRESSION_HINT = GL_FASTEST");
		break;
	case GL_DONT_CARE:
		sprintf(string, "GL_TEXTURE_COMPRESSION_HINT = GL_DONT_CARE");
		break;
	case GL_NICEST:
		sprintf(string, "GL_TEXTURE_COMPRESSION_HINT = GL_NICEST");
		break;
	}
	DrawString(2, 44, string);

	// ilość obsługiwanych formatów kompresji tekstur
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &var);
	sprintf(string, "GL_NUM_COMPRESSED_TEXTURE_FORMATS = %i", var);

	// wykaz obsługiwanych formatów kompresji tekstur
	GLint formats[256];
	glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, formats);
	for (int i = 0; i < var; i++)
	{
		switch (formats[i])
		{
			// formaty rozszerzenia EXT_texture_compression_s3tc
		case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
			sprintf(string, "GL_COMPRESSED_RGB_S3TC_DXT1_EXT");
			break;
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
			sprintf(string, "GL_COMPRESSED_RGBA_S3TC_DXT1_EXT");
			break;
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
			sprintf(string, "GL_COMPRESSED_RGBA_S3TC_DXT3_EXT");
			break;
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			sprintf(string, "GL_COMPRESSED_RGBA_S3TC_DXT5_EXT");
			break;

			// formaty rozszerzenia 3DFX_texture_compression_FXT1
		case GL_COMPRESSED_RGB_FXT1_3DFX:
			sprintf(string, "GL_COMPRESSED_RGB_FXT1_3DFX");
			break;
		case GL_COMPRESSED_RGBA_FXT1_3DFX:
			sprintf(string, "GL_COMPRESSED_RGBA_FXT1_3DFX");
			break;

			// format rozszerzenia ATI_texture_compression_3dc
			// (nie występuje w pliku glext.h)
		case 0x8837:
			sprintf(string, "GL_COMPRESSED_RGB_3DC_ATI");
			break;

			// pozostrałe formaty
		default:
			sprintf(string, "Format nieznany (0x%X)", formats[i]);
			break;
		}
		DrawString(2, 70 + 14 * i, string);
	}

	// skierowanie poleceń do wykonania
	glFlush();

	// zamiana buforów koloru
	glutSwapBuffers();
}

// zmiana wielkości okna

void Reshape(int width, int height)
{
	// obszar renderingu - całe okno
	glViewport(0, 0, width, height);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bryły obcinania
	if (aspect == ASPECT_1_1)
	{
		// wysokość okna większa od wysokości okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom*height / width, top*height / width, near, far);
		else

			// szerokość okna większa lub równa wysokości okna
			if (width >= height && height > 0)
				glFrustum(left*width / height, right*width / height, bottom, top, near, far);
	}
	else
		glFrustum(left, right, bottom, top, near, far);

	// generowanie sceny 3D
	DisplayScene();
}

// utworzenie list wyświetlania

void GenerateTextures()
{
	// zmienne użyte przy obsłudze plików TARGA
	GLsizei width, height;
	GLenum format, type;
	GLvoid *pixels;

	// tryb upakowania bajtów danych tekstury
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// wskazówki do kompresji tesktur
	glHint(GL_TEXTURE_COMPRESSION_HINT, texture_compression_hint);

	// wczytanie tekstury lena.tga
	GLboolean error = load_targa("light_blonde_girl_white_skin.tga", width, height, format, type, pixels);

	// błąd odczytu pliku
	if (error == GL_FALSE)
	{
		printf("Niepoprawny odczyt pliku white skin guy black hairs.tga");
		exit(0);
	}

	// utworzenie identyfikatora tekstury
	glGenTextures(1, &LENA);

	// dowiązanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, LENA);

	// definiowanie tekstury z kompresją
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, width, height, 0, format, type, pixels);

	// utworzenie identyfikatora tekstury
	glGenTextures(1, &LENA_UNC);

	// dowiązanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, LENA_UNC);

	// definiowanie tekstury bez kompresji
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, type, pixels);

	// porządki
	delete[](unsigned char*)pixels;
	
	// wczytanie tekstury lena_gray.tga
	error = load_targa("light_blonde_girl_white_skin.tga", width, height, format, type, pixels);

	// błąd odczytu pliku
	if (error == GL_FALSE)
	{
		printf("Niepoprawny odczyt pliku white skin guy black hairs_gray.tga");
		exit(0);
	}

	// utworzenie identyfikatora tekstury
	glGenTextures(1, &LENA_GRAY);

	// dowiązanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, LENA_GRAY);

	// definiowanie tekstury z kompresją
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_LUMINANCE, width, height, 0, format, type, pixels);

	// utworzenie identyfikatora tekstury
	glGenTextures(1, &LENA_GRAY_UNC);

	// dowiązanie stanu tekstury
	glBindTexture(GL_TEXTURE_2D, LENA_GRAY_UNC);

	// definiowanie tekstury bez kompresji
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, format, type, pixels);

	// porządki
	delete[](unsigned char*)pixels;

	// wybór bieżącej tekstury
	texture = LENA;
}

// obsługa menu podręcznego

void Menu(int value)
{
	GLint tmp_texture;
	switch (value)
	{
		// kompresja tekstur - GL_FASTEST
	case TEXTURE_COMPRESSION_FASTEST:
	{
		texture_compression_hint = GL_FASTEST;
		tmp_texture = texture;
		GenerateTextures();
		texture = tmp_texture;
		DisplayScene();
	}
	break;

	// kompresja tekstur - GL_DONT_CARE
	case TEXTURE_COMPRESSION_DONT_CARE:
	{
		texture_compression_hint = GL_DONT_CARE;
		tmp_texture = texture;
		GenerateTextures();
		texture = tmp_texture;
		DisplayScene();
	}
	break;

	// kompresja tekstur - GL_NICEST
	case TEXTURE_COMPRESSION_NICEST:
	{
		texture_compression_hint = GL_NICEST;
		tmp_texture = texture;
		GenerateTextures();
		texture = tmp_texture;
		DisplayScene();
	}
	break;

	// tekstura lena
	case TEXTURE_LENA:
	{
		texture = LENA;
		DisplayScene();
	}
	break;

	// teksura lena nieskompresowana
	case TEXTURE_LENA_UNC:
	{
		texture = LENA_UNC;
		DisplayScene();
	}
	break;

	// tekstura lena_gray
	case TEXTURE_LENA_GRAY:
	{
		texture = LENA_GRAY;
		DisplayScene();
	}
	break;

	// tekstura lena_gray nieskompresowana
	case TEXTURE_LENA_GRAY_UNC:
	{
		texture = LENA_GRAY_UNC;
		DisplayScene();
	}
	break;

	// obszar renderingu - całe okno
	case FULL_WINDOW:
	{
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
	break;

	// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
	{
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
	break;

	// wyjście
	case EXIT:
		exit(0);
	}
}

// sprawdzenie i przygotowanie obsługi wybranych rozszerzeń

void ExtensionSetup()
{
	// pobranie numeru wersji biblioteki OpenGL
	const char *version = (char*)glGetString(GL_VERSION);

	// odczyt wersji OpenGL
	int major = 0, minor = 0;
	if (sscanf(version, "%d.%d", &major, &minor) != 2)
	{
#ifdef WIN32
		printf("Błędny format wersji OpenGL\n");
#else

		printf("Bledny format wersji OpenGL\n");
#endif

		exit(0);
	}

	// sprawdzenie czy jest co najmniej wersja 1.4
	if (major > 1 || minor >= 4)
	{
		// pobranie wskaźnika wybranej funkcji OpenGL 1.4
		glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress("glWindowPos2i");
	}
	else
		// sprawdzenie czy jest obsługiwane rozszerzenie ARB_window_pos
		if (glutExtensionSupported("GL_ARB_window_pos"))
		{
			// pobranie wskaźnika wybranej funkcji rozszerzenia ARB_window_pos
			glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress
			("glWindowPos2iARB");
		}
		else
		{
			printf("Brak rozszerzenia ARB_window_pos!\n");
			exit(0);
		}

	// sprawdzenie czy jest co najmniej wersja 1.3 OpenGL lub
	// czy jest obsługiwane rozszerzenie ARB_texture_compression
	if (!(major > 1 || minor >= 3) &&
		!glutExtensionSupported("GL_ARB_texture_compression"))
	{
		printf("Brak rozszerzenia GL_ARB_texture_compression!\n");
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// rozmiary głównego okna programu
	glutInitWindowSize(550, 550);

	// utworzenie głównego okna programu
	glutCreateWindow("Kompresja tekstur");

	// dołączenie funkcji generującej scenę 3D
	glutDisplayFunc(DisplayScene);

	// dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// utworzenie podmenu - Tekstura
	int MenuTexture = glutCreateMenu(Menu);
	glutAddMenuEntry("light_blonde_girl_white_skin.tga (kompresja)", TEXTURE_LENA);
	glutAddMenuEntry("light_blonde_girl_white_skin.tga (bez kompresji)", TEXTURE_LENA_UNC);
	glutAddMenuEntry("light_blonde_girl_white_skin.tga (kompresja)", TEXTURE_LENA_GRAY);
	glutAddMenuEntry("light_blonde_girl_white_skin.tga (bez kompresji)", TEXTURE_LENA_GRAY_UNC);

	// utworzenie podmenu - GL_TEXTURE_COMPRESSION_HINT
	int MenuTextureCompressionHint = glutCreateMenu(Menu);
	glutAddMenuEntry("GL_FASTEST", TEXTURE_COMPRESSION_FASTEST);
	glutAddMenuEntry("GL_DONT_CARE", TEXTURE_COMPRESSION_DONT_CARE);
	glutAddMenuEntry("GL_NICEST", TEXTURE_COMPRESSION_NICEST);

	// utworzenie podmenu - Aspekt obrazu
	int MenuAspect = glutCreateMenu(Menu);
#ifdef WIN32

	glutAddMenuEntry("Aspekt obrazu - całe okno", FULL_WINDOW);
#else

	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#endif

	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);

	// menu główne
	glutCreateMenu(Menu);
	glutAddSubMenu("Tekstura", MenuTexture);
	glutAddSubMenu("GL_TEXTURE_COMPRESSION_HINT", MenuTextureCompressionHint);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);

#ifdef WIN32

	glutAddMenuEntry("Wyjście", EXIT);
#else

	glutAddMenuEntry("Wyjscie", EXIT);
#endif

	// określenie przycisku myszki obsługującego menu podręczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// utworzenie tekstur
	GenerateTextures();

	// sprawdzenie i przygotowanie obsługi wybranych rozszerzeń
	ExtensionSetup();

	// wprowadzenie programu do obsługi pętli komunikatów
	glutMainLoop();
	return 0;
}
