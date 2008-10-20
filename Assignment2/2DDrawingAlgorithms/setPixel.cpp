/**
 * setPixel -- openGL routine for setting a pixel
 *
 */

#include <windows.h>
#include <GL/gl.h>

void setPixel (int x, int y)
{
  //glRecti( x, y, x+1, y+1 );
  glRectf( x - 0.5f, y - 0.5f, x + 0.5f, y + 0.5f );
}
