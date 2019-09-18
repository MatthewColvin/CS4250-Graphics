// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.
//
// This version passes a transformation matrix to the vertex shader which
// applies it to the incoming points.
//
// Adapted from an Angel example, by David M. Chelberg
// last-modified: Wed Oct  2 13:10:10 2013
//

#include "Angel.h"

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

// Array of rotation angles (in degrees) for each coordinate axis
enum {Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = {0.0, 0.0, 0.0};

GLuint TransformLoc; // Holds location of transform in shader

// Adjust this value for your machine (to speed up, make bigger, to
// slow down rotation, make smaller
GLfloat incr =0.06;

// New code for a LOT of cubes
const int NumCubes=10000;
// Holds the initial transform for each cube.
mat4 transforms[NumCubes];
// Holds the scale transform.
mat4 scales[NumCubes];
// Holds the relative speed of rotation about each axis for this cube.
GLfloat rot[NumAxes][NumCubes];

//----------------------------------------------------------------------------
// MyQuad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void MyQuad(int a, int b, int c, int d, int e=0)
{
  colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
  colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
  colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
  colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
  colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
  colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;

  // The following code, allows each face to be one color

  // colors[Index] = vertex_colors[e]; points[Index] = vertices[a]; Index++;
  // colors[Index] = vertex_colors[e]; points[Index] = vertices[b]; Index++;
  // colors[Index] = vertex_colors[e]; points[Index] = vertices[c]; Index++;
  // colors[Index] = vertex_colors[e]; points[Index] = vertices[a]; Index++;
  // colors[Index] = vertex_colors[e]; points[Index] = vertices[c]; Index++;
  // colors[Index] = vertex_colors[e]; points[Index] = vertices[d]; Index++;

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
  // MyQuad(1, 0, 3, 2, 1);
  // MyQuad(2, 3, 7, 6, 2);
  // MyQuad(3, 0, 4, 7, 3);
  // MyQuad(6, 5, 1, 2, 4);
  // MyQuad(4, 5, 6, 7, 5);
  // MyQuad(5, 4, 0, 1, 7);
}

//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{
  colorcube();

  // Initialize cube army
  for (size_t cube=0; cube < NumCubes; ++cube){
    transforms[cube] =
      Translate(1.8*drand48()-0.9, 1.8*drand48()-0.9, 1.8*drand48()-0.9) *
      RotateZ(drand48()*360.0)*
      RotateY(drand48()*360.0)*
      RotateX(drand48()*360.0);

    GLfloat scalef = 0.05;
    scales[cube] = Scale(scalef*drand48(), scalef*drand48(), scalef*drand48());

    for (size_t axis=Xaxis; axis < NumAxes;++axis){
	rot[axis][cube] = drand48()*2.0-1.0;
	if (rot[axis][cube]>0.0){
	  rot[axis][cube]+=1.0;
	} else {
	  rot[axis][cube]-=1.0;
	}
      }
  }


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
  GLuint program = InitShader("vshader33.glsl", "fshader32.glsl");
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

  TransformLoc = glGetUniformLocation(program, "transform");

  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0); 
}

//----------------------------------------------------------------------------
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4  transform = (RotateZ(Theta[Zaxis]) *
		     RotateY(Theta[Yaxis]) *
		     RotateX(Theta[Xaxis]));

  // Send transform to graphics card
  glUniformMatrix4fv(TransformLoc, 1, GL_TRUE, transform);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  // Try drawing just lines for the triangles, not filled-in faces
  // for (size_t i=0; i<NumVertices; i+=3) {
  //   glDrawArrays(GL_LINE_LOOP, i, 3);
  // }

  // Let's draw another?
  /*
  transform = Translate(0.6, 0.7, 0.3) *(RotateZ(Theta[Zaxis]*2.0) *
					   RotateY(Theta[Yaxis]*2.0) *
					   RotateX(Theta[Xaxis]*2.0)) * Scale(0.5, 0.5, 0.5);

  // Send transform to graphics card
  glUniformMatrix4fv(TransformLoc, 1, GL_TRUE, transform);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  */

  // Let's draw a lot!
  //  for (size_t cube=0; cube < NumCubes; ++cube){
    // What does this do?
    /*
    transform =
      transforms[cube] *
      RotateZ(Theta[Zaxis]*rot[Zaxis][cube]) *
      RotateY(Theta[Yaxis]*rot[Yaxis][cube]) *
      RotateX(Theta[Xaxis]*rot[Xaxis][cube]) * scales[cube];
    */
    /*
    // What does this do?
    transform =
      RotateZ(Theta[Zaxis]) *
      RotateY(Theta[Yaxis]) *
      RotateX(Theta[Xaxis]) *
      transforms[cube] *
      scales[cube];
    */

    // Last one, what does this transform do?
    /*
    transform =
      RotateZ(Theta[Zaxis]) *
      RotateY(Theta[Yaxis]) *
      RotateX(Theta[Xaxis]) *
      transforms[cube] *
      RotateZ(Theta[Zaxis]*rot[Zaxis][cube]) *
      RotateY(Theta[Yaxis]*rot[Yaxis][cube]) *
      RotateX(Theta[Xaxis]*rot[Xaxis][cube]) * scales[cube];
    */
    // Send transform to graphics card
  /*
    glUniformMatrix4fv(TransformLoc, 1, GL_TRUE, transform);

    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  }
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
  // Added code to allow for speeding up/slowing down speed of
  // rotation. -- DMC
  case '+':
    incr *= 2.0;
    break;
  case '-':
    incr /= 2.0;
    break;
  }
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
  Theta[Axis] += incr*(glutGet(GLUT_ELAPSED_TIME)-time);
  time = glutGet(GLUT_ELAPSED_TIME);

  //  if (Theta[Axis] > 360.0) {
  //    Theta[Axis] -= 360.0;
  //  }
    
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
  glutMouseFunc(mouse);
  glutIdleFunc(idle);

  glutMainLoop();
  return EXIT_SUCCESS;
}
