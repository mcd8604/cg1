// 2DDrawing.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>

#include "GL/glut.h"

using namespace std;

#define RES_WIDTH 800.0
#define RES_HEIGHT 600.0

#define NEXT_SCREEN -1
#define PREV_SCREEN -2

//Polygon stipple pattern
const GLubyte pattern[] = {
  0xFF, 0x00, 0x00, 0x77, 0x77, 0x00, 0x00, 0xFF,
  0xFF, 0x00, 0x00, 0x77, 0x77, 0x00, 0x00, 0xFF,
  0x00, 0xFF, 0x00, 0x77, 0x77, 0x00, 0xFF, 0x00, 
  0x00, 0xFF, 0x00, 0x77, 0x77, 0x00, 0xFF, 0x00,
  0x00, 0x00, 0xFF, 0x77, 0x77, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0xFF, 0x77, 0x77, 0xFF, 0x00, 0x00, 
  0x00, 0x00, 0xAA, 0xFF, 0xFF, 0xAA, 0x00, 0x00,
  0xAA, 0xAA, 0xAA, 0xFF, 0xFF, 0xAA, 0xAA, 0xAA,
  0xAA, 0xAA, 0xAA, 0xFF, 0xFF, 0xAA, 0xAA, 0xAA, 
  0x00, 0x00, 0xAA, 0xFF, 0xFF, 0xAA, 0x00, 0x00,
  0x00, 0x00, 0xFF, 0x77, 0x77, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0xFF, 0x77, 0x77, 0xFF, 0x00, 0x00, 
  0x00, 0xFF, 0x00, 0x77, 0x77, 0x00, 0xFF, 0x00,
  0x00, 0xFF, 0x00, 0x77, 0x77, 0x00, 0xFF, 0x00,
  0xFF, 0x00, 0x00, 0x77, 0x77, 0x00, 0x00, 0xFF,
  0xFF, 0x00, 0x00, 0x77, 0x77, 0x00, 0x00, 0xFF
};

int mode;

struct Point {
	int x;
	int y;
};

struct PointList {
	vector<Point> points;
};

struct Screen {
	vector<PointList> pointLists;
};

vector<Screen> screens;

unsigned int curScreen;

void createNewPointList() {
	screens[curScreen].pointLists.push_back(*new PointList());
}

void createNewScreen() {
	Screen *screen = new Screen();
	screens.push_back(*screen);
	delete screen;
	createNewPointList();
}

Point* mousePos;
int mouseLeftState;

void nextScreen() {
	++curScreen;
	if(curScreen >= screens.size()) {
		createNewScreen();
	}
}

void prevScreen() {
	if(curScreen > 0) {
		--curScreen;
	}
}

void MenuHandler(int value) {
	if(value == NEXT_SCREEN) {
		nextScreen();
	} else if(value == PREV_SCREEN) {
		prevScreen();
	} else {
		mode = value;
		if(screens[curScreen].pointLists.back().points.size() > 0) {
			createNewPointList();
		}
	}
}

void Initialize(void) {

	//Init GL
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, RES_WIDTH, 0.0, RES_HEIGHT);
	
	//Init menu
	glutCreateMenu(MenuHandler);
	glutAddMenuEntry("Line", GL_LINES);
	glutAddMenuEntry("Line Strip", GL_LINE_STRIP);
	glutAddMenuEntry("Line Loop", GL_LINE_LOOP);
	glutAddMenuEntry("Triangle", GL_TRIANGLES);
	glutAddMenuEntry("Triangle Strip", GL_TRIANGLE_STRIP);
	glutAddMenuEntry("Triangle Fan", GL_TRIANGLE_FAN);
	glutAddMenuEntry("Quad", GL_QUADS);
	glutAddMenuEntry("Quad Strip", GL_QUAD_STRIP);
	glutAddMenuEntry("Polygon", GL_POLYGON);
	glutAddMenuEntry("Next Screen", NEXT_SCREEN);
	glutAddMenuEntry("Previous Screen", PREV_SCREEN);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Init mode and screen
	mode = GL_LINES;
	createNewScreen();

	mousePos = new Point();
	mousePos -> x = 0;
	mousePos -> y = 0;
}

void Unload() {
	delete mousePos;
}

void KeyUpdate(unsigned char key, int x, int y) {
	
}

void MouseUpdate(int button, int state, int x, int y) {
	
	//detect left mouse click and push a new point
	if(button == GLUT_LEFT_BUTTON) {
		mouseLeftState = state;

		Point* pt = new Point();
		pt -> x = x;
		pt -> y = RES_HEIGHT - y;

		if(state == GLUT_DOWN) {
			screens[curScreen].pointLists.back().points.push_back(*pt);
		}

		delete pt;

		glutPostRedisplay();
	}
}

void MotionUpdate(int x, int y)
{
	mousePos -> x = x;
	mousePos -> y = RES_HEIGHT - y;

	glutPostRedisplay();
}

void PassiveMotionUpdate(int x, int y)
{
	mousePos -> x = x;
	mousePos -> y = RES_HEIGHT - y;

	glutPostRedisplay();
}

void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	for(unsigned int i = 0; i < screens[curScreen].pointLists.size(); ++i) { 
	
		if( i % 2 == 0 )  {
			glEnable( GL_LINE_STIPPLE );
			glLineStipple( 4, 0xCCCC );
		} else {
			glEnable( GL_POLYGON_STIPPLE );
			glPolygonStipple( pattern ); 
		}

		glColor3f((1.0 / screens[curScreen].pointLists.size()) * i, 1 - (1.0 / screens[curScreen].pointLists.size()) * i, 0.0);
		glLineWidth(i * 1.0);

		glBegin(mode);
		for(unsigned int k = 0; k < screens[curScreen].pointLists[i].points.size(); ++k) {
			glVertex2i(screens[curScreen].pointLists[i].points[k].x, screens[curScreen].pointLists[i].points[k].y);
		}
		if(i == screens[curScreen].pointLists.size() - 1) {
			glVertex2i(mousePos -> x, mousePos -> y);
		}
		glEnd();

		
		if( i % 2 == 0 )  {
			glDisable( GL_LINE_STIPPLE );
		} else {
			glDisable( GL_POLYGON_STIPPLE );
		}
	}

	glFlush();
	glutSwapBuffers();
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
	glutMouseFunc(MouseUpdate);
	glutMotionFunc(MotionUpdate);
	glutPassiveMotionFunc(PassiveMotionUpdate);
	glutMainLoop();

	Unload();

	return 0;
}

