
#pragma warning (disable : 4996)

#include "GL\glew.h"
#include "GL\freeglut.h"
#include<iostream>
#include<stdio.h>;
#include<stdlib.h>;
#include<fstream>;
#include<vector>;

#include "Shader_Loader.h";

#define PI	3.14f
#define SPRING_STEP	0.1
#define SPRING_JUMP 0.9
#define ROTATE_STYLE 2

#define FRUSTUM_LEFT -2.0
#define FRUSTUM_RIGHT 2.0
#define FRUSTUM_BOTTOM -2.0
#define FRUSTUM_TOP 2.0
#define FRUSTUM_NEAR 1.0
#define FRUSTUM_FAR 25.0


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

//spring top coordinates

GLfloat springTopX = 0;
GLfloat springTopY = 0;
GLfloat springTopZ = 0;

GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;

GLfloat spreadx = 0.0;
GLfloat spready = 0.0;

GLfloat spread = 0.0;

GLuint metalTexture;

GLuint sphereTexture;

GLfloat springScale = 0.6;

GLfloat timeCounter = 0;

GLUquadric *quad;

bool spirngScaleDir = true; //true up false down

GLfloat xAxisSpringEquation(GLfloat t,GLfloat u)
{
	return(cos(t) * (3 + cos(u)));
}

GLfloat yAxisSpringEquation(GLfloat t, GLfloat u)
{
	return(sin(t) * (3 + cos(u)));
}

GLfloat zAxisSpringEquation(GLfloat t, GLfloat u, GLfloat zParam)
{
	return(zParam * t + sin(u));
}

void DrawCeling(GLfloat x, GLfloat y, GLfloat z, //positions: x,y,x
				GLfloat R, GLfloat G, GLfloat B, //colour: R,G,B
				GLfloat rotDir, GLfloat rotVal,  //rotate direction (-1,1), rotate value
				GLuint texture, bool textureEnable // texture to use, enable texturing
				)
{
	glPushMatrix();
	glColor3f(R, G, B);
		glTranslatef(x, y, z);
		glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
		glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);

		glScalef(20, 20, 0);

		glBegin(GL_QUADS);
			if (textureEnable)
			{
				glEnable(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, metalTexture);
			}

			if (textureEnable)
			{
				glTexCoord3f(x-1,y-1,z);
			}
			glVertex3f(x-1,y-1,z);

			if (textureEnable)
			{
				glTexCoord3f(x - 1, y + 1, z);
			}
			glVertex3f(x - 1, y + 1, z);

			if (textureEnable)
			{
				glTexCoord3f(x + 1, y - 1, z);
			}
			glVertex3f(x + 1, y - 1, z);

			if (textureEnable)
			{
				glTexCoord3f(x + 1, y + 1, z);
			}
			glVertex3f(x + 1, y + 1, z);

		glEnd();
	glPopMatrix();
}

void DrawSphere(GLUquadric *quad,
				GLdouble radius, GLint slices, GLint stacks,
				GLfloat x, GLfloat y, GLfloat z, //positions: x,y,x
				GLfloat R, GLfloat G, GLfloat B, //colour: R,G,B
				GLfloat rotDir, GLfloat rotVal,  //rotate direction (-1,1), rotate value
				GLuint textureId)
{
	glTranslatef(x, y, z);
	glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
	glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	gluQuadricTexture(quad, 1);

	gluSphere(quad, 2, 20, 20);
}

void DrawSpring(GLfloat x, GLfloat y, GLfloat z, //positions: x,y,x
				GLfloat R, GLfloat G, GLfloat B, //colour: R,G,B
				GLfloat rotDir, GLfloat rotVal,  //rotate direction (-1,1), rotate value
				GLuint texture, bool textureEnable, // texture to use, enable texturing
				GLfloat lenghtParam, GLfloat diameterParam,
				GLfloat scaleParam
				)
{
	glPushMatrix();
		glColor3f(R, G, B);
		//glColor4f(R, G, B, 1.0);
		glTranslatef(x, y, z);
			glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
			glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);		

			GLfloat t = 0.0;
			GLfloat u = 0.0;

			if (textureEnable)
			{
				glEnable(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, metalTexture);
			}
			glBegin(GL_QUADS);
			for (t = 0.0; t < (lenghtParam * PI); t = t + SPRING_STEP)
			{
				for (u = 0.0; u < (diameterParam * PI); u = u + SPRING_STEP)
				{
					if (textureEnable)
					{
						glTexCoord3f(xAxisSpringEquation(t, u), yAxisSpringEquation(t, u), zAxisSpringEquation(t, u, scaleParam));
					}
					glVertex3f(xAxisSpringEquation(t, u), yAxisSpringEquation(t, u), zAxisSpringEquation(t, u, scaleParam));

					if (textureEnable)
					{
						glTexCoord3f(xAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP), yAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP), zAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP, scaleParam));
					}
					glVertex3f(xAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP), yAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP), zAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP, scaleParam));

					if (textureEnable)
					{
						glTexCoord3f(xAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP), yAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP), zAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP, scaleParam));
					}
					glVertex3f(xAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP), yAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP), zAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP, scaleParam));

					if (textureEnable)
					{
						glTexCoord3f(xAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP), yAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP), zAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP, scaleParam));
					}
					glVertex3f(xAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP), yAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP), zAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP, scaleParam));

				}
			}
			springTopX = xAxisSpringEquation(t, u);
			springTopY = yAxisSpringEquation(t, u);
			springTopZ = zAxisSpringEquation(t, u,scaleParam);

			glEnd();
	glPopMatrix();
}

