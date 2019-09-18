// This program will show how a convex polygon may be
// defined in terms of the normalized affine sum of points.

// Written by dmc 
// Modified to modern OpenGL
// last-modified: Fri Sep 26 14:50:58 2014

#include <Angel.h>
#include <iostream>

using std::cout;
using std::endl;

// globals

int MyState = 0; // what state are we in?

GLint ColorLoc;       // For uniform variable in shader
GLint windowSizeLoc;  // For uniform variable in shader

const int NumPoints = 10000000; // How many points in the interior will we
                             // draw?

vec2 points[NumPoints+6];  // Room for square, line, and NumPoints pts.

GLsizei wh = 500;    // Initial window size (height)
GLsizei ww = 500;    // Initial window size (width)

// Origin of polygon
GLsizei polyo_x=0;
GLsizei polyo_y=0;

#define POLYSIZE 5 // Number of points for defining polygon.

// Points list
size_t current = 0;
GLsizei poly_x[POLYSIZE];
GLsizei poly_y[POLYSIZE];

GLfloat size=3.0; // size for point display.

// Auxilliary function to draw a square at a particular
// point on the window.
void drawSquare(int x, int y)
{
  points[0]=vec2(x+size, y+size);
  points[1]=vec2(x-size, y+size);
  points[3]=vec2(x-size, y-size);
  points[2]=vec2(x+size, y-size);

  // Copy just the changed data to the GPU
  glBufferSubData(GL_ARRAY_BUFFER, 0, 4*sizeof(vec2), points);

  // Draw the square
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// Auxilliary function to draw text at a particular
// point on the window.
void drawText(int x, int y, char *message)
{
  glUniform4f(ColorLoc, 0.0, 1.0, 0.0, 1.0);

  glRasterPos2i(x, y);
  size_t len;
  len = (int) strlen(message);
  for (size_t i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
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

// Does the majority of the work in this program.
extern "C" void mouse(int btn, int state, int x, int y)
{
  if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
    exit(EXIT_SUCCESS);
  } else if (MyState==1) {
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
      if (current < POLYSIZE) {
	poly_x[current] = x;
	poly_y[current] = wh-y;
	++current;

	glUniform4f(ColorLoc, 1.0, 0.0, 0.0, 1.0);

	drawSquare(x, wh-y);
	glFlush();
      }
      // Record polygon points.
    } else if (btn==GLUT_LEFT_BUTTON && state==GLUT_UP) {
      // Upon getting POLYSIZE points, increment state here.
      if (current >= POLYSIZE) {
	++MyState;
      }
    }
  } else if (MyState==3) {
    if (btn==GLUT_LEFT_BUTTON && state==GLUT_UP) {
      glClear(GL_COLOR_BUFFER_BIT);
      current=0;
      char *message=(char *)"Please click on more points for new polygon";
      drawText(100,100,message);
      glFlush();
      MyState=1;
    }
  }
}

extern "C" void idle()
{
  float p_x, p_y;
  // Draw the polygon if state == 2.
  if (MyState == 2) {
    glUniform4f(ColorLoc, 0.0, 0.5, 1.0, 1.0);// Set color to blue.
    for (size_t i = 0; i < NumPoints; ++i) {
      p_x = 0.0;
      p_y = 0.0;
      
      float alpha[POLYSIZE];
      float sum=0.0;
      // First generate POLYSIZE random variables
      for (size_t j=0; j< POLYSIZE; ++j) {
	alpha[j]=drand48();
	sum+=alpha[j];
      }
      // Then normalize them so they sum to 1.0
      for (size_t j=0; j< POLYSIZE; ++j) {
	alpha[j]/=sum;
      }
      // Then scale the points by the random variables.
      for (size_t j=0; j< POLYSIZE; ++j) {
	p_x += alpha[j] * poly_x[j];
	p_y += alpha[j] * poly_y[j];
      }
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
    {
      char *message=(char *)"Please click on points for polygon";
      drawText(100,100,message);
    }
    ++MyState;
    break;
  case 1:
  case 2:
  case 3:
    // Do nothing for now
    break;
  default:
    cout << "Unknown MyState = "<< MyState << endl;
    //    exit(1);
  }
  glFlush();
}

void myinit(void)
{
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow("polygon definition");

  // Pick 2D clipping window to match size of screen window 
  // This choice avoids having to scale object coordinates
  // each time window is resized
  glViewport(0,0,ww,wh);

  // set clear color to black and clear window
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_COLOR_LOGIC_OP);
  glFlush();

  // callback routine for reshape event
  glutReshapeFunc(myReshape);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);
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
