// Two-Dimensional Simple Triangle
// Written by dmc 
// last-modified: Wed Sep 19 11:20:44 2012
//
// Designed to demonstrate smooth shading of vertex colors 
// by using a varying parameter in the vertex shader

#include <iostream>
#include <Angel.h>

// How many points are needed to specify the triangle
// Why 6?
const int NumPoints = 6;

// Data storage for our geometry for the triangles
vec3 points[NumPoints];

// This function initializes the particular things for this program
void myinit()
{
  // First points defined:
  points[0]=vec3(  0.0,   0.0, 0.0);
  points[1]=vec3(500.0,   0.0, 0.0);
  points[2]=vec3(  0.0, 500.0, 0.0);
  // Now colors defined:
  points[3]=vec3(1, 0, 0); // red
  points[4]=vec3(1, 1, 0); // yellow
  points[5]=vec3(0, 0, 1); // blue

  // attributes
  glClearColor(0.1, 0.1, 0.1, 0.5); // dark background */
  //glClearColor(0.8, 0.8, 0.8, 0.5); // light grey background
}

// This function initializes the buffers and shaders
void init()
{
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
  GLuint program = InitShader("vshaderTriangle.glsl", "fshaderTriangle.glsl");
  glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  // Initialize the colors for the vertices for the fragment shader.
  GLuint col=  glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(col);

  // 3 points, each is 3 floating point coordinates after which is the
  // color data.
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(3*3*sizeof(float)));

  // Or you could use the following (since the color data is halfway through the vector):
  /*
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)/2));
  */
}  

// The only required callback, it must draw everything when called.
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT);          // clear the window
  glDrawArrays(GL_TRIANGLES, 0, 3);      // draw the triangles
  glFlush();                             // clear buffers
}

int main(int argc, char** argv)
{
  // Standard GLUT initialization
  glutInit(&argc,argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA); // default, not needed
  glutInitWindowSize(600,600);                   // 600 x 600 pixel window
  glutInitWindowPosition(0,0);                   // place window top left on display
  glutCreateWindow("Triangle Display");         // window title

  // Add callbacks here, only required is displayFunc
  glutDisplayFunc(display);                      // display callback
						 // invoked when
						 // window opened 

  // Initialize the "magic" that glues all the code together.
  glewInit();

  myinit();             // set attributes
  init();               // set up shaders and display environment
  glutMainLoop();       // enter event loop

  return(EXIT_SUCCESS); // return successful exit code
}
