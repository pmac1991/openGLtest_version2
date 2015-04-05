
#include "GL\glew.h"
#include "GL\freeglut.h"
#include<iostream>
#include<stdio.h>;
#include<stdlib.h>;
#include<fstream>;
#include<vector>;

#include "Shader_Loader.h";

#define PI	3.14
#define SPRING_STEP	0.1

using namespace Core;

enum
{
	FULL_WINDOW, // aspekt obrazu - ca³e okno
	ASPECT_1_1, // aspekt obrazu 1:1
	EXIT // wyjœcie
};

// aspekt obrazu

int Aspect = FULL_WINDOW;

// wpó³rzêdne po³o¿enia obserwatora

GLdouble eyex = 0;
GLdouble eyey = 0;
GLdouble eyez = 3;

// wspó³rzêdne punktu w którego kierunku jest zwrócony obserwator,

GLdouble centerx = 0;
GLdouble centery = 0;
GLdouble centerz = -100;

GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;

GLfloat spreadx = 0.0;
GLfloat spready = 0.0;

GLfloat spread = 0.0;

GLfloat xAxisSpringEquation(GLfloat t,GLfloat u)
{
	return(cos(t) * (3 + cos(u)));
}

GLfloat yAxisSpringEquation(GLfloat t, GLfloat u)
{
	return(sin(t) * (3 + cos(u)));
}

GLfloat zAxisSpringEquation(GLfloat t, GLfloat u)
{
	return(0.6*t + sin(u));
}

void DrawSpring(GLfloat x, GLfloat y, GLfloat z, //positions: x,y,x
				GLfloat R, GLfloat G, GLfloat B, //colour: R,G,B
				GLfloat rotDir, GLfloat rotVal  //rotate direction (-1,1), rotate value
				)
{
	glPushMatrix();
		glColor3f(R, G, B);
		glTranslatef(x, y, z);
			glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
			glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);
			glBegin(GL_LINES);
			for (GLfloat t = 0.0; t < (8 * PI); t = t + SPRING_STEP)
			{
				for (GLfloat u = 0.0; u < (2 * PI); u = u + SPRING_STEP)
				{
					glVertex3f(xAxisSpringEquation(t, u), yAxisSpringEquation(t, u), zAxisSpringEquation(t, u));
				}
			}
	glPopMatrix();
}

void DrawSky()
{
	// pocz¹tek definicji krawêdzi szeœcianu

	glPushMatrix();
	glBegin(GL_LINES);

		// wspólrzêdne kolejnych krawêdzi szeœcianu
		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, -1.0, 1.0);

		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(1.0, -1.0, -1.0);

		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(1.0, 1.0, -1.0);

		glVertex3f(1.0, 1.0, -1.0);
		glVertex3f(1.0, 1.0, 1.0);

		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);

		glVertex3f(-1.0, -1.0, 1.0);
		glVertex3f(-1.0, -1.0, -1.0);

		glVertex3f(-1.0, -1.0, -1.0);
		glVertex3f(-1.0, 1.0, -1.0);

		glVertex3f(-1.0, 1.0, -1.0);
		glVertex3f(-1.0, 1.0, 1.0);

		glVertex3f(1.0, 1.0, 1.0);
		glVertex3f(-1.0, 1.0, 1.0);

		glVertex3f(1.0, -1.0, 1.0);
		glVertex3f(-1.0, -1.0, 1.0);

		glVertex3f(1.0, -1.0, -1.0);
		glVertex3f(-1.0, -1.0, -1.0);

		glVertex3f(1.0, 1.0, -1.0);
		glVertex3f(-1.0, 1.0, -1.0);


	// koniec definicji prymitywu
	glEnd();
	glPopMatrix();

}

