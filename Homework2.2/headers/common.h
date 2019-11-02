#ifndef COMMON_H
#define COMMON_H

#include <Angel.h>
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
// Globals to control moving around a scene.
extern GLfloat mvx;
extern GLfloat mvy;
extern GLfloat mvz;

extern "C" void special(int key, int x, int y);

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

extern vector<point4> points;
extern vector<color4> colors;
extern vector<color4> normals;

// Viewing transformation parameters
extern GLfloat radius;
extern GLfloat radiusInit;

extern GLfloat cameraangle;
extern GLfloat theta;
extern GLfloat phi;

extern const GLfloat dr;      // Degrees to radian conversion

extern bool rotatep;          // whether to rotate or not
extern GLfloat angle;         // Angle of cube rotation.

extern GLint  camera_view; // camera-view matrix uniform shader variable location
extern GLint  model_view;  // model-view matrix uniform shader variable location
extern GLint  shade_loc;   // shade uniform shader variable location
extern GLint  projection; // projection matrix uniform shader variable location

// Projection transformation parameters
extern GLfloat left, right;
extern GLfloat bottom, top;
extern GLfloat zNear, zFar;
extern GLfloat zNearInit, zFarInit;

extern GLfloat fovy;
extern GLfloat aspect;

extern int Index;          // Keep track of how many points we generate.

void MyQuad(int a, int b, int c, int d);
void colorcube(vector<vec4> colors);

extern "C" void reshape(int width, int height);
extern "C" void keyboard(unsigned char key, int x, int y);
extern "C" void idle();
void init();

#endif
