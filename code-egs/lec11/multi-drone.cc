// drone.cc
// This program demonstrates how to draw multiple copies of objects at
// different locations on-screen.
//
// Written by dmc to show different mouse callbacks, and
// to show how menus interact with mouse callbacks.
//
// Modified to draw multiple drones on-screen to answer a questino
// from a student about the second programming project.
//
// Modified for better style -- dmc
// Modified for OpenGL 3.2 -- dmc
// Modified to display drone -- dmc
// Modified to color drone blades differently from drone -- dmc
// Modified to remove cout statements -- dmc
// Modified for better style -- dmc
//
// last-modified: Mon Oct 10 14:16:33 2016

#include <Angel.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using std::vector;
using std::cout;
using std::cerr;
using std::endl;

// How many points are needed to specify the square
const int NumPoints = 4;

// Data storage for our geometry for the lines
vec2 points[NumPoints];

// How big a square to draw.
vec2 size=vec2(30.0,40.0);
// rotation angle of rotors.
GLfloat angle;
GLfloat angleincr=0.2;
bool animate=false;

// Current color of drone body
vec4 droneColor;

// constant colors for drone body
const vec4 WHITE(1.0, 1.0, 1.0, 1.0);
const vec4 YELLOW(1.0, 1.0, 0.0, 1.0);
const vec4 BLUE(0.0, 0.0, 1.0, 1.0);
const vec4 CYAN(0.0, 1.0, 1.0, 1.0);

// Locations of variables in shaders.

GLint offsetLoc;     // Offset (position) of square
GLint sizeLoc;       // Size of square
GLint colorLoc;      // Color of square 
GLint windowSizeLoc; // Window Size 
GLint angleLoc;      // Offset of rotation angle

// Mouse location
int mouse_x=0;
int mouse_y=0;
// Window size
int win_h=0;
int win_w=0;

vector<vec2> drones;  // global to hold locations of drones.

// Sequence of display callbacks illustrating the evolution of the
// design of the drone code.
extern "C" void disp1()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glUniform2fv(sizeLoc,1, size);
  glUniform1f(angleLoc, 0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);  // draw the square
  
  // What do the following calls do?
  glUniform2fv(sizeLoc,1, size/2.0);
  glUniform2i(offsetLoc, mouse_x+size[0], win_h - mouse_y+size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x+size[0], win_h - mouse_y-size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x-size[0], win_h - mouse_y+size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x-size[0], win_h - mouse_y-size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);

  glutSwapBuffers ();
}

extern "C" void disp2()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glUniform2fv(sizeLoc,1, size);
  glUniform1f(angleLoc, 0);
  glUniform2i(offsetLoc, mouse_x, win_h - mouse_y);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);  // draw the square
  
  // What do the following calls do?
  glUniform2fv(sizeLoc,1, size/2.0);
  glUniform2i(offsetLoc, mouse_x+size[0], win_h - mouse_y+size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x+size[0], win_h - mouse_y-size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x-size[0], win_h - mouse_y+size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x-size[0], win_h - mouse_y-size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glutSwapBuffers ();
}

extern "C" void disp3()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glUniform2fv(sizeLoc,1, size);
  glUniform2i(offsetLoc, mouse_x, win_h - mouse_y);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);  // draw the square
  
  // What do the following calls do?
  glUniform4fv(colorLoc, 1, YELLOW);
  glUniform1f(angleLoc, angle);
  vec2 size2=vec2(size[0]/10.0, size[1]);
  glUniform2fv(sizeLoc,1, size2/2.0);
  glUniform2i(offsetLoc, mouse_x+size[0], win_h - mouse_y+size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x+size[0], win_h - mouse_y-size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x-size[0], win_h - mouse_y+size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, mouse_x-size[0], win_h - mouse_y-size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);

  glutSwapBuffers ();
}

