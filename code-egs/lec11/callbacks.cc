// callbacks.cc
// This program demonstrates different callbacks for
// interaction.
//
// Written by dmc to show different mouse callbacks, and
// to show how menus interact with mouse callbacks.
//
// Modified for better style -- dmc
// Modified for OpenGL 3.2 -- dmc
//
//   last-modified: Mon Oct  1 15:15:34 2012

#include <Angel.h>
#include <iostream>
#include <stdlib.h>

using std::cout;
using std::cerr;
using std::endl;

// How many points are needed to specify the square
const int NumPoints = 4;

// Data storage for our geometry for the lines
vec2 points[NumPoints];

// How big a square to draw.
GLfloat size=30.0;

// Locations of variables in shaders.
// Offset of square
GLint offsetLoc;
// Size of square
GLint sizeLoc;
// Color of square 
GLint colorLoc;
// Window Size 
GLint windowSizeLoc;

// Mouse location
int mouse_x=0;
int mouse_y=0;
// Window size
int win_h=0;
int win_w=0;

// Simple display draws a square of size sq_size*2 around mouse
// location.
extern "C" void display()
{
  // Actually only need to do the following whenever the size is modified.
  glUniform1f(sizeLoc, size);                     // Pass the current
						  // size of the
						  // square

  // What happens if the following is commented out?  Explain.
  glClear(GL_COLOR_BUFFER_BIT);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);  // draw the square
  
  // What do the following calls do?
  /*
  glUniform1f(sizeLoc, size/2.0);   
  glUniform2i(offsetLoc, mouse_x+size, win_h - mouse_y+size);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x+size, win_h - mouse_y-size);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x-size, win_h - mouse_y+size);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x-size, win_h - mouse_y-size);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  */

  glutSwapBuffers ();
}

// For debugging
void print_mouse_btn(int btn)
{
  switch (btn)  {
  case GLUT_LEFT_BUTTON:
    cout << "Left";
    break;
  case GLUT_MIDDLE_BUTTON:
    cout << "Middle";
    break;
  case GLUT_RIGHT_BUTTON:
    cout << "Right";
    break;
  default:
    cout << "Unknown button";
    break;
  }
}
    
// For debugging
void print_mouse_state(int state)
{
  switch (state) {
  case GLUT_UP:
    cout << "Up";
    break;
  case GLUT_DOWN:
    cout << "Down";
    break;
  default:
    cout << "Unknown button state";
    break;
  }
}
 
// Mouse callback, simply prints and records location, and sets color
// to white.
extern "C" void mouse(int btn, int state, int x, int y)
{
  // Change color
  glUniform4f(colorLoc, 1.0, 1.0, 1.0, 1.0);
  cout << "Mouse btn: ";
  print_mouse_btn(btn);
  cout << " state " ;
  print_mouse_state(state);
  cout << " x= " << x 
       << " y= " << y << endl;

  mouse_x = x;
  mouse_y = y;

  // Pass the new location to the shader.
  glUniform2i(offsetLoc, x, win_h - y);

  glutPostRedisplay(); 
}

// Allows you to still get location of mouse clicks when using menus
extern "C" void menustatus(int status, int x, int y)
{
  cout << "Menu Status = " << status;
  cout << " x= " << x 
       << " y= " << y << endl;

  mouse_x = x;
  mouse_y = y;

  // Pass the new location to the shader.
  glUniform2i(offsetLoc, x, win_h - y);

  glutPostRedisplay(); 
}  

// Called whenever mouse moves, after being pressed
extern "C" void motion(int x, int y)
{
  mouse_x = x;
  mouse_y = y;

  // Pass the new location to the shader.
  glUniform2i(offsetLoc, x, win_h - y);

  // Set color to blue
  glUniform4f(colorLoc, 0.0, 0.0, 1.0, 1.0);

  glutPostRedisplay(); 
}

// Called whenever mouse moves, when no buttons pressed
extern "C" void passivemotion(int x, int y)
{
  mouse_x = x;
  mouse_y = y;

  // Pass the new location to the shader.
  glUniform2i(offsetLoc, x, win_h - y);

  // Set color to cyan
  glUniform4f(colorLoc, 0.0, 1.0, 1.0, 1.0);

  glutPostRedisplay(); 
}

