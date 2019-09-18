// Two-Dimensional Simple Lines
// Written by dmc 
// last-modified: Mon Sep 17 10:37:52 2018
// Added use of vectors, and a random number of random lines.
//

#include <iostream>
#include <Angel.h>
#include <vector>
#include <random>
using std::vector;
using std::cerr;
using std::endl;

// How many points are needed to specify the lines 
// Three lines == 6 points
size_t NumPoints;

// Data storage for our geometry for the lines
vector<vec2> points;

GLint loc1; // Storage for where colors are on the graphics card.

// This function initializes the particular things for this program
void myinit()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> distribution(0.0,500.0);
  // First determine how many lines to draw
  size_t NumLines=distribution(mt);
  NumPoints = 2*NumLines;
  for (size_t i=0; i< NumPoints;++i) {
    points.push_back(vec2(distribution(mt), distribution(mt)));
  }

  // attributes
  //  glClearColor(0.8, 0.8, 0.8, 0.5); // light grey background

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
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec2)*points.size(),
	       &points[0], GL_STATIC_DRAW);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshaderRandom.glsl", "fshaderRandom.glsl");

  // InitShader does the glUseProgram
  // So the following would be redundant
  // glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  loc1 = glGetUniformLocation(program, "vColor");
  if (loc1==-1) {
    cerr << "Can't find vColor in shader" << endl;
    exit(EXIT_FAILURE);
  }
  

}  

// The only required callback, it must draw everything when called.
extern "C" void display()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> color(0.0,1.0);

  glClear(GL_COLOR_BUFFER_BIT);          //clear the window
  for (size_t i=0; i< NumPoints; i+=2) {
    glUniform4f(loc1, color(mt), color(mt), color(mt), 1.0);
    glDrawArrays(GL_LINES, i, 2);  // draw a line
  }
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