//funkcja rysuj¹ca trójk¹t w punkcie o przesunêciu
void DrawTriangle(GLfloat x, GLfloat y, GLfloat z, //positions: x,y,x
				  GLfloat R, GLfloat G, GLfloat B, //colour: R,G,B
				  GLfloat lenght,                  //lenght of triangle legs
				  GLfloat rotDir, GLfloat rotVal,  //rotate direction (-1,1), rotate value
				  int TriangleOrientation)         //orientation of triangle in coordinate system, number is equal to a quadrant in wich triangle is wrriten assuming that right-angle is in begining of cs
{
	switch (TriangleOrientation)
	{
	case 1:
		glPushMatrix();
			glColor3f(R, G, B);
			glTranslatef(x, y, z);
			glTranslatef(0.33, 0.33, z);
			glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
			glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);
			glTranslatef(-0.33, -0.33, z);
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, lenght, 0.0f);
				glVertex3f(lenght, 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
			glEnd();
		glPopMatrix();
		break;
	case 2:
		glPushMatrix();
			glColor3f(R, G, B);
			glTranslatef(x, y, z);
			glTranslatef(0.33, 0.33, z);
			glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
			glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);
			glTranslatef(-0.33, -0.33, z);
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, lenght, 0.0f);
				glVertex3f(-lenght, 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
			glEnd();
		glPopMatrix();
		break;
	case 3:
		glPushMatrix();
			glColor3f(R, G, B);
			glTranslatef(x, y, z);
			glTranslatef(0.33, 0.33, z);
			glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
			glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);
			glTranslatef(-0.33, -0.33, z);
				glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, -lenght, 0.0f);
				glVertex3f(-lenght, 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
			glEnd();
		glPopMatrix();
		break;
	case 4:
		glPushMatrix();
			glColor3f(R, G, B);
			glTranslatef(x , y, z);
			glTranslatef(0.33, 0.33, z);
			glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
			glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);
			glTranslatef(-0.33, -0.33, z);
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, -lenght, 0.0f);
				glVertex3f(lenght, 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
			glEnd();
		glPopMatrix();
		break;
	default:
		break;
	}
}

void firstExcercise()
{
	//central four triangles
	for (int i = 1; i <= 4; i++)
	{
		DrawTriangle(0, 0, 0, 0, 1, 0, 1.0f, 1.0, 1.0, i);
	}

	////first row of triangles
	for (int i = -4; i <= 4; i++)
	{
		for (int k = -4; k <= 4; k++)
		{
			if (i >= 0 && k >= 0)
			{
				if (i == 0 && k == 1)
				{
					DrawTriangle(i + spread, k + spread, 0, 1, 0, 0, 1.0f, -1.0, 3.0, 1);
				}
				if (i == 1 && k == 0)
				{
					DrawTriangle(i + spread, k + spread, 0, 0, 0, 1, 1.0f, -1.0, 3.0, 1);
				}
			}
			if (i >= 0 && k <= 0)
			{
				if (i == 0 && k == -1)
				{
					DrawTriangle(i + spread, k - spread, 0, 0, 0, 1, 1.0f, -1.0, 3.0, 4);
				}
				if (i == 1 && k == 0)
				{
					DrawTriangle(i + spread, k - spread, 0, 1, 0, 0, 1.0f, -1.0, 3.0, 4);
				}
			}
			if (i <= 0 && k >= 0)
			{
				if (i == -1 && k == 0)
				{
					DrawTriangle(i - spread, k + spread, 0, 1, 0, 0, 1.0f, -1.0, 3.0, 2);
				}
				if (i == 0 && k == 1)
				{
					DrawTriangle(i - spread, k + spread, 0, 0, 0, 1, 1.0f, -1.0, 3.0, 2);
				}
			}
			if (i <= 0 && k <= 0)
			{
				if (i == -1 && k == 0)
				{
					DrawTriangle(i - spread, k - spread, 0, 0, 0, 1, 1.0f, -1.0, 3.0, 3);
				}
				if (i == 0 && k == -1)
				{
					DrawTriangle(i - spread, k - spread, 0, 1, 0, 0, 1.0f, -1.0, 3.0, 3);
				}
			}
		}
	}
}

