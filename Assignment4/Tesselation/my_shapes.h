#ifndef _MY_SHAPES_H_
#define _MY_SHAPES_H_

#include "shape.h"

class MyShapes {
public:
  MyShapes();
  ~MyShapes();
  void draw();
  void tessellate(int, int, int);
private:

  Shape* current;
};

#endif
