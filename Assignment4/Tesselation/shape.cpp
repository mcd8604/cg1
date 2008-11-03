#include "stdafx.h"

#include <windows.h>

#include "shape.h"
#include "draw_routines.h"
#include <fstream>

// you must write this file

const double PI = 3.14159265359;

Shape::~Shape() {
}

void Shape::draw() {
  for (unsigned int i = 0; i < vertices.size(); i += 3)
    DrawRoutines::drawTriangle(vertices[i], vertices[i + 1], vertices[i + 2]);
}

Shape::Shape() : vertices() {
}

void Shape::addTriangle(const Point3& p1, const Point3& p2, const Point3& p3) {
  vertices.push_back(p1);
  vertices.push_back(p2);
  vertices.push_back(p3);
}

Cube::Cube(int n) : Shape() {

	// Your code for tessellating a cube goes here
	
	// length of each section
	double l = 1.0 / n;
	double u1, u2, v1, v2;
	Point3 *p1, *p2, *p3, *p4;

	for(int y = 0; y < n; ++y)  {
		v1 = l * y - 0.5;
		v2 = l * (y + 1) - 0.5;
		for(int x = 0; x < n; ++x) {
			u1 = l * x - 0.5;
			u2 = l * (x + 1) - 0.5;

			// -z face
			p1 = new Point3(u1, v1, -0.5);
			p2 = new Point3(u2, v1, -0.5);
			p3 = new Point3(u2, v2, -0.5);
			p4 = new Point3(u1, v2, -0.5);
			addTriangle(*p3, *p2, *p1);
			addTriangle(*p4, *p3, *p1);
			delete p1;
			delete p2;
			delete p3;
			delete p4;

			// +z face
			p1 = new Point3(u1, v1, 0.5);
			p2 = new Point3(u2, v1, 0.5);
			p3 = new Point3(u2, v2, 0.5);
			p4 = new Point3(u1, v2, 0.5);
			addTriangle(*p1, *p2, *p3);
			addTriangle(*p1, *p3, *p4);
			delete p1;
			delete p2;
			delete p3;
			delete p4;

			// -y face
			p1 = new Point3(u1, -0.5, v1);
			p2 = new Point3(u2, -0.5, v1);
			p3 = new Point3(u2, -0.5, v2);
			p4 = new Point3(u1, -0.5, v2);
			addTriangle(*p1, *p2, *p3);
			addTriangle(*p1, *p3, *p4);
			delete p1;
			delete p2;
			delete p3;
			delete p4;

			// +y face
			p1 = new Point3(u1, 0.5, v1);
			p2 = new Point3(u2, 0.5, v1);
			p3 = new Point3(u2, 0.5, v2);
			p4 = new Point3(u1, 0.5, v2);
			addTriangle(*p3, *p2, *p1);
			addTriangle(*p4, *p3, *p1);
			delete p1;
			delete p2;
			delete p3;
			delete p4;

			// -x face
			p1 = new Point3(-0.5, v1, u1);
			p2 = new Point3(-0.5, v1, u2);
			p3 = new Point3(-0.5, v2, u2);
			p4 = new Point3(-0.5, v2, u1);
			addTriangle(*p1, *p2, *p3);
			addTriangle(*p1, *p3, *p4);
			delete p1;
			delete p2;
			delete p3;
			delete p4;

			// +x face
			p1 = new Point3(0.5, v1, u1);
			p2 = new Point3(0.5, v1, u2);
			p3 = new Point3(0.5, v2, u2);
			p4 = new Point3(0.5, v2, u1);
			addTriangle(*p3, *p2, *p1);
			addTriangle(*p4, *p3, *p1);
			delete p1;
			delete p2;
			delete p3;
			delete p4;
		}
	}
}

Cone::Cone(int n, int m) : Shape() {
	if (n < 3)
		n = 3;

	if (m < 1)
		m = 1;
  
	// Your code for tessellating a cone goes here  
	double theta = (2 * PI) / n;

	Point3 *pTopOrgn, *pBotOrgn, *p1Top, *p2Top, *p1Bot, *p2Bot;

	// origins
	pTopOrgn = new Point3(0, 0.5, 0);
	pBotOrgn = new Point3(0, -0.5, 0);

	for(int i = 0; i < n; ++i) {
		p1Bot = new Point3(cos(theta * i) * 0.5, -0.5, sin(theta * i) * 0.5);
		p2Bot = new Point3(cos(theta * ((i + 1) % n)) * 0.5, -0.5, sin(theta * ((i + 1) % n)) * 0.5);

		// bottom
		addTriangle(*pBotOrgn, *p1Bot, *p2Bot);
	
		// free allocated memory
		delete p1Bot;
		delete p2Bot;
	}

	// free allocated memory
	delete pTopOrgn;
	delete pBotOrgn;
}

Cylinder::Cylinder(int n, int m) : Shape() {
	if (n < 3)
		n = 3;

	if (m < 1)
		m = 1;
  
	// Your code for tessellating a cylinder goes here
	double theta = (2 * PI) / n;

	Point3 *pTopOrgn, *pBotOrgn, *p1Top, *p2Top, *p1Bot, *p2Bot;

	// origins
	pTopOrgn = new Point3(0, 0.5, 0);
	pBotOrgn = new Point3(0, -0.5, 0);

	for(int i = 0; i < n; ++i) {
		p1Top = new Point3(cos(theta * i) * 0.5, 0.5, sin(theta * i) * 0.5);
		p2Top = new Point3(cos(theta * ((i + 1) % n)) * 0.5, 0.5, sin(theta * ((i + 1) % n)) * 0.5);

		p1Bot = new Point3(p1Top -> x, -0.5, p1Top -> z);
		p2Bot = new Point3(p2Top -> x, -0.5, p2Top -> z);

		// top
		addTriangle(*p2Top, *p1Top, *pTopOrgn);
		// bottom
		addTriangle(*pBotOrgn, *p1Bot, *p2Bot);
		
		// sides, start at bottom, increment upwards
		double deltaY = 1.0 / m;
		for(int j = 0; j < m; ++j) {
			p1Top -> y = (j + 1) * deltaY - 0.5;
			p2Top -> y = p1Top -> y;
			addTriangle(*p2Bot, *p1Bot, *p2Top);
			addTriangle(*p2Top, *p1Bot, *p1Top);
			p1Bot -> y += deltaY;
			p2Bot -> y = p1Bot -> y;
		}

		// free allocated memory
		delete p1Top;
		delete p2Top;
		delete p1Bot;
		delete p2Bot;
	}

	// free allocated memory
	delete pTopOrgn;
	delete pBotOrgn;
}


Sphere::Sphere(int n) : Shape() {

	//your code for tessellating a sphere goes here


	// Note:
	// If you decide to solve this problem by recursively subdividing a regular platonic solid, you may use 
	// the geometric description of an icosahredron mesh provided in the lecture notes as a starting point for the
	// subdivision.
	//
	// You should divide each triangle into 4 smaller triangles as the parameter 'n' increases.
	// There is no need to go beyond 5 levels of subdivision - i.e. if n > 5 then set n = 5

}


