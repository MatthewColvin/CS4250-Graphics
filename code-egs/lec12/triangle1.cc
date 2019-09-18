// This program is designed to show how rubberbanding may be
// used to interactively draw lines.  And to show how a
// triangle may be defined by an origin, and two vectors.

// Written by dmc
// 
// Modified to update to new OpenGL standard
// Added thicker lines and points to be more visible.
// Removed glut text calls, replaced by calls to change the window's title.
// last-modified: Fri Sep 26 14:50:40 2014

#include <Angel.h>
#include <iostream>

using std::cout;
using std::endl;

// globals

int MyState = 0;      // Keeps track of the state are we in?

GLint ColorLoc;       // For uniform variable in shader
GLint windowSizeLoc;  // For uniform variable in shader

const int NumPoints=1000;  // How many interior points to draw.
vec2 points[NumPoints+6];  // Room for square, line, and NumPoints pts.

GLsizei wh = 500;     // Initial window size (height)
GLsizei ww = 500;     // Initial window size (width)

// Origin of triangle
GLsizei tri_x=0;
GLsizei tri_y=0;

// Vector one for triangle
GLsizei triv1_x=0;
GLsizei triv1_y=0;

// Vector two for triangle
GLsizei triv2_x=0;
GLsizei triv2_y=0;

// initial location to rubber band from.
GLsizei initx = 250;
GLsizei inity = 250; 

// last location to draw rubber band to.
GLsizei lastx = 250;
GLsizei lasty = 250; 

// Global for first time initialization of rubberbanding.
bool init = false;

float size=3.0;

// Auxilliary function to draw a square at a particular
// point on the window.
void drawSquare(int x, int y)
{
  // Next line for colored square
  glUniform4f(ColorLoc, 0.0, 1.0, 0.0, 1.0);

  points[0]=vec2(x+size, y+size);
  points[1]=vec2(x-size, y+size);
  points[3]=vec2(x-size, y-size);
  points[2]=vec2(x+size, y-size);

  // Copy just the changed data to the GPU
  glBufferSubData(GL_ARRAY_BUFFER, 0, 4*sizeof(vec2), points);

  // Draw the square
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// Draws a line from the global position initx, inity to x,
// y using drawing mode xor
void drawLine(int x, int y)
{
  glEnable(GL_COLOR_LOGIC_OP);
  glLogicOp(GL_XOR);
  points[4]=vec2(initx, inity);
  points[5]=vec2(x, y);
  // Copy just the changed data to the GPU
  glBufferSubData(GL_ARRAY_BUFFER, 4*sizeof(vec2), 2*sizeof(vec2), &(points[4]));

  // Draw the updated line
  glDrawArrays(GL_LINES, 4, 2);
  glLogicOp(GL_COPY);// Good idea to undo what we did.
  glDisable(GL_COLOR_LOGIC_OP);
  glFlush();
}

// Callback for mouse movement.  In this case, first erase
// the last line drawn, then draw the line to the current
// location of the mouse. 
extern "C" void movement(int x, int y)
{
  y=wh-y;
  // Only draw if the state is correct to be doing
  // rubberbanding. 
  if ((MyState > 1) && (MyState < 5)) {
    // Correct the y coord.
    // Now erase last line if already inited.
    if (init) {
      drawLine(lastx, lasty);
    } else {
      init = true;
    }
    drawLine(x, y); // Now draw the current line.
    lastx =x;
    lasty =y;
  }
}
 
// rehaping routine called whenever window is resized or
// moved 
extern "C" void myReshape(int w, int h)
{
  // adjust viewport and clear
  glViewport(0,0,w,h);

  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
  // set global size for use by drawing routine
  ww = w;
  wh = h;
  glUniform2f(windowSizeLoc, ww, wh);             // Pass the window
}

// This does the majority of work, keeping track of what we
// are doing by means of the global state variable.
extern "C" void mouse(int btn, int state, int x, int y)
{
  // Exit if right button pressed in any state.
  if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
    exit(EXIT_SUCCESS);
  }
  // MyState==1 means we want to record the origin of the
  // triangle we are drawing.
  if (MyState == 1) {
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
      // Record the origin for the triangle.
      tri_x = x;
      tri_y = wh -y;
      glClear(GL_COLOR_BUFFER_BIT);
      glutSetWindowTitle("Draw one vector for the triangle");
      glFlush();
    } else if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP) {
      drawSquare(tri_x, tri_y);
      glFlush();
      ++MyState;// Increment state (now get two vectors)
    } 
  // MyState==2 means we want to record the first vector
  // that will define the triangle we are drawing.
  } else if (MyState==2) {
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
      initx = x;
      inity = wh -y;
      lastx = initx;
      lasty = inity;
      init = true;
    } else if (btn==GLUT_LEFT_BUTTON && state==GLUT_UP) {
      // We've now specified the first vector for the
      // triangle. 
      triv1_x=x - initx; 
      triv1_y=(wh-y)-inity;
      glClear(GL_COLOR_BUFFER_BIT);
      glutSetWindowTitle("Draw second vector for the triangle");
      drawSquare(tri_x, tri_y);
      glFlush();
      ++MyState;
    }
  // MyState==3 means we want to record the second vector
  // that will define the triangle we are drawing.
  } else if (MyState==3) {
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
      initx = x;
      inity = wh -y;
      lastx = initx;
      lasty = inity;
      init = true;
    } else if (btn==GLUT_LEFT_BUTTON && state==GLUT_UP) {
      // We've now specified the second vector for the
      // triangle. 
      triv2_x=x - initx; 
      triv2_y=(wh-y)-inity;
      glClear(GL_COLOR_BUFFER_BIT);
      drawSquare(tri_x, tri_y);
      glFlush();
      ++MyState;
    }
  // MyState==5 means we want to define a new triangle.
  } else if (MyState==5) {
    if (btn==GLUT_LEFT_BUTTON && state==GLUT_UP) {
      glClear(GL_COLOR_BUFFER_BIT);
      glutSetWindowTitle("Please click on origin for new triangle");
      glFlush();
      MyState=1;
    }
  }
}

