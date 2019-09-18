#include "common.h"

GLfloat mvx=0.0;
GLfloat mvz=0.0;

//----------------------------------------------------------------------------
extern "C" void special(int key, int x, int y)
{
  switch(key) {
  case GLUT_KEY_UP:
    mvz+=0.1;
    break;
  case GLUT_KEY_DOWN:
    mvz-=0.1;
    break;
  case GLUT_KEY_LEFT:
    mvx-=0.1;
    break;
  case GLUT_KEY_RIGHT:
    mvx+=0.1;
    break;
  }
}
