// This program illustrates the use of the glut library for
// interfacing with a Window System 

// The program opens a window, clears it to black,
// then animates a box moving around the window using the
// glutTimerFunc callback.

// The program also reacts correctly when the window is
// moved or resized by clearing the new window to black

// Written by dmc 
// Added in code to deal with double buffering issue.
// last-modified: Wed Sep 23 13:11:11 2015
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

// Parameters for the motion of the square (move in a circle)
GLfloat center_x=250.0;
GLfloat center_y=250.0;
GLfloat radius=250.0;
GLfloat angle=0.0;
GLfloat decrease=1.0;

// angle_incr controls the amount of movement per update frame.
GLfloat angle_incr=0.05;

// milliseconds per frame (controls speed of motion)
GLint time_per_frame=16;  // 16msecs = 62.5 fps

// Toggle clearing of screen after each display
bool clear_screen=true;

// Initial location of square
GLfloat square_x=250.0;
GLfloat square_y=0.0;

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

  if (clear_screen) {
    glClear(GL_COLOR_BUFFER_BIT);                   //clear the window
  }
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);  // draw the lines
  glutSwapBuffers();

  // Why would you want to do this drawing stuff twice?
  // Try it and see!
  /*
  if (clear_screen) {
    glClear(GL_COLOR_BUFFER_BIT);                   //clear the window
  }
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);  // draw the lines
  glutSwapBuffers();
  */
}

extern "C" void mouse(int btn, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    switch (btn) {
    // Right button ends program.
    case GLUT_RIGHT_BUTTON:
      exit(EXIT_SUCCESS);
      break;
    // Left buttun resets the radius
    case GLUT_LEFT_BUTTON:
      if (ww<wh){
	radius=ww/2.0;
      } else {
	radius=wh/2.0;
      }
      break;
    case GLUT_MIDDLE_BUTTON:
      clear_screen=!clear_screen;
      break;
    }
  }
}

// rehaping routine called whenever window is resized
// or moved 
extern "C" void myReshape(int w, int h)
{
  // adjust viewport and clear
  glViewport(0,0,w,h);

  // set global size for use by drawing routine
  ww = w;
  wh = h;
  glUniform2f(windowSizeLoc, ww, wh);             // Pass the window
						  // size
  center_x=ww/2.0;
  center_y=wh/2.0;
  if (ww<wh){
    radius=ww/2.0;
  } else {
    radius=wh/2.0;
  }
  
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT);

  glutPostRedisplay();
}

// Auxilliary function to draw a square at location x, y.
void drawSquare(int x, int y)
{
  glUniform2i(offsetLoc, x, y);
}

// Function to move the square
// each time it is called, it updates the angle, and redraws the
// square at the new location.
// Parameterized equation of a cirle is used to determine the location
// of the square.
extern "C" void timerFunc(int value)
{
  square_x = center_x + radius*cos(angle);
  square_y = center_y + radius*sin(angle);
  angle+=angle_incr;
  // What does next line do?
  radius *=decrease;
  drawSquare(square_x, square_y);
  glutPostRedisplay();
  glutTimerFunc(time_per_frame, timerFunc, 0);
}

// Process keystrokes
extern "C" void keys (unsigned char key, int x, int y)
{
  switch (key) {
    // + and - control the speed of the animation
  case '+':
    time_per_frame--;
    if (time_per_frame < 1) {
      time_per_frame=1;
    }
    break;
  case '-':
    time_per_frame++;
    break;
    // f(aster) and s(lower) control how far the square moves per frame
  case 'f':
    // Make it move 10% farther
    angle_incr*=1.1;
    break;
  case 's':
    // Make it move 10% less far
    angle_incr/=1.1;
    break;
  case 'd':
    decrease=0.99;
    break;
  case ' ':
    decrease=1.0;
    break;
  }
}

// This function initializes the particular things for this program
void myinit()
{
  points[0]= vec2( 1,  1);
  points[1]= vec2(-1,  1);
  points[3]= vec2(-1, -1);
  points[2]= vec2( 1, -1);

  // Initialize graphics state
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
  glutCreateWindow("square");

  // attributes
  //  glClearColor(0.8, 0.8, 0.8, 1.0); // light grey background
    glClearColor(0.0, 0.0, 0.0, 1.0); // black background

  // callback routines
  glutReshapeFunc(myReshape);
  glutMouseFunc (mouse);

  // The following function draws to the screen.
  glutTimerFunc(time_per_frame, timerFunc, 0);
  glutKeyboardFunc(keys);

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
  GLuint loc = glGetAttribLocation(program, "vPosition");
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
