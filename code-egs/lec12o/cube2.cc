// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.
// 
// This version passes three angles to the vertex shader which
// calculates the rotation matrix and applies it to the incoming
// points.
//
// Adapted from an Angel example, by David M. Chelberg
// last-modified: Wed Oct  2 13:10:10 2013
//

#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
  point4(-0.5, -0.5,  0.5, 1.0),
  point4(-0.5,  0.5,  0.5, 1.0),
  point4( 0.5,  0.5,  0.5, 1.0),
  point4( 0.5, -0.5,  0.5, 1.0),
  point4(-0.5, -0.5, -0.5, 1.0),
  point4(-0.5,  0.5, -0.5, 1.0),
  point4( 0.5,  0.5, -0.5, 1.0),
  point4( 0.5, -0.5, -0.5, 1.0)
};

// RGBA olors
color4 vertex_colors[8] = {
  color4(0.0, 0.0, 0.0, 1.0),  // black
  color4(1.0, 0.0, 0.0, 1.0),  // red
  color4(1.0, 1.0, 0.0, 1.0),  // yellow
  color4(0.0, 1.0, 0.0, 1.0),  // green
  color4(0.0, 0.0, 1.0, 1.0),  // blue
  color4(1.0, 0.0, 1.0, 1.0),  // magenta
  color4(1.0, 1.0, 1.0, 1.0),  // white
  color4(0.0, 1.0, 1.0, 1.0)   // cyan
};

// Array of rotation angles (in degrees) for each coordinate axis
enum {Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = {0.0, 0.0, 0.0};

GLuint  thetaLoc;  // The location of the "theta" shader uniform variable

// Adjust this value for your machine (to speed up, make bigger, to
// slow down rotation, make smaller
GLfloat incr =0.06;

//----------------------------------------------------------------------------
// MyQuad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void MyQuad(int a, int b, int c, int d)
{
  colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
  colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
  colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
  colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
  colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
  colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void colorcube()
{
  MyQuad(1, 0, 3, 2);
  MyQuad(2, 3, 7, 6);
  MyQuad(3, 0, 4, 7);
  MyQuad(6, 5, 1, 2);
  MyQuad(4, 5, 6, 7);
  MyQuad(5, 4, 0, 1);
}

//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{
  colorcube();

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
	       NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader36.glsl", "fshader36.glsl");
  glUseProgram(program);

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  GLuint vColor = glGetAttribLocation(program, "vColor"); 
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)));

  thetaLoc = glGetUniformLocation(program, "theta");
    
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  glUniform3fv(thetaLoc, 1, Theta);
  glDrawArrays(GL_TRIANGLES, 0, NumVertices);

  glutSwapBuffers();
}

//----------------------------------------------------------------------------
extern "C" void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case 033: // Escape Key
  case 'q': case 'Q':{
    exit(EXIT_SUCCESS);
    break;
  }
  // Added code to allow for speeding up/slowing down speed of
  // rotation. -- DMC
  case '+':
    incr *= 2.0;
    break;
  case '-':
    incr /= 2.0;
    break;
  }
}

//----------------------------------------------------------------------------
extern "C" void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    switch(button) {
    case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
    case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
    case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
    }
  }
}

//----------------------------------------------------------------------------
extern "C" void idle()
{
  static GLint time=glutGet(GLUT_ELAPSED_TIME);
  Theta[Axis] += incr*(glutGet(GLUT_ELAPSED_TIME)-time);
  time = glutGet(GLUT_ELAPSED_TIME);

  if (Theta[Axis] > 360.0) {
    Theta[Axis] -= 360.0;
  }
    
  glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Color Cube");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);

  glutMainLoop();
  return EXIT_SUCCESS;
}
