#include "common.h"

const GLfloat dr = 90.0 * DegreesToRadians;

bool rotatep=false;          // whether to rotate or not


GLfloat theta = 0.0;
GLfloat phi = 0.0;
GLfloat angle = 0.0;
GLfloat cameraangle = 0.0;

GLfloat left = -1.0, right = 1.0;
GLfloat bottom = -1.0, top = 1.0;

// For perspective camera
GLfloat fovy=45;
GLfloat aspect=1.0;

// Camera and view parameters
GLfloat zNearInit = 0.1, zFarInit=300.0;
GLfloat zNear = zNearInit;
GLfloat zFar = zFarInit;
GLfloat radiusInit=15.0;
GLfloat radius=radiusInit;

GLint  camera_view;// camera-view matrix uniform shader variable location
GLint  model_view; // model-view matrix uniform shader variable locatio
GLint  projection; // projection matrix uniform shader variable location
GLint  shade_loc;  // shade uniform shader variable location

int Index = 0;      // Keeps track of which vertex we are on.

// Triangle data for the cube
vector<point4> points;
vector<color4> colors;
vector<color4> normals;

GLfloat mvx=0.0;
GLfloat mvy=0.0;
GLfloat mvz=0.0;

GLfloat incr=0.1;

//----------------------------------------------------------------------------
extern "C" void special(int key, int x, int y)
{
  switch(key) {
  case GLUT_KEY_UP:
    mvz+=incr;
    break;
  case GLUT_KEY_DOWN:
    mvz-=incr;
    break;
  case GLUT_KEY_LEFT:
    mvx+=incr;
    break;
  case GLUT_KEY_RIGHT:
    mvx-=incr;
    break;
  }
}

//----------------------------------------------------------------------------
extern "C" void reshape(int width, int height)
{
  glViewport(0, 0, width, height);

  aspect = GLfloat(width)/height;
}


//----------------------------------------------------------------------------
extern "C" void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case 033: // Escape Key
  case 'q': case 'Q':
    exit(EXIT_SUCCESS);
    break;

    // Speed up/slow down movements
  case '+':
    incr*=2.0;
    break;
  case '-':
    incr/=2.0;
    break;

  case 'x': left *= 1.1; right *= 1.1; break;
  case 'X': left /= 1.1; right /= 1.1; break;
    //  case 'y': bottom *= 1.1; top *= 1.1; break;
    //  case 'Y': bottom /= 1.1; top /= 1.1; break;
  case 'z': zNear  *= 1.1; zFar /= 1.1; break;
  case 'Z': zNear /= 1.1; zFar *= 1.1; break;
  case 'Y':
    mvy+=incr;
    break;
  case 'y':
    mvy-=incr;
    break;

  case 'r': radius *= 1.5; break;
  case 'R': radius /= 1.5; break;

  case 'o': theta += dr; break;
  case 'O': theta -= dr; break;

  case 'p': phi += dr; break;
  case 'P': phi -= dr; break;

  case 'T': cameraangle += dr; break;
  case 't': cameraangle -= dr; break;


  case 'v': 
    fovy-=5; 
    if (fovy < 0) {
      // Min angle of view 1 degree
      fovy = 1;
    }
    break;
  case 'V': fovy+=5; break;
    if (fovy > 179) {
      // Max angle of view 179 degrees
      fovy = 179;
    }
    break;

  case ' ':  // reset values to their defaults
    rotatep=!rotatep;

    incr=0.1;
    left = -1.0;
    right = 1.0;
    bottom = -1.0;
    top = 1.0;
    zNear = zNearInit;
    zFar = zFarInit;
    mvx = 0.0;
    mvy = 0.0;
    mvz = 0.0;

    radius = radiusInit;
    theta  = 0.0;
    phi    = 0.0;
    break;
  }

  glutPostRedisplay();
}

// Simple animation
extern "C" void idle()
{
  // Code to animate cube goes here.
  if (rotatep) {
    angle+=1;
  }
  glutPostRedisplay();
}

