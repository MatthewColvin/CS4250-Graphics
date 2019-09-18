// 
// Sample code to read in terrain data and display as line_strips
// 
// Written by David Chelberg 
// Note the code to move around the grid is not what I want in your
// prog3.  I would like it to feel like you are walking around the
// grid for your programs, i.e. as if you were in a virtual world and
// can walk around and look in different directions as you walk. 
// 
// The interface in this program is just a demo to allow you to look
// at the terrain data.
//
// last-modified: Mon Nov 19 14:07:04 2012
// 

#include "Angel.h"
#include <iostream>
#include <fstream>
#include <string>

using std::string;
using std::ifstream;
using std::cerr;
using std::cout;
using std::endl;

// Adjust this value for your machine (to speed up, make bigger, to
// slow down rotation, make smaller
GLfloat incr =0.06;
// Should we rotate our world?
bool rot=false;

// Where the viewer is initially
vec3 viewer_pos(-1830.5, -1830.5, -2000.0);

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// Vertex data arrays

// Number of total vertices in grid
size_t NumVertices=0;

// Size of input heightmap
size_t Width;
size_t Height;

// Array to hold the vertices for the line strips
point4  *points;

// Temporary array to hold height values read in.
float *z_vals;

// Model-view and projection matrices uniform location
GLuint ModelView;
GLuint Projection;

// Array of rotation angles (in degrees) for each coordinate axis
enum {Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};
int      Axis = Zaxis;
GLfloat  Theta[NumAxes] = {0.0, 0.0, 0.0};

// Uniform to control how texture information is combined with colors.
bool     blend=false;
GLuint   blendLoc;

//----------------------------------------------------------------------------
int Index = 0;

// Assumes Height and Width are size_t global variables.
float* terrain_read (string filename)
{
  ifstream heightFile;
  heightFile.open(filename.c_str(), ifstream::in);
  // Check for errors opening files here
  if (heightFile.fail()) {
    std::cerr << "Error opening file '" << filename << "' in terrain_read" << std::endl;
    exit(EXIT_FAILURE);
  }

  heightFile >> Height >> Width;
  
  float *zvals = new float[Width*Height];
  char chr; // dummy chr to read ","
  size_t index=0;
  for (size_t i=0; i<Width; ++i) {
    for (size_t j=0; j< Height; ++j) {
      // read Height value
      heightFile >> zvals[index++];
      // read ,
      heightFile >> chr;
    }
  }
  heightFile.close();
  return(zvals);
}

// Now we build the terrain model 
// Initial version, just line strips
void terrain_build ()
{
  // Each line has Width points or Height points
  points     = new vec4[2*Width*Height];
  
  // Here we need to generate the line strips
  float x=0.0;
  float y=0.0;
  // For AthensMedium
  float dx=3661.0/Width;
  float dy=3308.0/Height;
  // How much to exaggerate vertical
  float stretch=2.0;

  // The following generates the vertices for the horizontal line strips.
  for (size_t r=0; r<Height; ++r) {
    x=0.0;
    for (size_t c=0; c< Width; ++c) {
      points[NumVertices++]       = vec4(x,    y, stretch*z_vals[r*Width+c], 1);
      x+=dx;
    }
    y+=dy;
  }

  // Generate the vertical line strips
  x=0.0;
  for (size_t c=0; c< Width; ++c) {
    y=0.0;
    for (size_t r=0; r<Height; ++r) {
      points[NumVertices++]       = vec4(x,    y, stretch*z_vals[r*Width+c], 1);
      y+=dy;
    }
    x+=dx;
  }
  
}


//----------------------------------------------------------------------------
void init()
{
  z_vals=terrain_read("AthensMedium.txt");
  terrain_build();  

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*NumVertices, points, GL_STATIC_DRAW);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader75.glsl", "fshader75.glsl");
  glUseProgram(program);

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  // Retrieve transformation uniform variable locations
  ModelView = glGetUniformLocation(program, "ModelView");
  Projection = glGetUniformLocation(program, "Projection");
    
  glEnable(GL_DEPTH_TEST);
    
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4 projection = Perspective(50.0, 1.0, 1.0, 50000.0);

  mat4  model_view = (Translate(viewer_pos) *
		      Translate(1830.5, 1830.5, 0.0) *
		      RotateX(Theta[Xaxis]) *
		      RotateY(Theta[Yaxis]) *
		      RotateZ(Theta[Zaxis]) *
		      Translate(-1830.5, -1830.5, 0.0));
    
  glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
  glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

  // Draw the line strips Height of them each Width points
  size_t strip=0;
  for (;strip < NumVertices/2;strip+=Width) {
    // mode, first, count
    glDrawArrays(GL_LINE_STRIP, strip, Width);
  }
  // Draw the line strips Width of them each Height points
  for (;strip < NumVertices;strip+=Height) {
    // mode, first, count
    glDrawArrays(GL_LINE_STRIP, strip, Height);
  }
  glutSwapBuffers();
}

//----------------------------------------------------------------------------
extern "C" void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    switch(button) {
    case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
    case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
    case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
    }
  }
}

//----------------------------------------------------------------------------
extern "C" void idle()
{
  static GLint time=glutGet(GLUT_ELAPSED_TIME);
  if (rot) {
    Theta[Axis] += incr*(glutGet(GLUT_ELAPSED_TIME)-time);
  }
  time = glutGet(GLUT_ELAPSED_TIME);

  if (Theta[Axis] > 360.0) {
    Theta[Axis] -= 360.0;
  }
    
  glutPostRedisplay();
}

//----------------------------------------------------------------------------
extern "C" void keyboard(unsigned char key, int mousex, int mousey)
{
  switch(key) {
  case 'S':
    incr*=1.5;
    break;
  case 's':
    incr/=1.5;
    break;
  case 'R':
    incr*=-1;
    break;
  case 'r':
    rot = !rot;
    break;
  case 033: // Escape Key
  case 'q': 
  case 'Q':
    exit(EXIT_SUCCESS);
    break;
  case ' ':
    // Reset Viewing
    Theta[0]=Theta[1]=Theta[2]=0.0;
    viewer_pos=vec3(-1830.5, -1830.5, -2000.0);
  case 'x':
    viewer_pos.x-=100.0;
    break;
  case 'X':
    viewer_pos.x+=100.0;
    break;
  case 'y':
    viewer_pos.y-=100.0;
    break;
  case 'Y':
    viewer_pos.y+=100.0;
    break;
  case 'z':
    viewer_pos.z-=100.0;
    break;
  case 'Z':
    viewer_pos.z+=100.0;
    break;
  }

  glutPostRedisplay();
}

//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Simple Terrain");

  glewInit();

  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