void DrawSpringAndSphere(GLfloat x, GLfloat y, GLfloat z, //positions: x,y,x
				GLfloat R, GLfloat G, GLfloat B, //colour: R,G,B
				GLfloat rotDir, GLfloat rotVal,  //rotate direction (-1,1), rotate value
				GLuint texture, bool textureEnable, // texture to use, enable texturing
				GLfloat lenghtParam, GLfloat diameterParam,
				GLfloat scaleParam
				)
{
	glPushMatrix();
	glColor3f(R, G, B);
	//glColor4f(R, G, B, 1.0);
	glTranslatef(x, y, z);
	glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
	glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);



	if (textureEnable)
	{
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, metalTexture);
	}
	glBegin(GL_QUADS);
	for (GLfloat t = 0.0; t < (lenghtParam * PI); t = t + SPRING_STEP)
	{
		for (GLfloat u = 0.0; u < (diameterParam * PI); u = u + SPRING_STEP)
		{
			if (textureEnable)
			{
				glTexCoord3f(xAxisSpringEquation(t, u), yAxisSpringEquation(t, u), zAxisSpringEquation(t, u, scaleParam));
			}
			glVertex3f(xAxisSpringEquation(t, u), yAxisSpringEquation(t, u), zAxisSpringEquation(t, u, scaleParam));

			if (textureEnable)
			{
				glTexCoord3f(xAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP), yAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP), zAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP, scaleParam));
			}
			glVertex3f(xAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP), yAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP), zAxisSpringEquation(t + SPRING_STEP, u + SPRING_STEP, scaleParam));

			if (textureEnable)
			{
				glTexCoord3f(xAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP), yAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP), zAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP, scaleParam));
			}
			glVertex3f(xAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP), yAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP), zAxisSpringEquation(t + SPRING_JUMP, u + SPRING_JUMP, scaleParam));

			if (textureEnable)
			{
				glTexCoord3f(xAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP), yAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP), zAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP, scaleParam));
			}
			glVertex3f(xAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP), yAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP), zAxisSpringEquation(t + SPRING_STEP + SPRING_JUMP, u + SPRING_STEP, scaleParam));

		}
	}
//	DrawSphere(quad, 2, 20, 20, sphereTexture);
	glEnd();
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
void DrawTriangle(GLfloat x, GLfloat y, GLfloat z,			//positions: x,y,x
				  GLfloat R, GLfloat G, GLfloat B,			//colour: R,G,B
				  GLfloat rotX, GLfloat roty, GLfloat rotz, //center of rotation position x,y,z
				  GLfloat lenght,							//lenght of triangle legs
				  GLfloat rotDir, GLfloat rotVal,			//rotate direction (-1,1), rotate value
				  int rotateOrientation,					//rotate style: 1 - 3D, 2 - 2D
				  int TriangleOrientation)					//orientation of triangle in coordinate system, number is equal to a quadrant in wich triangle is wrriten assuming that right-angle is in begining of cs
{
	switch (TriangleOrientation)
	{
	case 1:
		glPushMatrix();
			glColor3f(R, G, B);
			glTranslatef(x, y, z);
			glTranslatef(rotX, roty, rotz);
			if (rotateOrientation == 1)
			{
				glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
				glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);
			};
			if (rotateOrientation == 2)
			{
				glRotatef((rotDir*rotatex) / rotVal, 0.0, 0.0, 1.0);
			};
			glTranslatef(-rotX, -roty, -rotz);
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
			glTranslatef(rotX, roty, rotz);
			if (rotateOrientation == 1)
			{
				glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
				glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);
			};
			if (rotateOrientation == 2)
			{
				glRotatef((rotDir*rotatex) / rotVal, 0.0, 0.0, 1.0);
			};
			glTranslatef(-rotX, -roty, -rotz);
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
			glTranslatef(rotX, roty, rotz);
			if (rotateOrientation == 1)
			{
				glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
				glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);
			};
			if (rotateOrientation == 2)
			{
				glRotatef((rotDir*rotatex) / rotVal, 0.0, 0.0, 1.0);
			};
			glTranslatef(-rotX, -roty, -rotz);
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
			glTranslatef(rotX, roty, rotz);
			if (rotateOrientation == 1)
			{
				glRotatef((rotDir*rotatex) / rotVal, 1.0, 0, 0);
				glRotatef((rotDir*rotatey) / rotVal, 0, 1.0, 0);
			};
			if (rotateOrientation == 2)
			{
				glRotatef((rotDir*rotatex) / rotVal, 0.0, 0.0, 1.0);
			};
			glTranslatef(-rotX, -roty, -rotz);
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

