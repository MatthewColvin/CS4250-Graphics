// Two-Dimensional Simple Lines
// Written by dmc 
// last-modified: Mon Sep 17 10:56:22 2012
// 

#include <iostream>
#include <Angel.h>

// How many points are needed to specify the lines 
// Three lines == 6 points
const int NumPoints = 6;

// Data storage for our geometry for the lines
vec2 points[NumPoints];

// This function initializes the particular things for this program
void myinit()
{
  // First line  
  points[0]= vec2(  0.0,   0.0);
  points[1]= vec2(500.0, 500.0);
  // Second line
  points[2]= vec2(500.0, 500.0);
  points[3]= vec2(  0.0, 500.0);
  // Third line
  points[4]= vec2(0.0, 500.0);
  points[5]= vec2(0.0,   0.0);

  // attributes
  glClearColor(0.8, 0.8, 0.8, 0.5); // light grey background

}

// This function initializes the buffers and shaders
void init()
{
  // Create a vertex array object on the GPU
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
    
  // Create and initialize a buffer object for transferring data to
  // the GPU.
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader21.glsl", "fshader21.glsl");
  //  GLuint program = InitShader("vshaderSimple.glsl", "fshaderSimple.glsl");

  // InitShader does the glUseProgram
  // So the following would be redundant
  // glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}  

// The only required callback, it must draw everything when called.
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT);          //clear the window
  glDrawArrays(GL_LINES, 0, NumPoints);  // draw the lines
  glFlush();                             // clear buffers
}

int main(int argc, char** argv)
{
  // Standard GLUT initialization
  glutInit(&argc,argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA); // default, not needed
  glutInitWindowSize(600,600);                   // 600 x 600 pixel window
  glutInitWindowPosition(0,0);                   // place window top left on display
  glutCreateWindow("Lines Display");             // window title

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
