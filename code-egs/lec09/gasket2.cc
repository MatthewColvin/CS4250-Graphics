// sierpinski gasket with vertex arrays 
//
// Using triangles rather than points
// 
// Modified by David M. Chelberg
// last-modified: Tue Sep 18 12:59:33 2012

#include "Angel.h"

//const int NumTimesToSubdivide = 6;
//const int NumTriangles = 3*3*3*3*3*3;  // 3^6 triangles generated
const int NumTimesToSubdivide = 4;
const int NumTriangles = 3*3*3*3;  // 3^4 triangles generated
const int NumVertices  = 3 * NumTriangles;

vec2 points[NumVertices];
int Index = 0;

//----------------------------------------------------------------------------

void triangle(const vec2& a, const vec2& b, const vec2& c)
{
  points[Index++] = a;
  points[Index++] = b;
  points[Index++] = c;
}

//----------------------------------------------------------------------------

void divide_triangle(const vec2& a, const vec2& b, const vec2& c, int count)
{
  if (count > 0) {
    vec2 v0 = (a + b) / 2.0;
    vec2 v1 = (a + c) / 2.0;
    vec2 v2 = (b + c) / 2.0;
    divide_triangle(a, v0, v1, count - 1);
    divide_triangle(c, v1, v2, count - 1);
    divide_triangle(b, v2, v0, count - 1);
    //divide_triangle(b, v2, v1, count - 1);
  } else {
    triangle(a, b, c);    // draw triangle at end of recursion
  }
}

//----------------------------------------------------------------------------

void init()
{
  vec2 vertices[3] = {vec2(-1.0, -1.0),
		      vec2(0.0, 1.0), 
		      vec2(1.0, -1.0)};

  // Subdivide the original triangle
  divide_triangle(vertices[0], vertices[1], vertices[2], NumTimesToSubdivide);

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
  GLuint program = InitShader("vshader22.glsl", "fshader22.glsl");
  glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader    
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glClearColor(1.0, 1.0, 1.0, 1.0); // white background 
}

//----------------------------------------------------------------------------

extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  glFlush();
}

//----------------------------------------------------------------------------

extern "C" void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  // Escape key
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
  return EXIT_SUCCESS;
}
