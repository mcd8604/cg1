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
	gluPerspective(54.0, 1.0, 1.0, 10.0);

}

void Draw() {

	glClear(GL_COLOR_BUFFER_BIT);

	//Draw stuff
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( 
		0.0, 0.0, 2.0, 
		0.0, 0.0, 0.0, 
		0.0, 1.0, 0.0 );

	glPushMatrix();	/* save copy of “basic” matrix */
	//glTranslatef( 1.0, 2.0, 3.0 ); 
	//glScalef( 2.0, 2.0, 2.0 ); 
	//glRotatef( 45.0, 0.0, 0.0, 1.0 ); 
	//glRotatef( 30.0, 0.0, 1.0, 0.0 );
	
	
	/* code to draw object1 */
	glutWireCube( 1.0 );


	glPopMatrix();	/* restore “basic” matrix */

	glPushMatrix();	/* save again */
	glTranslatef( 10.0, 10.0, 10.0 );

	/* code to draw object2 */

	glPopMatrix();	/* restore again */


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

