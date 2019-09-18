//
//  Display a rotating cube with lighting
//
//  Light and material properties are sent to the shader as uniform
//    variables.  Vertex positions and normals are sent after each
//    rotation.
//
// Modified by David M. Chelberg for style
// last-modified: Tue Oct 16 16:14:55 2012

#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
vec3   normals[NumVertices];

// Adjust this value for your taste (to speed up, make bigger, to slow
// down rotation, make smaller
GLfloat incr =-0.06;

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

// Array of rotation angles (in degrees) for each coordinate axis
enum {Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = {0.0, 0.0, 0.0};

// Model-view and projection matrices uniform location
GLint  ModelView, Projection;
// Bool for whether to turn on/off specular lighting.
bool spec=true;
// Uniform location of above.
GLint Spec;

//----------------------------------------------------------------------------
// MyQuad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

void MyQuad(int a, int b, int c, int d)
{
  // Initialize temporary vectors along the MyQuad's edge to
  //   compute its face normal 
  vec4 u = vertices[b] - vertices[a];
  vec4 v = vertices[c] - vertices[b];

  vec3 normal = normalize(cross(u, v));

  normals[Index] = normal; points[Index] = vertices[a]; Index++;
  normals[Index] = normal; points[Index] = vertices[b]; Index++;
  normals[Index] = normal; points[Index] = vertices[c]; Index++;
  normals[Index] = normal; points[Index] = vertices[a]; Index++;
  normals[Index] = normal; points[Index] = vertices[c]; Index++;
  normals[Index] = normal; points[Index] = vertices[d]; Index++;
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

// Utility function to check for errors and report messages.
// Could also exit if errors are fatal.
void check_err(const char *text, int amb)
{
  if (amb==-1) {
    std::cerr << "WARNING: " << text << " not found in shader!" << std::endl;
  }
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
	       NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
		  sizeof(normals), normals);

  // Load shaders and use the resulting shader program
  GLint program = InitShader("vshader53.glsl", "fshader53.glsl");
  glUseProgram(program);

  // set up vertex arrays
  GLint vPosition = glGetAttribLocation(program, "vPosition");
  check_err("vPosition", vPosition);
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  GLint vNormal = glGetAttribLocation(program, "vNormal"); 
  check_err("vNormal", vNormal);
  glEnableVertexAttribArray(vNormal);
  glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)));

  // Initialize shader lighting parameters

  // What light source is the following?
  //point4 light_position(0.0, 0.0, 1.0, 1.0);

  // What light source is the following?
  point4 light_position(0.0, 0.0, 1.0, 0.0);

  color4 light_ambient(0.2, 0.2, 0.2, 1.0);
  color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
  color4 light_specular(1.0, 1.0, 1.0, 1.0);

  color4 material_ambient(1.0, 0.0, 1.0, 1.0);
  color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
  color4 material_specular(1.0, 0.8, 0.0, 1.0);
  float  material_shininess = 100.0;

  color4 ambient_product = light_ambient * material_ambient;
  color4 diffuse_product = light_diffuse * material_diffuse;
  color4 specular_product = light_specular * material_specular;

  GLint amb=glGetUniformLocation(program, "AmbientProduct");
  check_err("AmbientProduct", amb);
  glUniform4fv(amb, 1, ambient_product);

  GLint diff=glGetUniformLocation(program, "DiffuseProduct");
  check_err("DiffuseProduct", diff);
  glUniform4fv(diff, 1, diffuse_product);

  GLint spec=glGetUniformLocation(program, "SpecularProduct");
  check_err("SpecularProduct", spec);
  glUniform4fv(spec, 1, specular_product);
	
  GLint light=glGetUniformLocation(program, "LightPosition");
  check_err("LightPosition", light);
  glUniform4fv(light, 1, light_position);

  GLint shine=glGetUniformLocation(program, "Shininess");
  check_err("Shininess", shine);
  glUniform1f(shine, material_shininess);
		 
  // Retrieve transformation uniform variable locations
  ModelView = glGetUniformLocation(program, "ModelView");
  check_err("ModelView", ModelView);

  Projection = glGetUniformLocation(program, "Projection");
  check_err("Projection", Projection);

  // Retrieve transformation uniform variable locations
  Spec = glGetUniformLocation(program, "Spec");
  check_err("Spec", Spec);
  glUniform1i(Spec, spec);

  glEnable(GL_DEPTH_TEST);

  glClearColor(1.0, 1.0, 1.0, 1.0); 
}

//----------------------------------------------------------------------------
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //  Generate tha model-view matrixn
  const vec3 viewer_pos(0.0, 0.0, 2.0);
  mat4  model_view = (Translate(-viewer_pos)*
		      RotateX(Theta[Xaxis]) *
		      RotateY(Theta[Yaxis]) *
		      RotateZ(Theta[Zaxis]));

  // This version doesn't move the viewer.
  /*    mat4  model_view = (RotateX(Theta[Xaxis]) *
		      RotateY(Theta[Yaxis]) *
		      RotateZ(Theta[Zaxis]));
  */
  glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
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
  Theta[Axis] += incr*(glutGet(GLUT_ELAPSED_TIME)-time);
  time = glutGet(GLUT_ELAPSED_TIME);

  if (Theta[Axis] > 360.0) {
    Theta[Axis] -= 360.0;
  }
    
  glutPostRedisplay();
}

//----------------------------------------------------------------------------
extern "C" void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case 033: // Escape Key
  case 'q': case 'Q':
    exit(EXIT_SUCCESS);
    break;
  case 's': case 'S':
    spec=!spec;
    glUniform1i(Spec, spec);
    break;
  }  
}

//----------------------------------------------------------------------------
extern "C" void reshape(int width, int height)
{
  glViewport(0, 0, width, height);

  GLfloat aspect = GLfloat(width)/height;
  mat4  projection = Perspective(45.0, aspect, 0.5, 3.0);
  // What if we wanted to use orthographic perspective?
  //mat4  projection = Ortho(-1, 1, -1, 1, -2, 2);

  glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
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
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