// Maintains the mapping from screen to world coordinates.
extern "C" void myReshape(int w, int h)
{
  glViewport(0,0,w,h);
  win_h = h;
  win_w = w;
  glUniform2f(windowSizeLoc, win_w, win_h);       // Pass the window size
						  // size
  glutPostRedisplay();
}

// Example menu code.
extern "C" void myMenu(int value)
{
  switch (value) {
  case 1:
    // Clear screen
    cout << "Entry 1" << endl;
    glClear (GL_COLOR_BUFFER_BIT);
    break;
  case 2:
    cout << "Entry 2" << endl;
    // Set clear color to red
    glClearColor (1.0, 0.0, 0.0, 1.0);
    break;
  case 3:
    cout << "Entry 3" << endl;
    // Set clear color to black
    glClearColor (0.0, 0.0, 0.0, 1.0);
    break;
  default:
    cout << "Menu value = " << value << endl;
    break;
  }
  glutPostRedisplay(); 
}


// Create menu and items.
// %%%
// %%% What happens if we change the menu's numbering scheme?
// %%%
void setupMenu()
{
  //  int menu_id=glutCreateMenu(myMenu);
  glutCreateMenu(myMenu);
  glutAddMenuEntry("clear screen", 1);
  glutAddMenuEntry("red background", 2);
  glutAddMenuEntry("black background", 3);
  /*
  glutAddMenuEntry("The Answer", 42);
  glutAddMenuEntry("clear screen", 32);
  glutAddMenuEntry("red background", 22);
  glutAddMenuEntry("black background", 12);
  */

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Keypress events.
// %%%
// %%% WHICH CALLBACK PROCESSES THE UP ARROW KEY?
// %%%
extern "C" void key(unsigned char k, int xx, int yy)
{
  switch (k) {
  case 'q':
  case 'Q': 
    exit(0);
    break;
  // make the size of the square larger
  case '+':
    size+=2;
    glutPostRedisplay();
    break;
  // make the size of the square smaller
  case '-':
    size-=2;
    // Test for non-sensical size.
    if (size < 0) {
      size=0;
    }
    glutPostRedisplay();
    break;
  case GLUT_KEY_UP:
    cout << "key up pressed normal" << endl;
    break;
  default:
    // Anything else.
    break;
  }
}

// Special Keys events.
// This one only responds to the up arrow key.
extern "C" void special(int k, int xx, int yy)
{
  if (k==GLUT_KEY_UP) {
    cout << "key up pressed special" << endl;
  }
}

// Initialize all OpenGL callbacks, create window.
void myinit ()
{
  points[0]= vec2( 1,  1);
  points[1]= vec2(-1,  1);
  points[3]= vec2(-1, -1);
  points[2]= vec2( 1, -1);

  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500,500);

  glutCreateWindow("Mouse interaction test");

  // Color initializations
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // Can you do this?  Attach a menu to a button already
  // used for something else?  
  setupMenu();

  // Callbacks
  glutDisplayFunc(display); 
  glutReshapeFunc (myReshape);
  glutMouseFunc (mouse);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutMotionFunc (motion);
  glutPassiveMotionFunc (passivemotion);

  // NOTE: This is not in book. The following sets up a callback
  // whenever a menu is in use.  It gives mouse click location as well
  // as the status of the use of the menu.
  glutMenuStatusFunc(menustatus);
}

// This function initializes the buffers and shaders
void init()
{
  // Initial color white.
  glUniform4f(colorLoc, 1.0, 1.0, 1.0, 1.0);

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
  GLuint program = InitShader("vshaderCallback.glsl", "fshaderCallback.glsl");


  // InitShader does the glUseProgram
  // So the following would be redundant
  // glUseProgram(program);

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
  colorLoc  = glGetUniformLocation(program, "vcolor");
  if (colorLoc==-1) {
    std::cerr << "Unable to find colorLoc parameter" << std::endl;
  }
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glUniform2f(offsetLoc, 0.0, 0.0);               // Initial offset
						  // for mouse loc.
}  


//  Main Program
int main(int argc, char** argv)
{
  // Several people forgot to put in the following line.  This is an
  // error, even if it still works on your machine, a program is
  // incorrect without the following call to initialize GLUT.
  glutInit(&argc,argv);

  myinit();             // set window attributes, and initial data

  // Initialize the "magic" that glues all the code together.
  glewInit();

  init();               // set up shaders and display environment
  
  glutMainLoop();       // enter event loop

  return(EXIT_SUCCESS); // return successful exit code
}
