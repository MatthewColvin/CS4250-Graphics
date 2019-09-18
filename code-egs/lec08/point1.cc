// Test variable point size  -- dmc.
// Added depth buffer size determination, and initial string to ask
// for as much depth precision as possible, etc.
//
// last-modified: Wed Dec 11 16:45:27 2013
// by David M. Chelberg
//
// TODO: Old style code, needs to be updated. 
// 
#include <iostream>
#include <GL/glut.h>

using namespace std;

void myinit(void)
{
  
  /* attributes */
  glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */

  //  glClearColor(0.1, 0.1, 0.1, 0.5); /* dark background */

  glColor3f(1.0, 0.0, 0.0); /* draw in red */

  //  glColor3f(0.0, 1.0, 0.0); /* draw in green */

  /* set up viewing */
  /* 500 x 500 window with origin lower left */

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 500.0, 0.0, 500.0);
  glMatrixMode(GL_MODELVIEW);

  // Test what effect point size has!
  /*  float size;
  cout << "Input Point size for points:";
  cin >> size;
  glPointSize(size);
  */
}

void display( void )
{
  glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

  //  glEnable(GL_POINT_SMOOTH);
  glPointSize(1.3);
  glColor3f(1.0, 0.0, 0.0); /* draw in red */
  glBegin(GL_POINTS);
    glVertex2f(250, 250); 
  glEnd();

  glPointSize(10.0);
  glColor3f(0.0, 1.0, 0.0); /* draw in green */
  glBegin(GL_POINTS);
      glVertex2f(50, 50); 
  glEnd();

  glLineWidth(10.0);
  glBegin(GL_LINES);
      glVertex2f(50, 0); 
      glVertex2f(0, 50); 
  glEnd();

  glutSwapBuffers();
}

int main(int argc, char** argv)
{

/* Standard GLUT initialization */

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

    myinit(); /* set attributes */
    
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
