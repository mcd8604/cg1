// 3DDrawing.cpp : Defines the entry point for the console application.
// author: Mike DeMauro

#include "stdafx.h"
#include <stdlib.h>
#include <cstdio>
#include <cmath>

#include "GL/glut.h"

using namespace std;


#define RES_WIDTH 800.0
#define RES_HEIGHT 800.0

float rot = 0.0;
#define ROT_ACCEL 0.01;
float rotSpeed = ROT_ACCEL;

#define PI 3.14159265

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
bool light2;
bool light3;

// array of texture id's
GLuint textures[2];

// Sets up lighting for 4 light sources
void InitLighting() {
	glEnable(GL_LIGHTING);
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

	glEnable(GL_LIGHT0);
	light0 = true;

    glEnable(GL_LIGHT1);
    light1 = true;

	glEnable(GL_LIGHT2);
    light2 = true;

	glEnable(GL_LIGHT3);
    light3 = true;
    
	glEnable(GL_NORMALIZE);

	glEnable(GL_DEPTH_TEST);
}

// Sets up two cameras
void InitCameras() {
	cam1 = *new Camera();
	cam1.eyeY = 0.0;
	cam1.eyeZ = 5.0;
	cam1.upY = 1.0;
	cam1.ID = 0;

	cam2 = *new Camera();
	cam2.upY = 1.0;
	cam2.ID = 1;

	curCam = &cam1;
}

/*
 *  LoadTexturePPM - copied from Nan's directory.  Originally
 *  written by: Blaine Hodge
 */
int LoadTexturePPM( const char * filename, int w, int h, int color )
{
    GLubyte *data;
    FILE * file;

    /* open texture data */
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;

    /* allocate buffer */
    data = (GLubyte*)malloc( w * h * 3 * (sizeof(GLubyte)));

    /* read texture data */
    fread( data, w * h * 3, 1, file );
    fclose( file );

    /* build our tex image */

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    if (color) {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                                      GL_UNSIGNED_BYTE, data );

    } else {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE,
                                      GL_UNSIGNED_BYTE, data );
    }

    /* free buffer */
    free( data );

    return 1;
}

// Loads textures
void LoadTextures() {    glPixelStorei( GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(2, textures);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    if (!LoadTexturePPM("metal.bmp", 256, 256, 1)) {
        printf("Error loading texture!");
    }
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    if (!LoadTexturePPM("metalFloor.bmp", 256, 256, 1)) {
        printf("Error loading texture!");
    }
}


//Initializes OpenGL and ...
void Initialize() {

	// Init GL 
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	// Init lighting
	InitLighting();

	// Init cameras
	InitCameras();

	// Init Textures
	glEnable( GL_TEXTURE_2D );
	LoadTextures();
	
	// clear the matrix
	glLoadIdentity ();    
	
}

// Updates the scene
void update() {
	
	// increment rotation to animate gyroscope
	rot += rotSpeed;
	if(rot > 360) rot = 0.0;

	// update camera 2	
	// TODO: optimize
	float x1, y1, z1, x2, y2, z2;
	float rad = (rot) * PI / 180;

	// y-axis
	x1 = cos(rad);
	y1 = 0;
	z1 = sin(rad);

	// x-axis
	x2 = 0;
	y2 = cos(rad);
	z2 = sin(rad);

	cam2.eyeX = x1 + x2;
	cam2.eyeY = y1 + y2;
	cam2.eyeZ = z1 + z2;

	glutPostRedisplay();
}

GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat mat_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat mat_specular[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_shine[] = {30};

// Draws the gyroscope
void drawGyroscope() {

	glPushMatrix();

	glPopMatrix();
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shine);

	// center sphere
	glutSolidSphere(.25, 64 , 64);

	// torus1 
	glRotatef(rot, 0.0, 1.0, 0.0);
	glScalef (1.0, 1.0, 1.0);
	//gluCylinder(gluNewQuadric(), 1.0, 1.0, 0.1, 64, 8); 
	gluDisk(gluNewQuadric(), 0.9, 1.0, 64, 16);
	glutSolidTorus(0.05, 1.5, 16, 64);

	// torus2
	
	glRotatef(rot, 1.0, 0.0, 0.0);
	glScalef (1.1, 1.1, 1.1);
	//gluCylinder(gluNewQuadric(), 1.0, 1.0, 0.1, 64, 8); 
	gluDisk(gluNewQuadric(), 0.9, 1.0, 64, 16);
	glutSolidTorus(0.05, 1.5, 16, 64);
	
	// torus3 

	glRotatef(rot, 0.0, 1.0, 0.0);
	glScalef (1.1, 1.1, 1.1);
	//gluCylinder(gluNewQuadric(), 1.0, 1.0, 0.1, 64, 8); 
	gluDisk(gluNewQuadric(), 0.9, 1.0, 64, 16);
	glutSolidTorus(0.05, 1.5, 16, 64);

	glPopMatrix();
}

GLfloat floorAmbient[] = { 1.0, 1.0, 1.0, 1.0 };

// Draws the base of the scene
void drawBase() {
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, floorAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shine);

	glPushMatrix();
	glTranslatef(0.0, -1.9, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluDisk(gluNewQuadric(), 0.0, 4.0, 64, 32); 
	glPopMatrix();
}

// lighting parameters
GLfloat position1[] = { 2.0, -2.0, 2.0, 1.0 };
GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };

