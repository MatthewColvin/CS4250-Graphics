// animate.cc
// This program demonstrates different animation techniques.
//
// It also demonstrates how to determine which on-screen object
// you have clicked on (i.e. selection) using the drawing with colors
// to another buffer technique.
//
// Written by Prof. David M. Chelberg
//
// Modified to allow byte values (raw pixel data) to be read back from
// GPU allowing 2^24 values to be used for selection with A channel
// added could get to 2^32 (4 billion values).
// 
// last-modified: Fri Oct  6 07:41:04 2017
//

#include <Angel.h>
#include <iostream>
#include <stdlib.h>
#include "square.h"
#include "circle.h"
#include <vector>
#include <random>
#include <time.h>

using std::cin;
using std::cout;
using std::endl;
using std::vector;


// Game functions 
void setupgame();
void setupcharacters(GLint offsetloc,GLint sizeloc, GLint colorloc);


void processSelection(unsigned char PixelColor[], int btn);
void setupMenu();
void glutwindowinit();
vec3 nextselectioncolor();

//Glut Callbackfuncitons 
extern "C" void display();
extern "C" void idle();
extern "C" void mouse(int btn, int state, int x, int y);
extern "C" void menustatus(int status, int x, int y);
extern "C" void motion(int x, int y);
extern "C" void myReshape(int w, int h);
extern "C" void myMenu(int value);
extern "C" void key(unsigned char k, int xx, int yy);
extern "C" void special(int k, int xx, int yy);

//Global Variables 
bool clearscreen=true;
// Bool to determine whether to update the animation
bool updating=true;
//vector to hold all the characters
vector<Square*> characters;


// Data storage for our geometry for the lines
vec2 *points;
// Used to keep track of where we are in generating selection colors
vec3 selectioncolors = vec3(0.0,0.0,0.0);
// Window Size 
GLint windowSizeLoc;
// Window size
int win_h=900;
int win_w=900;



extern "C" void display(){
  if (clearscreen) {
    glClear(GL_COLOR_BUFFER_BIT);
  }
  //Draw all the characters
  for (auto character : characters){
    character->draw();
  }

  glutSwapBuffers ();
}

extern "C" void idle(){
  if (updating) {

    glutPostRedisplay();
  }
}

void processSelection(unsigned char PixelColor[], int btn){
  // std::cout << PixelColor.x << " " << PixelColor.y << " " << PixelColor.z << std::endl;
  
  for (auto character : characters){
    if (cmpcolor(PixelColor,character->getSelectColor())){
      character->Selected();
    }else{
      character->notSelected();
    }
  } 
}

// Mouse callback, implements selection by using colors in the back buffer.
extern "C" void mouse(int btn, int state, int x, int y){
  if (state == GLUT_DOWN) {
    // Draw the scene with identifying colors
    // Ensure the clear color isn't the same as any of your objects
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw the character in the selection color in the back buffer
    for (auto character : characters){
      character->draw(true);
    }

    // Flush ensures all commands have drawn
    glFlush();

    // Read the value at the location of the cursor
    // In the back buffer (not the one visible on-screen)
    glReadBuffer(GL_BACK);
    unsigned char PixelColor[3];
    glReadPixels(x, win_h-y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &PixelColor);
    std::cout << int(PixelColor[0]) << " "
	      << int(PixelColor[1]) << " "
	      << int(PixelColor[2]) << std::endl;
    processSelection(PixelColor, btn);


    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay();
  }
}

// Allows you to still get location of mouse clicks when using menus
extern "C" void menustatus(int status, int x, int y){
  // Pass the new location to the object.

  glutPostRedisplay();
}  

// Called whenever mouse moves, after being pressed
extern "C" void motion(int x, int y){

 
  glutPostRedisplay();
}


// Maintains the mapping from screen to world coordinates.
extern "C" void myReshape(int w, int h){
  glViewport(0,0,w,h);
  win_h = h;
  win_w = w;
  glUniform2f(windowSizeLoc, win_w, win_h);       // Pass the window size
						  // size
  glutPostRedisplay();
}

// Example menu code.
extern "C" void myMenu(int value){
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
    break;
  }
  glutPostRedisplay(); 
}


