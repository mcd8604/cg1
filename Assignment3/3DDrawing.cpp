
#include "GL/glut.h"

#define RES_WIDTH 800.0
#define RES_HEIGHT 600.0

void Initialize() {

}

void Draw() {

}

int _tmain(int argc, char** argv)
{
	glEnable(GL_DOUBLEBUFFER);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(RES_WIDTH,RES_HEIGHT);
	glutCreateWindow("3D Drawing");

	Initialize();
	
	glutDisplayFunc(Draw);
	glutMainLoop();

	return 0;
}