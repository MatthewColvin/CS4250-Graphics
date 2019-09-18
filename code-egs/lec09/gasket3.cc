// sierpinski gasket using vertex buffer objects (using points again)
//
// This code is to demonstrate a different shader, one that colors the points
// 
// Modified by David M. Chelberg
// Cleaned up code, added comments
// last-modified: Tue Sep 18 13:04:31 2012

#include "Angel.h"

const int NumPoints = 20000; // Number of points to draw.

//----------------------------------------------------------------------------

void init()
{
  // NOTE: Should put the point generating code in a separate function!

  vec3 points[NumPoints];

  // DMC NOTE: there is no reason for the fourth point here!
  // Specifiy the vertices for a tetrahedron
  //
  vec3 vertices[] = {
    vec3(-0.5, -0.5, -0.5),
    vec3( 0.5, -0.5, -0.5),
    vec3( 0.0,  0.5,  0.0),
    vec3( 0.0, -0.5,  0.5)
  };
  //

  // DMC - What happens if we use the following instead?

  /*
    vec3 vertices[] = {
    vec3(-0.3, -0.5, -0.5),
    vec3( 0.5, -0.7, -0.5),
    vec3( 0.0,  0.8,  0.0),
    };
  */

  // Select an arbitrary initial point inside of the triangle
  points[0] = vec3(0.0, 0.0, 0.0);

  // compute and store N-1 new points
  for (int i = 1; i < NumPoints; ++i) {
    int j = rand() % 3;   // pick a vertex at random

    // Compute the point halfway between the selected vertex
    //   and the previous point
    points[i] = (points[i - 1] + vertices[j]) / 2.0;
  }


  // Here is code to deal with getting the data to the GPU

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader23.glsl", "fshader23.glsl");
  glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

//----------------------------------------------------------------------------

extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT);     // clear the window
  glDrawArrays(GL_POINTS, 0, NumPoints);    // draw the points
  glFlush();
}

//----------------------------------------------------------------------------

extern "C" void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 033:
    exit(EXIT_SUCCESS);
    break;
  }
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Simple GLSL example");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
