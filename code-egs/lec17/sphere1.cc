// fragment shading of sphere model
//
// Modified by David M. Chelberg for style
// last-modified: Tue Oct 16 16:24:38 2012

#include "Angel.h"

const int NumTimesToSubdivide = 5;
const int NumTriangles        = 4096;  // (4 faces)^(NumTimesToSubdivide + 1)
const int NumVertices         = 3 * NumTriangles;

/*
// What effect will the following have?
const int NumTimesToSubdivide = 2;
const int NumTriangles        = 64;  // (4 faces)^(NumTimesToSubdivide + 1)
const int NumVertices         = 3 * NumTriangles;
*/
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

GLfloat rot_incr = 0.3;  // angle to increment rotation by each idle tick.
GLfloat rotation = 0.0; // angle of rotation of view.

point4 points[NumVertices];
vec4   normals[NumVertices];

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

//----------------------------------------------------------------------------
int Index = 0;

void triangle(const point4& a, const point4& b, const point4& c)
{
  vec4  normal = normalize(cross(b - a, c - b));

  normals[Index] = normal;  points[Index] = a;  Index++;
  normals[Index] = normal;  points[Index] = b;  Index++;
  normals[Index] = normal;  points[Index] = c;  Index++;
}

//----------------------------------------------------------------------------
point4 unit(const point4& p)
{
  float len = p.x*p.x + p.y*p.y + p.z*p.z;
    
  point4 t;
  if (len > DivideByZeroTolerance) {
    t = p / sqrt(len);
    t.w = 1.0;
  }

  return t;
}

void divide_triangle(const point4& a, const point4& b,
		     const point4& c, int count)
{
  if (count > 0) {
    point4 v1 = unit(a + b);
    point4 v2 = unit(a + c);
    point4 v3 = unit(b + c);
    divide_triangle( a, v1, v2, count - 1);
    divide_triangle( c, v2, v3, count - 1);
    divide_triangle( b, v3, v1, count - 1);
    divide_triangle(v1, v3, v2, count - 1);
  }
  else {
    triangle(a, b, c);
  }
}

void tetrahedron(int count)
{
  point4 v[4] = {
    vec4(0.0, 0.0, 1.0, 1.0),
    vec4(0.0, 0.942809, -0.333333, 1.0),
    vec4(-0.816497, -0.471405, -0.333333, 1.0),
    vec4(0.816497, -0.471405, -0.333333, 1.0)
  };

  divide_triangle(v[0], v[1], v[2], count);
  divide_triangle(v[3], v[2], v[1], count);
  divide_triangle(v[0], v[3], v[1], count);
  divide_triangle(v[0], v[2], v[3], count);
}

//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{
  // Subdivide a tetrahedron into a sphere
  tetrahedron(NumTimesToSubdivide);

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
  GLuint program = InitShader("vshader56.glsl", "fshader56.glsl");
  glUseProgram(program);
	
  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  GLuint vNormal = glGetAttribLocation(program, "vNormal"); 
  glEnableVertexAttribArray(vNormal);
  glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)));

  // Initialize shader lighting parameters
  point4 light_position(0.0, 0.0, 2.0, 0.0);
  color4 light_ambient(0.2, 0.2, 0.2, 1.0);
  color4 light_diffuse(0.7, 0.7, 0.7, 1.0);
  color4 light_specular(1.0, 1.0, 1.0, 1.0);

  color4 material_ambient(1.0, 0.0, 1.0, 1.0);
  color4 material_diffuse(1.0, 117.0/255.0, 24.0/255.0, 1.0);
  color4 material_specular(1.0, 117.0/255.0, 24.0/255.0, 1.0);
  float  material_shininess = 5.0;

  color4 ambient_product = light_ambient * material_ambient;
  color4 diffuse_product = light_diffuse * material_diffuse;
  color4 specular_product = light_specular * material_specular;

  glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
	       1, ambient_product);
  glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
	       1, diffuse_product);
  glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
	       1, specular_product);
	
  glUniform4fv(glGetUniformLocation(program, "LightPosition"),
	       1, light_position);

  glUniform1f(glGetUniformLocation(program, "Shininess"),
	      material_shininess);
		 
  // Retrieve transformation uniform variable locations
  ModelView = glGetUniformLocation(program, "ModelView");
  Projection = glGetUniformLocation(program, "Projection");
    
  glEnable(GL_DEPTH_TEST);
    
  glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

//----------------------------------------------------------------------------
extern "C" void idle()
{
  point4 at(0.0, 0.0, 0.0, 1.0);
  vec4   up(0.0, 1.0, 0.0, 0.0);

  static int last_time = glutGet(GLUT_ELAPSED_TIME);
  
  int delta_time = glutGet(GLUT_ELAPSED_TIME) - last_time;
  last_time = glutGet(GLUT_ELAPSED_TIME);

  rotation += rot_incr*delta_time/1000.0;
  GLfloat z = 4.0*cos(rotation);
  GLfloat x = 4.0*sin(rotation);
  point4 eye(x, 0.0, z, 1.0);

  mat4 model_view = LookAt(eye, at, up);
  glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

  glutPostRedisplay();
}

//----------------------------------------------------------------------------
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
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
  case '+':
    rot_incr *= 1.4;
    break;
  case '-':
    rot_incr /= 1.4;
    break;
  case ' ':
    if (rot_incr == 0.0) {
      rot_incr = 0.3;
    } else {
      rot_incr = 0.0;
    }
    break;
  }
}

//----------------------------------------------------------------------------
extern "C" void reshape(int width, int height)
{
  glViewport(0, 0, width, height);

  GLfloat left = -2.0, right = 2.0;
  GLfloat top = 2.0, bottom = -2.0;
  //  GLfloat zNear = -20.0, zFar = 20.0;
  GLfloat zNear = 2.0, zFar = 20.0;

  GLfloat aspect = GLfloat(width)/height;

  if (aspect > 1.0) {
    left *= aspect;
    right *= aspect;
  }
  else {
    top /= aspect;
    bottom /= aspect;
  }

  mat4 projection = Perspective(45.0, aspect, zNear, zFar);
  //  mat4 projection = Ortho(left, right, bottom, top, zNear, zFar);
  glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

//----------------------------------------------------------------------------
int main(int argc, char **argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Sphere");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
