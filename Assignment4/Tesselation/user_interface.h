#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

class UserInterface {
public:
  UserInterface() {}
  virtual ~UserInterface() {}

  virtual void resize(int, int) = 0;
  virtual void draw() = 0;
  virtual int handle(int) = 0;
};

#endif
