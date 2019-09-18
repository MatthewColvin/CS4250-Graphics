// Test variable point size  -- dmc.
// Added depth buffer size determination, and initial string to ask
// for as much depth precision as possible, etc.
//
// modified to use shaders everywhere
//
// last-modified: Sun Sep 14 18:53:13 2014
// by David M. Chelberg
//

#include <iostream>
#include <Angel.h>

using std::endl;
using std::cout;

// Size of points array, not the number of vertices, as we will have a
// color for each vertex.
const int NumPoints = 8;

// Data storage for our geometry for the triangles
vec3 points[NumPoints];

// This function initializes the particular things for this program
void myinit()
{
  // First points defined:
  points[0]=vec3(250.0,   250.0, 0.0);
  points[1]=vec3( 50.0,    50.0, 0.0);

  // Line defined next
  points[2]=vec3(  50.0,  0.0, 0.0);
  points[3]=vec3(   0.0, 50.0, 0.0);

  // Now colors defined:
  points[4]=vec3(1, 0, 0); // red
  points[5]=vec3(0, 1, 0); // green

  points[6]=vec3(0, 1, 1); // cyan

  points[7]=vec3(0, 1, 1); // cyan

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

  // 4 points, each is 3 floating point coordinates after which is the
  // color data.
  /*
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(4*3*sizeof(float)));
  */
  // Or you could use the following (since the color data is halfway through the vector):
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)/2));
}  

// The only required callback, it must draw everything when called.
extern "C" void display()
{
  // In order to enable antialiasing, we need the following two lines
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
 
  // Turn on smoothing for lines/points
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  // Tell the system to do it as nice as possible
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

  // The alternative, do it as fast as possible
  //glHint (GL_LINE_SMOOTH_HINT, GL_FASTEST);
  //glHint (GL_POINT_SMOOTH_HINT, GL_FASTEST);

  glClear(GL_COLOR_BUFFER_BIT);          // clear the window
  glPointSize(1.3);
  glDrawArrays(GL_POINTS, 0, 1);         // draw the first point

  glPointSize(20);
  glDrawArrays(GL_POINTS, 1, 1);         // draw the second point

  //  glEnable(GL_LINE_SMOOTH);
  glLineWidth(10.0);
  glDrawArrays(GL_LINES, 2, 2);          // draw the line
  glutSwapBuffers();                     // make the graphics appear
}

int main(int argc, char** argv)
{
  // Standard GLUT initialization

  glutInit(&argc,argv);

  // Add in many of the options for fun.
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ACCUM | GLUT_MULTISAMPLE);

  // Next line requests a double buffered window with an RGBA color
  // model, a depth buffer with at least 24 bits of precsion but
  // preferring more, mutlisampling if available, stencil buffer and
  // accumulation buffer with at least 16 bits of precision, with
  // multisampling if available
  glutInitDisplayString("stencil rgba acca>=16 double depth>=24 samples");

  glutInitWindowSize(500,500); /* 500 x 500 pixel window */
  glutInitWindowPosition(0,0); /* place window top left on display */
  glutCreateWindow("Point Size Demo"); /* window title */
  glutDisplayFunc(display); /* display callback invoked when window opened */

  // Initialize the "magic" that glues all the code together.
  glewInit();

  myinit();             // set attributes
  init();               // set up shaders and display environment
    
  // Find out about my system's capabilities
  GLfloat range[2];
  GLfloat gran;

  glGetFloatv(GL_POINT_SIZE_RANGE, range);
  glGetFloatv(GL_POINT_SIZE_GRANULARITY, &gran);

  cout << "Point size range: " << range[0] 
       << " - " << range[1] << endl 
       << "Point size granularity " <<  gran << endl;

  GLfloat lrange[2];
  GLfloat lgran;

  glGetFloatv(GL_LINE_WIDTH_RANGE, lrange);
  glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &lgran);

  cout << "Line width range: " << lrange[0]
       << " - " << lrange[1] << endl 
       << "Line width granularity " <<  lgran << endl;

  // Determine precision of depth buffer
  GLint depth = glutGet(GLUT_WINDOW_DEPTH_SIZE);
  cout << "Depth Buffer size: " << depth << endl;

  // Alternate way to determine precision of depth buffer
  GLint bits;
  glGetIntegerv(GL_DEPTH_BITS, &bits);
  cout << "Depth Buffer size: " << bits << endl;

  GLint accum = glutGet(GLUT_WINDOW_ACCUM_RED_SIZE);
  cout << "Accum Buffer size: " << accum << endl;

  glutMainLoop(); // enter event loop
    
  return(EXIT_SUCCESS); // return successful exit code
}