extern "C" void idle()
{
  int p_x, p_y;
  double alpha, beta;
  // MyState==4 means we want to draw the triangle that is
  // now completely specified by an origin and two vectors.
  if (MyState == 4) {
    for (size_t i = 0; i < NumPoints; ++i) {
      // Generate random points in the interior of the triangle.
      alpha = drand48();
      beta = drand48();
      // This equation comes from the book pg 152.
      p_x = (int)(tri_x + 
		  beta * (1-alpha) * triv1_x + 
		  (1-beta) * triv2_x + 0.5);
      p_y = (int)(tri_y + 
		  beta * (1-alpha) * triv1_y + 
		  (1- beta)* triv2_y + 0.5);
      points[6+i]=vec2(p_x, p_y);
    }

    // Copy just the changed data to the GPU
    glBufferSubData(GL_ARRAY_BUFFER, 6*sizeof(vec2), NumPoints*sizeof(vec2), points);

    // Draw the points
    glDrawArrays(GL_POINTS, 6*sizeof(vec2),NumPoints);
    glFlush();
    ++MyState;
  }
}

// display callback required by GLUT 3.0
extern "C" void display(void)
{
  switch (MyState) {
  case 0:
    // Initial state, ask to input a triangle.
    glutSetWindowTitle("Please click on origin for triangle");
    glFlush();
    ++MyState;
    break;
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
    // Do nothing for now
    break;
  default:
    cout << "Unknown MyState = "<< MyState << endl;
    // Could exit if we get here, or just continue.
    //    exit(1);
  }
}

void myinit(void)
{
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Triangle Definition");

  glViewport(0,0,ww,wh);
  /* Pick 2D clipping window to match size of screen window 
     This choice avoids having to scale object coordinates
     each time window is resized */

  // set clear color to black and clear window
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glLineWidth(3.0);
  glPointSize(2.5);

  // callback routines
  glutReshapeFunc(myReshape);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);
  glutMotionFunc(movement);
  glutDisplayFunc(display);

}

// This function initializes the buffers and shaders
void shaderInit()
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
  GLuint program = InitShader("vshaderTriangle.glsl", "fshaderTriangle.glsl");


  // InitShader does the glUseProgram
  // So the following would be redundant
  // glUseProgram(program);

  // Initialize the window size uniform from the vertex shader
  windowSizeLoc = glGetUniformLocation(program, "windowSize");
  if (windowSizeLoc==-1) {
    std::cerr << "Unable to find windowSize parameter" << std::endl;
  }

  // Initialize the window size uniform from the vertex shader
  ColorLoc = glGetUniformLocation(program, "vColor");
  if (ColorLoc==-1) {
    std::cerr << "Unable to find vColor parameter" << std::endl;
  }

  // Initialize the vertex position attribute from the vertex shader
  GLint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glUniform4f(ColorLoc, 0.0, 1.0, 0.0, 1.0);
}

int main(int argc, char** argv)
{
  glutInit(&argc,argv);
  myinit ();

  // Initialize the "magic" that glues all the code together.
  glewInit();

  shaderInit();

  glutMainLoop();

  return(EXIT_SUCCESS);
}
