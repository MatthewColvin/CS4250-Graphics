// Two-Dimensional Square Box
// Written by dmc 
// last-modified: Mon Sep 17 11:11:24 2012
// 
// The "hello world" of OpenGL

#include <iostream>
#include <Angel.h>

// How many points are needed to specify the lines 
const int NumPoints = 4;

// Data storage for our geometry for the lines
vec3 points[NumPoints];

// This function initializes the particular things for this program
void myinit()
{
  // What happens if we use the following points?
  // First line  
  points[0]= vec3(-0.5, 0.5, 0.3);
  points[1]= vec3(-0.5, -0.5, 0.2);
  points[2]= vec3(0.5, -0.5, 0.3);
  points[3]= vec3(0.5, 0.5, 0.4);

  // attributes
  //  glClearColor(0.8, 0.8, 0.8, 0.5); // light grey background 
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
  GLuint program = InitShader("vshaderSimple.glsl", "fshaderSimple.glsl");
  glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);

  // 3D points now
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0,
    BUFFER_OFFSET(0));
}  

// The only required callback, it must draw everything when called.
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT);          //clear the window
  glDrawArrays(GL_POLYGON, 0, NumPoints);// draw the square
  //  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);// draw the square
  glFlush();                             // clear buffers
}

int main(int argc, char** argv)
{
  // Standard GLUT initialization
  glutInit(&argc,argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA); // default, not needed
  glutInitWindowSize(600,600);                   // 600 x 600 pixel window
  glutInitWindowPosition(0,0);                   // place window top left on display
  glutCreateWindow("Simple Display1");           // window title

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
