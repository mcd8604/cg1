// 3DDrawing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cstdio>

#include "GL/glut.h"

using namespace std;

#define RES_WIDTH 800.0
#define RES_HEIGHT 800.0

float rot = 0.0;

// Holds values for the View transform
struct Camera {
	int ID;

	GLdouble eyeX;
	GLdouble eyeY;
	GLdouble eyeZ;

	GLdouble centerX;
	GLdouble centerY;
	GLdouble centerZ;

	GLdouble upX;
	GLdouble upY;
	GLdouble upZ;

	void CreateLookAt() {
		gluLookAt ( eyeX, eyeY, eyeZ,
			 centerX, centerY, centerZ,
			 upX, upY, upZ);
	}
};

Camera cam1;
Camera cam2;

Camera *curCam;

bool light0;
bool light1;

//Initializes OpenGL and ...
void Initialize() {

	// Init GL 
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	// Init lighting
	glEnable(GL_LIGHTING);
    
	glEnable(GL_LIGHT0);
	light0 = true;

    glEnable(GL_LIGHT1);
    light1 = true;

	//glEnable(GL_LIGHT2);
    
	glEnable(GL_DEPTH_TEST);

	// Init cameras
	cam1 = *new Camera();
	cam1.eyeY = 1.0;
	cam1.eyeZ = 5.0;
	cam1.upY = 1.0;
	cam1.ID = 0;

	cam2 = *new Camera();
	cam2.centerZ = 1.0;
	cam2.upY = 1.0;
	cam2.ID = 1;

	curCam = &cam1;
}

// Updates the scene
void update() {
	// increment rotation to animate gyroscope
	rot += 0.1;
	if(rot > 360) rot = 0.0;
	glutPostRedisplay();
}

GLfloat mat_specular[]      = { 0.1, 0.1, 0.0, 0.0 };

// Draws the gyroscope
void drawGyroscope() {

	// center sphere
	glutSolidSphere(.6, 32 , 32);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	// torus1 
	glRotatef(rot, 0.0, 1.0, 0.0);
	glScalef (1.0, 1.0, 1.0);
	//gluCylinder(gluNewQuadric(), 1.0, 1.0, 0.1, 64, 8); 
	//gluDisk(gluNewQuadric(), 0.9, 1.0, 64, 16);
	glutSolidTorus(0.05, 1.5, 16, 64);

	// torus2
	glPushMatrix();
	glRotatef(rot, 1.0, 0.0, 0.0);
	glScalef (1.1, 1.1, 1.1);
	//gluCylinder(gluNewQuadric(), 1.0, 1.0, 0.1, 64, 8); 
	//gluDisk(gluNewQuadric(), 0.9, 1.0, 64, 16);
	glutSolidTorus(0.05, 1.5, 16, 64);
	
	// torus3 
	glPushMatrix();
	glRotatef(rot, 0.0, 1.0, 0.0);
	glScalef (1.1, 1.1, 1.1);
	//gluCylinder(gluNewQuadric(), 1.0, 1.0, 0.1, 64, 8); 
	//gluDisk(gluNewQuadric(), 0.9, 1.0, 64, 16);
	glutSolidTorus(0.05, 1.5, 16, 64);
	
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_specular);
}

// Draws the base of the scene
void drawBase() {
	glLoadIdentity ();    
	glTranslatef(0.0, -1.5, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluDisk(gluNewQuadric(), 0.0, 4.0, 16, 8); 
}

GLfloat position1[] = { 2.0, -2.0, 2.0, 1.0 };
GLfloat ambient1[] = { 0.2, 0.2, 0.1, 0.5 };
GLfloat diffuse1[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat exponent1[] = { 2.0 };

GLfloat position2[] = { -2.0, 2.0, 2.0, 1.0 };
GLfloat specular2[] = { 0.5, 0.5, 0.5, 1.0 };

// Sets the lighting for draw
void lighting() {
    glLightfv(GL_LIGHT0, GL_POSITION, position1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse1);

    glLightfv(GL_LIGHT1, GL_POSITION, position2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, exponent1);
}

// Draws the graphics
void Draw() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

	glColor3f (1.0, 1.0, 1.0);
	
	// clear the matrix
	glLoadIdentity ();    

	// view transform
	curCam->CreateLookAt();

	// lighting
	lighting();

	// world transforms
    
	//drawBase();

	drawGyroscope();

	glFlush();
	glutSwapBuffers();
}

// Handles keyboard input
void keyboard(unsigned char key, int x, int y) {
	if(key == 113) { // q, quit
		exit(0);
	} else if(key == 99) { // c, switches camera
		curCam = curCam->ID == 0 ? &cam2 : &cam1;
	} else if(key == 49) { // 1
		light0 ? glDisable(GL_LIGHT0) : glEnable(GL_LIGHT0);
		light0 = !light0;
	} else if(key == 50) { // 2
		light1 ? glDisable(GL_LIGHT1) : glEnable(GL_LIGHT1);
		light1 = !light1;
	} else if(key == 51) { // 3

	}

	// d = 100
	// s = 115
	// a = 97
	// w = 119
}

// Handles window resizing
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

// Free up allocated memory
void Unload() {
	delete curCam;
}

int _tmain(int argc, char** argv)
{
	glEnable(GL_DOUBLEBUFFER);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(RES_WIDTH,RES_HEIGHT);
	glutCreateWindow("2D Drawing");

	Initialize();
	
	glutDisplayFunc(Draw); 
	glutReshapeFunc(reshape);
	//glutTimerFunc(20, update, 0);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	Unload();

	return 0;
}