void firstExcercise(GLfloat posX, GLfloat posY, GLfloat posZ,
					GLfloat rotDir, GLfloat rotValue,         //rotate direction (-1,1), rotate value
					GLfloat rotX, GLfloat rotY, GLfloat rotZ
	                )
{
	glTranslatef(posX, posY, posZ);

	//central four triangles
	for (int i = 1; i <= 4; i++)
	{
		DrawTriangle(0, 0, 0, 0, 1, 0, 0.0 ,0.0 ,0.0, 1.0f, 1.0, 1.0, ROTATE_STYLE, i);
	}


	glTranslatef(rotX, rotY, rotZ);

	glRotatef((rotDir*rotatex) / rotValue, rotX, rotY, rotZ);

	////first row of triangles
	for (int i = -4; i <= 4; i++)
	{
		for (int k = -4; k <= 4; k++)
		{
			if (i >= 0 && k >= 0)
			{
				if (i == 0 && k == 1)
				{
					DrawTriangle(i + spread, k + spread, 0, 1, 0, 0, 0.33, 0.33, 0, 1.0f, -1.0, 3.0, ROTATE_STYLE, 1);
				}
				if (i == 1 && k == 0)
				{
					DrawTriangle(i + spread, k + spread, 0, 0, 0, 1, 0.33, 0.33, 0, 1.0f, -1.0, 3.0, ROTATE_STYLE, 1);
				}
			}
			if (i >= 0 && k <= 0)
			{
				if (i == 0 && k == -1)
				{
					DrawTriangle(i + spread, k - spread, 0, 0, 0, 1, 0.33, 0.33, 0, 1.0f, -1.0, 3.0, ROTATE_STYLE, 4);
				}
				if (i == 1 && k == 0)
				{
					DrawTriangle(i + spread, k - spread, 0, 1, 0, 0, 0.33, 0.33, 0, 1.0f, -1.0, 3.0, ROTATE_STYLE, 4);
				}
			}
			if (i <= 0 && k >= 0)
			{
				if (i == -1 && k == 0)
				{
					DrawTriangle(i - spread, k + spread, 0, 1, 0, 0, 0.33, 0.33, 0, 1.0f, -1.0, 3.0, ROTATE_STYLE, 2);
				}
				if (i == 0 && k == 1)
				{
					DrawTriangle(i - spread, k + spread, 0, 0, 0, 1, 0.33, 0.33, 0, 1.0f, -1.0, 3.0, ROTATE_STYLE, 2);
				}
			}
			if (i <= 0 && k <= 0)
			{
				if (i == -1 && k == 0)
				{
					DrawTriangle(i - spread, k - spread, 0, 0, 0, 1, 0.33, 0.33, 0, 1.0f, -1.0, 3.0, ROTATE_STYLE, 3);
				}
				if (i == 0 && k == -1)
				{
					DrawTriangle(i - spread, k - spread, 0, 1, 0, 0, 0.33, 0.33, 0, 1.0f, -1.0, 3.0, ROTATE_STYLE, 3);
				}
			}
		}
	}
}

void secondExcercise()
{
	DrawCeling(0, 0, 0, 0.3, 0.3, 0.3, -1, 3, metalTexture, false);

	DrawSpring(0.0, 0.0, 0.0, 0.3, 0.3, 0.3, -1, 3, metalTexture, true, 8, 2, springScale);

	DrawSphere(quad, 2, 20, 20, springTopY, springTopZ, springTopX, 0, 0, 0, 1, 1, sphereTexture);
}