// funkcja generuj¹ca scenê 3D
void Display()
{
	// kolor t³a - zawartoœæ bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// ustawienie obserwatora
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);

	// kolor krawêdzi szeœcianu
	glColor3f(0.0, 0.0, 0.0);

	// obroty obiektu - klawisze kursora
	//glRotatef(rotatex, 1.0, 0, 0);
	//glRotatef(rotatey, 0, 1.0, 0);
	
//	glutWireTeapot(1.0);

	GLfloat j = 0.0;
	
	//firstExcercise();

	DrawSpring(0.0, 1.0, 1.0,1.0,0.0,0.0,1,1);

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
	if (Aspect == ASPECT_1_1)
	{
		// wysokoœæ okna wiêksza od wysokoœci okna
		if (width < height && width > 0)
			glFrustum(-2.0, 2.0, -2.0 * height / width, 2.0 * height / width, 1.0, 5.0);
		else

			// szerokoœæ okna wiêksza lub równa wysokoœci okna
			if (width >= height && height > 0)
				glFrustum(-2.0 * width / height, 2.0 * width / height, -2.0, 2.0, 1.0, 5.0);

	}
	else
		glFrustum(-2.0, 2.0, -2.0, 2.0, 1.0, 5.0);

	// generowanie sceny 3D
	Display();
}

// obs³uga klawiatury

void Keyboard(unsigned char key, int x, int y)
{
	// klawisz +
	if (key == '+')
		eyez -= 0.1;
	else

		// klawisz -
		if (key == '-')
			eyez += 0.1;

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obs³uga klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		// kursor w lewo
	case GLUT_KEY_LEFT:
		eyex += 0.1;
		break;

		// kursor w górê
	case GLUT_KEY_UP:
		eyey -= 0.1;
		break;

		// kursor w prawo
	case GLUT_KEY_RIGHT:
		eyex -= 0.1;
		break;

		// kursor w dó³
	case GLUT_KEY_DOWN:
		eyey += 0.1;
		break;
		
		// kursor w lewo
	case GLUT_KEY_F1:
		rotatey -= 1;
		spread += 0.001;
		break;

		// kursor w górê
	case GLUT_KEY_F2:
		rotatex -= 1;
		spread += 0.001;
		break;

		// kursor w prawo
	case GLUT_KEY_F3:
		rotatey += 1;
		spread += 0.001;
		break;

		// kursor w dó³
	case GLUT_KEY_F4:
		rotatex += 1;
		spread += 0.001;
		break;
	}

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obs³uga menu podrêcznego

void Menu(int value)
{
	switch (value)
	{
		// obszar renderingu - ca³e okno
	case FULL_WINDOW:
		Aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
		Aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// rozmiary g³ównego okna programu
	glutInitWindowSize(400, 400);

	// utworzenie g³ównego okna programu
#ifdef WIN32

	glutCreateWindow("Szeœcian 4");
#else

	glutCreateWindow("Szescian 4");
#endif

	// do³¹czenie funkcji generuj¹cej scenê 3D
	glutDisplayFunc(Display);

	// do³¹czenie funkcji wywo³ywanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// do³¹czenie funkcji obs³ugi klawiatury
	glutKeyboardFunc(Keyboard);

	// do³¹czenie funkcji obs³ugi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);

	// utworzenie menu podrêcznego
	glutCreateMenu(Menu);

	// dodanie pozycji do menu podrêcznego
#ifdef WIN32

	glutAddMenuEntry("Aspekt obrazu - ca³e okno", FULL_WINDOW);
	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);
	glutAddMenuEntry("Wyjœcie", EXIT);
#else

	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);
	glutAddMenuEntry("Wyjscie", EXIT);
#endif

	// okreœlenie przycisku myszki obs³uguj¹cej menu podrêczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// wprowadzenie programu do obs³ugi pêtli komunikatów
	glutMainLoop();
	return 0;
}