GLfloat position2[] = { -2.0, 2.0, 2.0, 1.0 };
GLfloat diffuse[] = { 0.7, 0.7, 0.3, 1.0 };
GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat position3[] = { 0.0, 5.0, 0.0 };
GLfloat down[] = { 0.0, -1.0, 0.0 };

// Sets the lighting for draw
void lighting() {
    glLightfv(GL_LIGHT0, GL_POSITION, position1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    glLightfv(GL_LIGHT1, GL_POSITION, position2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

    glLightfv(GL_LIGHT2, GL_POSITION, position2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular);

    glLightfv(GL_LIGHT3, GL_POSITION, position3);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, down);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);
}

// Draws the graphics
void Draw() {

	glPushMatrix();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

	glColor3f (1.0, 1.0, 1.0);

	// view transform
	curCam->CreateLookAt();

	// lighting
	lighting();

	// world transforms
	drawBase();
	drawGyroscope();

	glFlush();
	glutSwapBuffers();

	glPopMatrix();
}

// Handles keyboard input
void keyboard(unsigned char key, int x, int y) {
	if(key == 113) { // q, quit
		exit(0);
	} else if(key == 99) { // c, switches camera
		curCam = curCam->ID == 0 ? &cam2 : &cam1;
	} else if(key == 97) { // a, speed up
		rotSpeed += ROT_ACCEL;
	} else if(key == 122) { // z, slow down
		rotSpeed -= ROT_ACCEL;
	} else if(key == 49) { // 1, toggle light 0
		light0 ? glDisable(GL_LIGHT0) : glEnable(GL_LIGHT0);
		light0 = !light0;
	} else if(key == 50) { // 2, toggle light 1
		light1 ? glDisable(GL_LIGHT1) : glEnable(GL_LIGHT1);
		light1 = !light1;
	} else if(key == 51) { // 3, toggle light 2
		light2 ? glDisable(GL_LIGHT2) : glEnable(GL_LIGHT2);
		light2 = !light2;
	} else if(key == 52) { // 4, toggle light 3
		light3 ? glDisable(GL_LIGHT3) : glEnable(GL_LIGHT3);
		light3 = !light3;
	}
}

// Handles window resizing
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(54.0, 1.0, 0.01, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

// Free up allocated memory
void Unload() {
	delete curCam;
}

// Entry point
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
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	Unload();

	return 0;
}

