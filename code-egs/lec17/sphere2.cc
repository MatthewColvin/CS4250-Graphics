// Fragment shading of sphere model
// Original from Angel.
//
// Modified by David M. Chelberg for style
// Added more accurate normals (one per vertex).
// 
// Substantially modified (by DMC) to be able to control more aspects
// of the lighting model.  Look at the key function to see what you
// can control.
//
// last-modified: Fri Oct 31 13:06:41 2014
//

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

GLfloat rot_incr = 0.003;  // angle to increment rotation by each idle tick.
GLfloat rotation = 0.0;    // angle of rotation of view.

GLfloat camera_angle=45.0; // Camera's angle of view in degrees
GLfloat zNear;             // Camera's near clipping plane
GLfloat zFar;              // Camera's far clipping plane
GLfloat aspect;            // Window's aspect ratio (updated in reshape)

mat4 model_view;           // Matrix for sphere's transform

// Variables to control the amount of each light type
GLfloat ambient = 0.2;
GLfloat diffuse = 0.7;
GLfloat specular = 0.7;
GLfloat material_shininess = 5.0;
GLint emmissivep;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// Globals to control lighting with above.
color4 ambient_product;
color4 diffuse_product;
color4 specular_product;
color4 emmissiveColor;

// Where is the light?
point4 light_position;

// Uniform variable positions to be able to change them.
GLint ambientLoc;
GLint diffuseLoc;
GLint specularLoc;
GLint shineLoc;
GLint lightPos;
GLint emmisiveLoc;
GLint emmisiveColorLoc;

point4 points[NumVertices];
vec4   normals[NumVertices];
//vec2   tex[NumVertices];

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

//----------------------------------------------------------------------------

int Index = 0;

void triangle(const point4& a, const point4& b, const point4& c)
{
  normals[Index] = a;  normals[Index].w=0.0;  points[Index] = a;  Index++;
  normals[Index] = b;  normals[Index].w=0.0;  points[Index] = b;  Index++;
  normals[Index] = c;  normals[Index].w=0.0;  points[Index] = c;  Index++;
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
  light_position=point4(0.0, 0.0, 2.0, 1.0);
  color4 light_ambient(1.0, 1.0, 1.0, 1.0);
  color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
  color4 light_specular(1.0, 1.0, 1.0, 1.0);

  color4 material_ambient(1.0, 1.0, 0.0, 1.0);
  // Pumpkin color
  color4 material_diffuse(1.0, 117.0/255.0, 24.0/255.0, 1.0);
  //  color4 material_specular(1.0, 117.0/255.0, 24.0/255.0, 1.0);
  color4 material_specular(1.0, 1.0, 1.0, 1.0);
  emmissiveColor = light_diffuse;

  ambient_product = light_ambient * material_ambient;
  diffuse_product = light_diffuse * material_diffuse;
  specular_product = light_specular * material_specular;

  ambientLoc = glGetUniformLocation(program, "AmbientProduct");
  glUniform4fv(ambientLoc, 1, ambient * ambient_product);
  diffuseLoc = glGetUniformLocation(program, "DiffuseProduct");
  glUniform4fv(diffuseLoc, 1, diffuse * diffuse_product);
  specularLoc = glGetUniformLocation(program, "SpecularProduct");
  glUniform4fv(specularLoc, 1, specular * specular_product);
	
  lightPos = glGetUniformLocation(program, "LightPosition");
  glUniform4fv(lightPos, 1, light_position);

  shineLoc = glGetUniformLocation(program, "Shininess");
  glUniform1f(shineLoc, material_shininess);
		 
  emmisiveLoc = glGetUniformLocation(program, "EmmissiveP");
  glUniform1i(emmisiveLoc, 0);
  
  emmisiveColorLoc = glGetUniformLocation(program, "EmmissiveColor");
  glUniform4fv(emmisiveColorLoc, 1, emmissiveColor);


  // Retrieve transformation uniform variable locations
  ModelView = glGetUniformLocation(program, "ModelView");
  Projection = glGetUniformLocation(program, "Projection");
    
  glEnable(GL_DEPTH_TEST);
    
  glClearColor(135.0/255.0, 206.0/255.0, 250.0/255.0, 1.0); // sky blue background
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

  model_view = LookAt(eye, at, up);
  glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

  glutPostRedisplay();
}