// Create menu and items.
void setupMenu(){
  glutCreateMenu(myMenu);
  glutAddMenuEntry("clear screen", 1);
  glutAddMenuEntry("red background", 2);
  glutAddMenuEntry("black background", 3);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Keypress events.
extern "C" void key(unsigned char k, int xx, int yy){
  switch (k) {
  case 'q':
  case 'Q': 
    exit(0);
    break;
  case 'c':
    clearscreen = !clearscreen;
  }


  // control the selected hero
  
  for (auto character: characters){
    if(character->isSelected()){
      switch (k){
      case 'w':
        character->move_up(10);
        break;
      case 'a':
        character->move_left(10);
        break;
      case 's':
        character->move_down(10);
        break;
      case 'd':
        character->move_right(10);
        break;
      }
    }
  }
  glutPostRedisplay();
}

// Special Keys events.
// This one only responds to the up arrow key.
extern "C" void special(int k, int xx, int yy){
  switch (k) {
  case GLUT_KEY_UP:
    break;
  case GLUT_KEY_DOWN:
    break;
  case GLUT_KEY_LEFT:
    break;
  case GLUT_KEY_RIGHT:
    break;
  default:
    // do nothing
    break;
  }
  glutPostRedisplay();
}

// Initialize all OpenGL callbacks, create window.
void glutwindowinit(){
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(win_w,win_h);
  glutInitWindowPosition(20,20);

  glutCreateWindow("Homework 1");

  // Color initializations
  glClearColor(0.0, 0.0, 0.0, 1.0);
  
  // Can you do this?  Attach a menu to a button already
  // used for something else?  
  setupMenu();

  // Callbacks
  glutDisplayFunc(display); 
  glutIdleFunc(idle); 
  glutReshapeFunc (myReshape);
  glutMouseFunc (mouse);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutMotionFunc (motion);

  // NOTE: This is not in book. The following sets up a callback
  // whenever a menu is in use.  It gives mouse click location as well
  // as the status of the use of the menu.
  glutMenuStatusFunc(menustatus);
}

// This function initializes the buffers and shaders
void setupgame(){
  // Locations of variables in shaders.
  // Offset of square
  GLint offsetLoc;
  // Size of square
  GLint sizeLoc;
  // Color of square 
  GLint colorLoc;

  // Create a vertex array object - this will hold the buffers necessary for the VAO
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object for transferring data to
  // the GPU.
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader.glsl", "fshader.glsl");

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

  // We need just one square and circle to draw any number of them.
  points = new vec2[Square::NumPoints+Circle::NumPoints];
  //  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  setupcharacters(offsetLoc,sizeLoc,colorLoc);
  // Send the data to the graphics card, after it has been generated
  // by creating the objects in the world (above).
  glBufferData(GL_ARRAY_BUFFER, (Square::NumPoints+Circle::NumPoints)*sizeof(vec2), points, GL_STATIC_DRAW);
}

void setupcharacters(GLint offsetloc,GLint sizeloc,GLint colorloc){
  srand(time(NULL));// seed the random function with time
  GLfloat randomx, randomy;
  for (int i = 0; i<10; i++){
    randomx = rand() % win_w;
    randomy = rand() % win_h;
    
    characters.push_back(new Square(0,points,offsetloc,sizeloc,colorloc));
    characters[i]->change_size(20);
    characters[i]->move(randomx,randomy);
    characters[i]->selectColor(nextselectioncolor());
  }

}
// generates a selection color different from all others
vec3 nextselectioncolor(){
  if (selectioncolors.x < 255.0){
    if (selectioncolors.y >= 255.0){
      selectioncolors.x++;
      selectioncolors.y = 0;
    }
    if(selectioncolors.y < 255.0){
      if (selectioncolors.z >= 255.0){
        selectioncolors.y++;
        selectioncolors.z = 0;
      }
      if(selectioncolors.z < 255.0){
        selectioncolors.z++;
      }
    }
  }
  return selectioncolors/255;
}


int main(int argc, char** argv){
  // Several people forgot to put in the following line.  This is an
  // error, even if it still works on your machine, a program is
  // incorrect without the following call to initialize GLUT.
  glutInit(&argc,argv);

  glutwindowinit();
  // Initialize the "magic" that glues all the code together.
  glewInit();

  setupgame();   // set up shaders and display environment

  glutMainLoop();       // enter event loop
  return(EXIT_SUCCESS); // return successful exit code
}
