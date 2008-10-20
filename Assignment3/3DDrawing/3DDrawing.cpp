// 3DDrawing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "GL/glut.h"

#define RES_WIDTH 800.0
#define RES_HEIGHT 600.0

//Initializes OpenGL and ...
void Initialize() {

	//Init GL
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0, 1.0, 1.0, 10.0);

}

void Draw() {

	glClear(GL_COLOR_BUFFER_BIT);

	//Draw stuff
	

	glFlush();
	glutSwapBuffers();
}

void Unload() {

}

int _tmain(int argc, char** argv)
{
	glEnable(GL_DOUBLEBUFFER);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(RES_WIDTH,RES_HEIGHT);
	glutCreateWindow("2D Drawing");

	Initialize();
	
	glutDisplayFunc(Draw);
	glutMainLoop();

	Unload();

	return 0;
}

