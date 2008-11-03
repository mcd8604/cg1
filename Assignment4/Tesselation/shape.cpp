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

			// +z face
			p1 = new Point3(u1, v1, 0.5);
			p2 = new Point3(u2, v1, 0.5);
			p3 = new Point3(u2, v2, 0.5);
			p4 = new Point3(u1, v2, 0.5);
			addTriangle(*p1, *p2, *p3);
			addTriangle(*p1, *p3, *p4);

			// -y face
			p1 = new Point3(u1, -0.5, v1);
			p2 = new Point3(u2, -0.5, v1);
			p3 = new Point3(u2, -0.5, v2);
			p4 = new Point3(u1, -0.5, v2);
			addTriangle(*p1, *p2, *p3);
			addTriangle(*p1, *p3, *p4);

			// +y face
			p1 = new Point3(u1, 0.5, v1);
			p2 = new Point3(u2, 0.5, v1);
			p3 = new Point3(u2, 0.5, v2);
			p4 = new Point3(u1, 0.5, v2);
			addTriangle(*p3, *p2, *p1);
			addTriangle(*p4, *p3, *p1);

			// -x face
			p1 = new Point3(-0.5, v1, u1);
			p2 = new Point3(-0.5, v1, u2);
			p3 = new Point3(-0.5, v2, u2);
			p4 = new Point3(-0.5, v2, u1);
			addTriangle(*p1, *p2, *p3);
			addTriangle(*p1, *p3, *p4);

			// +x face
			p1 = new Point3(0.5, v1, u1);
			p2 = new Point3(0.5, v1, u2);
			p3 = new Point3(0.5, v2, u2);
			p4 = new Point3(0.5, v2, u1);
			addTriangle(*p3, *p2, *p1);
			addTriangle(*p4, *p3, *p1);
		}
	}
	free(p1);
	free(p2);
	free(p3);
	free(p4);
}

Cone::Cone(int n, int m) : Shape() {
  if (n < 3)
    n = 3;
  
  // Your code for tessellating a cone goes here  
}

Cylinder::Cylinder(int n, int m) : Shape() {
  if (n < 3)
    n = 3;
  
  // Your code for tessellating a cylinder goes here
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


