// Modified by David M. Chelberg for style
// last-modified: Mon Nov  7 11:39:05 2016
//
// rotating cube with lighting

// Comments in this version poor, also many places have poor style
// coding.  -- DMC
// Added some more comments -- DMC

// shades computed at vertices in application

#include "Angel.h"

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

// Adjust this value for your taste (to speed up, make bigger, to
// slow down rotation, make smaller
GLfloat incr =0.06;

int axis = 0;
float theta[3] = {0.0, 0.0, 0.0};

GLuint buffers[2];
GLuint loc, loc2;
GLint matrix_loc;

point4  vertices[8] = {
  point4(-0.5,-0.5,0.5, 1.0),  
  point4(-0.5,0.5,0.5, 1.0),
  point4(0.5,0.5,0.5, 1.0),    
  point4(0.5,-0.5,0.5, 1.0), 
  point4(-0.5,-0.5,-0.5, 1.0), 
  point4(-0.5,0.5,-0.5, 1.0),
  point4(0.5,0.5,-0.5, 1.0),   
  point4(0.5,-0.5,-0.5, 1.0)};


vec4 viewer = vec4(0.0, 0.0, -1.0, 0.0);
point4 light_position = point4(0.0, 0.0, -1.0, 0.0);

color4 light_ambient = color4(0.2, 0.2, 0.2, 1.0);


color4 light_diffuse = color4(1.0, 1.0, 1.0, 1.0);



color4 light_specular = color4(1.0, 1.0, 1.0, 1.0);
// NOTE: SPECULAR LIGHTING to start!!
bool spec=true;

color4 material_ambient = color4(1.0, 0.0, 1.0, 1.0);
color4 material_diffuse = color4(1.0, 0.8, 0.0, 1.0);
color4 material_specular = color4(1.0, 0.8, 0.0, 1.0);
float material_shininess = 100.0;

point4 points[NumVertices];
color4 quad_color[NumVertices];
mat4 ctm;

GLuint program;

// matrix functions

// product of components

vec4 product(vec4 a, vec4 b)
{
  return vec4(a[0]*b[0], a[1]*b[1], a[2]*b[2], a[3]*b[3]);
}


void MyQuad(int a, int b, int c, int d)
{
  static int i =0; 
     
  // Compute normal vector.
  vec3 n1 = normalize(cross(ctm*vertices[b] - ctm*vertices[a], 
			    ctm*vertices[c] - ctm*vertices[b]));
  vec4 n = vec4(n1[0], n1[1], n1[2], 0.0);
  vec4 half = normalize(light_position+viewer);
  half.w=0.0;
  color4 ambient_color, diffuse_color, specular_color;

  ambient_color = product(material_ambient, light_ambient);

  float dd = dot(light_position, n);
  if(dd>0.0) {
    diffuse_color = dd*product(light_diffuse, material_diffuse);
  } else {
    diffuse_color =  color4(0.0, 0.0, 0.0, 1.0);
  }

  dd = dot(half, n);
  if(dd > 0.0) {
    specular_color = exp(material_shininess*log(dd))*product(light_specular, material_specular);
  } else {
    specular_color = vec4(0.0, 0.0, 0.0, 1.0);
  }
  // If we don't want specular color added
  if (!spec) {
    specular_color = vec4(0.0, 0.0, 0.0, 1.0); 
  }

  quad_color[i] = ambient_color + diffuse_color + specular_color;
  points[i] = ctm*vertices[a];
  i++;
  quad_color[i] = ambient_color + diffuse_color + specular_color;
  points[i] = ctm*vertices[b];
  i++;
  quad_color[i] = ambient_color + diffuse_color + specular_color;
  points[i] = ctm*vertices[c];
  i++;
  quad_color[i] = ambient_color + diffuse_color + specular_color;
  points[i] = ctm*vertices[a];
  i++;
  quad_color[i] = ambient_color + diffuse_color + specular_color;
  points[i] = ctm*vertices[c];
  i++;
  quad_color[i] = ambient_color + diffuse_color + specular_color;
  points[i] = ctm*vertices[d];
  i++;
  i%=NumVertices;
}

void colorcube()
{
  MyQuad(1,0,3,2);
  MyQuad(2,3,7,6);
  MyQuad(3,0,4,7);
  MyQuad(6,5,1,2);
  MyQuad(4,5,6,7);
  MyQuad(5,4,0,1);
}


// OpenGL initialization

void init()
{

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // set up vertex buffer object

  glGenBuffers(1, buffers);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(quad_color), NULL, GL_STATIC_DRAW);

  program = InitShader("vshader52.glsl", "fshader52.glsl");
  glUseProgram(program);

  loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  loc2 = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(loc2);
  glVertexAttribPointer(loc2, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

  glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear the window
  ctm = RotateX(theta[0])*RotateY(theta[1])*RotateZ(theta[2]);
  colorcube();

  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(quad_color), quad_color);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices); 
  glutSwapBuffers();
}

extern "C" void mouse(int btn, int state, int x, int y)
{
  if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
  if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
  if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void spinCube()
{
  static GLint time=glutGet(GLUT_ELAPSED_TIME);
  theta[axis] += incr*(glutGet(GLUT_ELAPSED_TIME)-time);
  time = glutGet(GLUT_ELAPSED_TIME);

  if(theta[axis] > 360.0) theta[axis] -= 360.0;
  glutPostRedisplay();
}

extern "C" void mykey(unsigned char key, int mousex, int mousey)
{
  if(key=='q'||key=='Q') {
    exit(0);
  } else if (key=='S'||key=='s') {
    spec=!spec;
  }
}

int main(int argc, char** argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Color Cube0a");
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutIdleFunc(spinCube);
  glutKeyboardFunc(mykey);

  glewInit();

  init();

  glEnable(GL_DEPTH_TEST);
  glutMainLoop();
  return(EXIT_SUCCESS);
}
