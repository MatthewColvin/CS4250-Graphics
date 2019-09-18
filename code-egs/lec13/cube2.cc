// Book code from Angel.
//
// Perspective view of a color cube using LookAt() and Perspective()
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.
//
// Modified by David Chelberg
// last-modified: Fri Oct 28 14:06:29 2016
// Added an animated rotation to the code.
// Separated the transform for the camera (camera_view) from the
//   object (model_view).
// Added code to draw a "door"
// Added code to draw a moving second cube (animated moving back and
//   forth in a straight line).
// Added code to account for elapsed time in the animations.

#include <Angel.h>
#include "common.h"
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4(-0.5, -0.5,  0.5, 1.0),
    point4(-0.5,  0.5,  0.5, 1.0),
    point4( 0.5,  0.5,  0.5, 1.0),
    point4( 0.5, -0.5,  0.5, 1.0),
    point4(-0.5, -0.5, -0.5, 1.0),
    point4(-0.5,  0.5, -0.5, 1.0),
    point4( 0.5,  0.5, -0.5, 1.0),
    point4( 0.5, -0.5, -0.5, 1.0)
};

// RGBA olors
color4 vertex_colors[8] = {
    color4(0.0, 0.0, 0.0, 1.0),  // black
    color4(1.0, 0.0, 0.0, 1.0),  // red
    color4(1.0, 1.0, 0.0, 1.0),  // yellow
    color4(0.0, 1.0, 0.0, 1.0),  // green
    color4(0.0, 0.0, 1.0, 1.0),  // blue
    color4(1.0, 0.0, 1.0, 1.0),  // magenta
    color4(1.0, 1.0, 1.0, 1.0),  // white
    color4(0.0, 1.0, 1.0, 1.0)   // cyan
};

// Viewing transformation parameters
GLfloat radius = 3.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

const GLfloat  dr = 5.0 * DegreesToRadians;

// Rotation of first cube
bool rotatep;                  // whether to rotate or not
GLfloat angle = 0.0;           // Angle of cube rotation.

// Implementing a door that opens by swinging
GLfloat doorAngle = 0.0;       // Angle of cube's door's rotation.
GLfloat doorAngleIncr = 30.0;   // Amount to increment Angle of cube's
			       // door's rotation.

GLfloat trans = 0.0;   // trans of 2nd cube.
GLfloat transinc = 1.0;// trans increment of 2nd cube.

GLuint  camera_view;  // camera-view matrix uniform shader variable location
GLuint  model_view;  // model-view matrix uniform shader variable location

// Projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 3.0;
//GLfloat  zNear = 0.1;
//GLfloat zFar = 300.0;

GLuint  projection; // projection matrix uniform shader variable location

// Global to keep track of what vertex we are setting.
int Index = 0;

//----------------------------------------------------------------------------
// MyQuad generates two triangles for each face and assigns colors
//    to the vertices
void MyQuad(int a, int b, int c, int d)
{
  colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
  colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
  colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
  colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
  colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
  colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void colorcube()
{
  MyQuad(1, 0, 3, 2);
  MyQuad(2, 3, 7, 6);
  MyQuad(3, 0, 4, 7);
  MyQuad(6, 5, 1, 2);
  MyQuad(4, 5, 6, 7);
  MyQuad(5, 4, 0, 1);
}

//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{
  colorcube();

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
	       NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader41.glsl", "fshader41.glsl");
  glUseProgram(program);

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  GLuint vColor = glGetAttribLocation(program, "vColor"); 
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)));

  model_view = glGetUniformLocation(program, "model_view");
  camera_view = glGetUniformLocation(program, "camera_view");
  projection = glGetUniformLocation(program, "projection");
    
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0); 
}

//----------------------------------------------------------------------------
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  point4  eye(mvx+radius*sin(theta)*cos(phi),
	      radius*sin(theta)*sin(phi),
	      mvz+radius*cos(theta),
	      1.0);
  point4  at(mvx, 0.0, mvz, 1.0);
  vec4    up(0.0, 1.0, 0.0, 0.0);

  mat4  cv = LookAt(eye, at, up);
  glUniformMatrix4fv(camera_view, 1, GL_TRUE, cv);

  mat4 mv = RotateZ(angle);
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);

  mat4  p = Perspective(fovy, aspect, zNear, zFar);
  glUniformMatrix4fv(projection, 1, GL_TRUE, p);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);


  // What does this draw?
  /*
  // The Y translation is so that the two planes aren't at the exact
  // same z-value which will cause artifacts in the scene.  Try making
  // it 0 and see what happens.
  mv = Translate(trans, 0.001, 0.5);
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  */

  /*
  // What does this do?
  mv = RotateZ(angle)*Translate(0.5, 0.0, 0.5)*RotateY(doorAngle)*Translate(-0.5, 0.0, -0.5);
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  */

  glutSwapBuffers();
}

//----------------------------------------------------------------------------

extern "C" void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case 033: // Escape Key
  case 'q': case 'Q':
    exit(EXIT_SUCCESS);
    break;

  case 'z': zNear  *= 1.1; zFar /= 1.1; break;
  case 'Z': zNear /= 1.1; zFar *= 1.1; break;
  case 'r': radius *= 1.5; break;
  case 'R': radius /= 1.5; break;
  case 'o': theta += dr; break;
  case 'O': theta -= dr; break;
  case 'p': phi += dr; break;
  case 'P': phi -= dr; break;
  case 'v': 
    fovy-=5; 
    if (fovy < 0) {
      // Min angle of view 1 degree
      fovy = 1;
    }
    break;
  case 'V': fovy+=5; break;
    if (fovy > 179) {
      // Max angle of view 179 degrees
      fovy = 179;
    }
    break;

  case ' ':  // reset values to their defaults
    rotatep=!rotatep;
    //    zNear = 0.1; 
    //    zFar = 300.0;
    zNear = 0.5;
    zFar = 3.0;

    radius = 3.0;
    theta  = 0.0;
    phi    = 0.0;
    break;
  }

  glutPostRedisplay();
}

//----------------------------------------------------------------------------
extern "C" void reshape(int width, int height)
{
  glViewport(0, 0, width, height);

  aspect = GLfloat(width)/height;
}

// Simple animation
extern "C" void idle()
{
  // Added code to account for glutElapsedTime
  static GLint lasttime = glutGet(GLUT_ELAPSED_TIME);
  GLint time = glutGet(GLUT_ELAPSED_TIME);

  // Code to animate cube goes here.
  if (rotatep) {
    angle+=20.0/1000.0*(time-lasttime);
  }

  doorAngle+=doorAngleIncr/1000.0*(time-lasttime);;
  if (doorAngle > 60.0) {
    doorAngleIncr*=-1.0;
  }
  if (doorAngle < 0.0){
    doorAngle = 0.0;
    doorAngleIncr*=-1.0;
  }

  // Do the animation code here in idle, not in display.
  //  Code for moving 2nd cube
  trans += transinc/1000.0*(time-lasttime);
  if (trans > 1.5) {
    trans = 1.5;
    transinc*=-1;
  }
  if (trans < -1.5) {
    trans = -1.5;
    transinc*=-1;
  }
  lasttime=time;

  glutPostRedisplay();
}


//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Color Cube");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