GLuint LoadTexture(const char * filename, int width, int height)
{
	GLuint texture;
	unsigned char * data;
	FILE * file;

	//The following code will read in our RAW file
		file = fopen(filename, "rb");  //We need to open our file
		if (file == NULL) return 0;  //If our file is empty, set our texture to empty


			data = (unsigned char *)malloc(width * height * 3); //assign the nessecary memory for the texture


			fread(data, width * height * 3, 1, file);  //read in our file
			fclose(file); //close our file, no point leaving it open

			glGenTextures(1, &texture); //then we need to tell OpenGL that we are generating a texture
			glBindTexture(GL_TEXTURE_2D, texture); //now we bind the texture that we are working with
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
			//The parameter GL_MODULATE will blend the texture with whatever is underneath, setting it to GL_DECAL
			//will tell the texture to replace whatever is on the object.

			//here we are setting what textures to use and when.The MIN filter is which quality to show
			//when the texture is near the view, and the MAG filter is which quality to show when the texture
			//is far from the view.

			//The qualities are(in order from worst to best)
			//GL_NEAREST
			//GL_LINEAR
			//GL_LINEAR_MIPMAP_NEAREST
			//GL_LINEAR_MIPMAP_LINEAR

			//The two mipmap variables only work in textures with generated mipmaps, so you will see that in action
			//in a later tutorial.

			//And if you go and use extensions, you can use Anisotropic filtering textures which are of an
			//even better quality, but this will do for now.

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Here we are setting the parameter to repeat the texture instead of clamping the texture
		//to the edge of our shape.

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	free(data);// free the texture
	return texture;// return the texture data
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

	//firstExcercise(0,0,0,1,1,0,0,1);

	/*DrawCeling(0, 0, 0, 0.3, 0.3, 0.3, -1, 3, metalTexture, false);

	DrawSpring(0.0, 0.0, 0.0, 0.3, 0.3, 0.3, -1, 3, metalTexture, true, 8, 2, springScale);

	DrawSphere(quad, 2, 20, 20, springTopY, springTopZ, springTopX, 0, 0, 0, 1, 1, sphereTexture);*/

	secondExcercise();

	

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
			glFrustum(FRUSTUM_LEFT, FRUSTUM_RIGHT, FRUSTUM_BOTTOM * height / width, FRUSTUM_TOP * height / width, FRUSTUM_NEAR, FRUSTUM_FAR);
		else

			// szerokoœæ okna wiêksza lub równa wysokoœci okna
			if (width >= height && height > 0)
				glFrustum(FRUSTUM_LEFT * width / height, FRUSTUM_RIGHT * width / height, FRUSTUM_BOTTOM, FRUSTUM_TOP, FRUSTUM_NEAR, FRUSTUM_FAR);

	}
	else
		glFrustum(FRUSTUM_LEFT, FRUSTUM_RIGHT, FRUSTUM_BOTTOM, FRUSTUM_TOP, FRUSTUM_NEAR, FRUSTUM_FAR);

	// generowanie sceny 3D
	Display();
}

// obs³uga klawiatury

static void timerCallback(int value)
{
	/* Do timer processing */
	/* maybe glutPostRedisplay(), if necessary */

	if (springScale < 1 && spirngScaleDir == true)
	{
		springScale += cosf(timeCounter);
	}
	if (springScale > 0.6 && spirngScaleDir == false)
	{
		springScale -= cosf(timeCounter);
	}
	if (springScale < 0.6)
	{
		spirngScaleDir = true;
	}
	if (springScale > 1)
	{
		spirngScaleDir = false;
	}

	timeCounter += 1;

	Display();

	/* call back again after elapsedUSecs have passed */
	glutTimerFunc(10, timerCallback, value);
}

void Keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
		case '+':
		{
			eyez -= 0.1;
			break;
		}
		case '-':
		{
			eyez += 0.1;
			break;
		}
		case 'w':
		{
			springScale += 0.1;
			break;
		}
		case 's':
		{
			springScale -= 0.1;
			break;
		}
	}
	
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
	case GLUT_KEY_F3:
		rotatex -= 1;
		spread += 0.001;
		break;

		// kursor w prawo
	case GLUT_KEY_F2:
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
	glutInitWindowSize(800, 800);

	// utworzenie g³ównego okna programu
#ifdef WIN32

	glutCreateWindow("Szeœcian 4");
#else

	glutCreateWindow("Szescian 4");
#endif

	//Ustawianie tekstury
	metalTexture = LoadTexture("C:\\Users\\piotr_komp\\Documents\\Visual Studio 2013\\Projects\\OpenGLSampleV2\\bmetal.bmp", 256, 256);

	sphereTexture = LoadTexture("C:\\Users\\piotr_komp\\Documents\\Visual Studio 2013\\Projects\\OpenGLSampleV2\\earth.bmp", 1024, 512);

	quad = gluNewQuadric();



	// do³¹czenie funkcji generuj¹cej scenê 3D
	glutDisplayFunc(Display);

	// do³¹czenie funkcji wywo³ywanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// do³¹czenie funkcji obs³ugi klawiatury
	glutKeyboardFunc(Keyboard);

	// do³¹czenie funkcji obs³ugi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);

	glutTimerFunc(100, timerCallback, springScale);

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