//----------------------------------------------------------------------------
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniform1i(emmisiveLoc, 0);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);

  mat4 model2_view = model_view*Translate(light_position.x, light_position.y, light_position.z)*Scale(0.1, 0.1, 0.1);
  glUniformMatrix4fv(ModelView, 1, GL_TRUE, model2_view);
  glUniform1i(emmisiveLoc, 1);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);

  glutSwapBuffers();
}

//----------------------------------------------------------------------------
extern "C" void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case 033: // Escape Key
  case 'q': 
  case 'Q':
    exit(EXIT_SUCCESS);
    break;

  // Modify camera's angle of view
  case '>': {
    camera_angle += 5.0;
    if (camera_angle > 175.0) {
      camera_angle = 175.0;
    }
    mat4 projection = Perspective(camera_angle, aspect, zNear, zFar);

    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
    glutPostRedisplay();
  }
    break;
  case '<':{
    camera_angle -= 5.0;
    if (camera_angle < 1.0) {
      camera_angle = 1.0;
    }
    mat4 projection1 = Perspective(camera_angle, aspect, zNear, zFar);
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection1);
    glutPostRedisplay();
  }
    break;
    
  // Modify the position of the light source to be nearer or farther
  // from the object
  case 'l':
    light_position.z*=0.9;
    glUniform4fv(lightPos, 1, light_position);
    break;
  case 'L':
    light_position.z*=1.1;
    glUniform4fv(lightPos, 1, light_position);
    break;

  // Modify the shininess of the object
  case 'p':
    material_shininess-= 1.0;
    if (material_shininess < 1.0) {
      material_shininess = 1.0;
    }
    glUniform1f(shineLoc, material_shininess);
    break;
  case 'P':
    material_shininess+= 1.0;
    glUniform1f(shineLoc, material_shininess);
    break;

  // Make the sphere rotate faster/slower
  case '+':
    rot_incr *= 1.4;
    break;
  case '-':
    rot_incr /= 1.4;
    break;

  // Stop/Start the rotation of the sphere
  case ' ':
    if (rot_incr == 0.0) {
      rot_incr = 0.003;
    } else {
      rot_incr = 0.0;
    }
    break;

  // Make the ambient light brighter/dimmer
  case 'a':
    ambient/=1.1;
    break;
  case 'A':
    ambient *=1.1;
    if (ambient > 1.0) {
      ambient = 1.0;
    }
    break;

  // Make the diffuse light brighter/dimmer
  case 'd':
    diffuse*=0.9;
    break;
  case 'D':
    diffuse *=1.1;
    if (diffuse > 1.0) {
      diffuse = 1.0;
    }
    break;

  // Make the specular light brighter/dimmer
  case 's':
    specular*=0.9;
    break;
  case 'S':
    specular *=1.1;
    if (specular > 1.0) {
      specular = 1.0;
    }
    break;
  }
  
  // Pass them to the shader.
  glUniform4fv(ambientLoc, 1, ambient*ambient_product);
  glUniform4fv(diffuseLoc, 1, diffuse*diffuse_product);
  glUniform4fv(specularLoc, 1, specular*specular_product);
}

//----------------------------------------------------------------------------
extern "C" void reshape(int width, int height)
{
  glViewport(0, 0, width, height);

  GLfloat left = -2.0, right = 2.0;
  GLfloat top = 2.0, bottom = -2.0;

  // Use following for ortho projection
  //  GLfloat zNear = -20.0, zFar = 20.0;

  // Use following for perspective projection
  zNear = 0.2;
  zFar = 40.0;

  aspect = GLfloat(width)/height;

  if (aspect > 1.0) {
    left *= aspect;
    right *= aspect;
  }
  else {
    top /= aspect;
    bottom /= aspect;
  }

  mat4 projection = Perspective(camera_angle, aspect, zNear, zFar);

  // Can use either perspective or ortho projection.
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