void draw_a_drone(vec2 location)
{
  glUniform4fv(colorLoc, 1, droneColor);
  glUniform2fv(sizeLoc,1, size);
  glUniform1f(angleLoc, 0);
  //  glUniform2i(offsetLoc, mouse_x, win_h - mouse_y);
  glUniform2i(offsetLoc, location.x, location.y);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);  // draw the square
  
  // What do the following calls do?
  glUniform4fv(colorLoc, 1, YELLOW);
  glUniform1f(angleLoc, angle);
  vec2 size2=vec2(size[0]/10.0, size[1]);
  glUniform2fv(sizeLoc,1, size2/2.0);
  glUniform2i(offsetLoc, location.x+size[0], location.y+size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, location.x+size[0], location.y-size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, location.x-size[0], location.y+size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
  glUniform2i(offsetLoc, location.x-size[0], location.y-size[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, NumPoints);
}

extern "C" void disp4()
{
  glClear(GL_COLOR_BUFFER_BIT);
  //  vec2 location=vec2(mouse_x, win_h - mouse_y);
  for (size_t i=0; i<drones.size();++i) {
    draw_a_drone(drones[i]);
  }

  glutSwapBuffers ();
}
// For animation
extern "C" void idle()
{
  // How would you modify the program so that each drone's angle was
  // independent of each other, and their speeds of rotation also are
  // all different?
  if (animate){
    angle+=angleincr;
  }
  glutPostRedisplay();
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
  if (btn==GLUT_LEFT_BUTTON) {
    // Change color
    droneColor=WHITE;
    glUniform4fv(colorLoc, 1, droneColor);
    mouse_x = x;
    mouse_y = y;

    // Pass the new location to the shader.
    glUniform2i(offsetLoc, x, win_h - y);
    drones[0]=vec2(mouse_x, win_h - mouse_y);
  } else if (btn==GLUT_MIDDLE_BUTTON) {
    // Add a new drone to list
    drones.push_back(vec2(mouse_x, win_h - mouse_y));
    
  }
  glutPostRedisplay(); 
}

// Allows you to still get location of mouse clicks when using menus
extern "C" void menustatus(int status, int x, int y)
{
  mouse_x = x;
  mouse_y = y;

  // Pass the new location to the shader.
  glUniform2i(offsetLoc, x, win_h - y);
  drones[0]=vec2(mouse_x, win_h - mouse_y);
 
  glutPostRedisplay(); 
}  

// Called whenever mouse moves, after being pressed
extern "C" void motion(int x, int y)
{
  mouse_x = x;
  mouse_y = y;

  // Pass the new location to the shader.
  glUniform2i(offsetLoc, x, win_h - y);
   drones[0]=vec2(mouse_x, win_h - mouse_y);
 
  // Set color to blue
  droneColor=BLUE;
  glUniform4fv(colorLoc, 1, droneColor);

  glutPostRedisplay(); 
}

// Called whenever mouse moves, when no buttons pressed
extern "C" void passivemotion(int x, int y)
{
  mouse_x = x;
  mouse_y = y;

  // Pass the new location to the shader.
  glUniform2i(offsetLoc, x, win_h - y);
  drones[0]=vec2(mouse_x, win_h - mouse_y);
 
  droneColor=CYAN;
  glUniform4fv(colorLoc, 1, droneColor);

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
    glClear (GL_COLOR_BUFFER_BIT);
    break;
  case 2:
    // Set clear color to red
    glClearColor (1.0, 0.0, 0.0, 1.0);
    break;
  case 3:
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
  case '1':
    glutDisplayFunc(disp1);
    break;
  case '2':
    glutDisplayFunc(disp2);
    break;
  case '3':
    glutDisplayFunc(disp3);
    break;
  case '4':
    glutDisplayFunc(disp4);
    break;
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
    break;
  case ' ':
    animate=!animate;
    break;
  case 'R':
    angleincr*=2.0;
    break;
  case 'r':
    angleincr/=2.0;
    break;
  case 'y':
    droneColor=YELLOW;
    glutPostRedisplay();
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
  }
}

// Initialize all OpenGL callbacks, create window.
void myinit ()
{
  drones.push_back(vec2(0,0));
  points[0]= vec2( 1,  1);
  points[1]= vec2(-1,  1);
  points[3]= vec2(-1, -1);
  points[2]= vec2( 1, -1);

  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500,500);

  glutCreateWindow("Drone test");

  // Color initializations
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // Can you do this?  Attach a menu to a button already
  // used for something else?  
  setupMenu();

  // Callbacks
  glutDisplayFunc(disp1); 
  glutIdleFunc(idle); 
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
  droneColor=WHITE;
  glUniform4fv(colorLoc, 1, droneColor);

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
  GLuint program = InitShader("vshaderDrone.glsl", "fshaderCallback.glsl");


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
  angleLoc  = glGetUniformLocation(program, "angle");
  if (angleLoc==-1) {
    std::cerr << "Unable to find angleLoc parameter" << std::endl;
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
