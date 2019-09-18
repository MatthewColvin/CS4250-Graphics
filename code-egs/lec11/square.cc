// This program illustrates the use of the glut library for
// interfacing with a Window System 

// The program opens a window, clears it to black,
// then draws a box at the location of the mouse each time the
// left button is clicked and moved. The right button exits the program

// The program also reacts correctly when the window is
// moved or resized by clearing the new window to black

// Written by dmc 
// last-modified: Mon Oct  1 13:02:21 2012
// 

#include <iostream>
#include <Angel.h>

// How many points are needed to specify the square
const int NumPoints = 4;

// Data storage for our geometry for the lines
vec2 points[NumPoints];

// How big a square to draw.
GLfloat size=30.0;

// initial window size
GLfloat wh = 500;
GLfloat ww = 500;

// Locations of variables in shaders.
// Offset of square
GLint offsetLoc;
// Size of square
GLint sizeLoc;
// Window Size 
GLint windowSizeLoc;

// The only required callback, it must draw everything when called.
extern "C" void display()
{
  // What happens if you comment out the next line?  Explain.
  glClear(GL_COLOR_BUFFER_BIT);                   //clear the window
  glUniform1f(sizeLoc, size);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);  // draw the square

  // Draw another square
  /*
  glUniform2i(offsetLoc, 200, 100);
  glUniform1f(sizeLoc, 100);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);  // draw the square
  */
  glFlush();                                      // clear buffers
}

// Right button ends program.
extern "C" void mouse(int btn, int state, int x, int y)
{
  if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
    exit(EXIT_SUCCESS);
  }
}

// rehaping routine called whenever window is resized
// or moved 
extern "C" void myReshape(int w, int h)
{
  // adjust viewport and clear
  glViewport(0,0,w,h);

  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // set global size for use by drawing routine
  ww = w;
  wh = h;
  glUniform2f(windowSizeLoc, ww, wh);             // Pass the window
						  // size
  glutPostRedisplay();
}

// Auxilliary function to draw a square at location x, y.
void drawSquare(int x, int y)
{
  glUniform2i(offsetLoc, x, y);
}

extern "C" void motionFunc(int x, int y)
{
  // Flip y coordinate.
  y=wh-y;
  drawSquare(x, y);
  glutPostRedisplay();
}

// This function initializes the particular things for this program
void myinit()
{
  points[0]= vec2( 1,  1);
  points[1]= vec2(-1,  1);
  points[3]= vec2(-1, -1);
  points[2]= vec2( 1, -1);


  // Initialize graphics state
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutCreateWindow("square");

  // attributes
  glClearColor(0.8, 0.8, 0.8, 0.5); // light grey background

  // callback routines
  glutReshapeFunc(myReshape);
  glutMouseFunc (mouse);

  // The following function draws to the screen.
  glutMotionFunc(motionFunc);

  glutDisplayFunc(display);
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
  GLuint program = InitShader("vshaderSquare.glsl", "fshaderSquare.glsl");


  // InitShader does the glUseProgram
  // So the following would be redundant
  glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  windowSizeLoc = glGetUniformLocation(program, "windowSize");
  if (windowSizeLoc==-1) {
    std::cerr << "Unable to find windowSize parameter" << std::endl;
  }
  offsetLoc  = glGetUniformLocation(program, "offset");
  if (offsetLoc==-1) {
    std::cerr << "Unable to find offsetLoc parameter" << std::endl;
  }
  sizeLoc  = glGetUniformLocation(program, "size");
  if (sizeLoc==-1) {
    std::cerr << "Unable to find sizeLoc parameter" << std::endl;
  }
  GLint loc = glGetAttribLocation(program, "vPosition");
  if (loc==-1) {
    std::cerr << "Unable to find vPosition parameter" << std::endl;
  }
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glUniform2f(offsetLoc, 0.0, 0.0);               // Initial offset
						  // for mouse loc.

  glUniform1f(sizeLoc, size);                     // Pass the current
						  // size of the
						  // square
}


int main(int argc, char** argv)
{
  glutInit(&argc,argv);

  myinit();             // set window attributes, and initial data

  // Initialize the "magic" that glues all the code together.
  glewInit();

  init();               // set up shaders and display environment
  glutMainLoop();       // enter event loop

  return(EXIT_SUCCESS); // return successful exit code
}
