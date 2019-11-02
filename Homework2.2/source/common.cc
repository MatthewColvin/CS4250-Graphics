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



//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
extern "C" void reshape(int width, int height)
{
  glViewport(0, 0, width, height);

  aspect = GLfloat(width)/height;
}


//----------------------------------------------------------------------------

// Simple animation
extern "C" void idle()
{
  // Code to animate cube goes here.
  if (rotatep) {
    angle+=1;
  }
  glutPostRedisplay();
}

