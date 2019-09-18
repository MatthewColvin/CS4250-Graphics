// Modified by David M. Chelberg for style
// last-modified: Mon Nov  7 11:31:26 2016
//
// rotating cube with lighting (note this version has no specular
// lighting, see cube0a to see specular lighting)
//
// Modified with more comments -- DMC
// Modified for better style -- DMC (note, still many parts poor
//    style)
//

// Shading by computing lighting equation at vertices in the
// application

#include "Angel.h"

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

typedef Angel::vec4  point4;
typedef Angel::vec4  color4;

// Adjust this value for your taste (to speed up, make bigger, to slow
// down rotation, make smaller (this is 0.06 degrees per millisecond)
GLfloat incr =0.06;

int axis = 0;                       // Which axis to rotate about
GLfloat theta[3] = {0.0, 0.0, 0.0}; // The angle to rotate about each axis.

// 
GLuint buffers[1];// No idea why the Angel book has an array of one
		  // element here.

GLuint loc, loc2; // Position and Color locations

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

vec4 viewer = vec4(0.0, 0.0, -1.0, 0.0);
point4 light_position = point4(0.0, 0.0, -1.0, 0.0);

color4 light_ambient = color4(0.2, 0.2, 0.2, 1.0);
//color4 light_ambient = color4(1.0, 1.0, 1.0, 1.0);

color4 light_diffuse = color4(1.0, 1.0, 1.0, 1.0);
//color4 light_diffuse = color4(0.0, 0.0, 0.0, 1.0);

color4 light_specular = color4(1.0, 1.0, 1.0, 1.0);
// NOTE: NO SPECULAR LIGHTING to start!!
bool spec=false;

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
     
  // Compute lighting equation for entire quad with the same lighting
  // and viewing direction for all four vertices.

  // Compute normal vector.
  vec3 n1 = normalize(cross(vertices[b] - vertices[a], 
			    vertices[c] - vertices[b]));
  //  vec3 n1 = normalize(cross(ctm*vertices[b] - ctm*vertices[a], 
  //		   	        ctm*vertices[c] - ctm*vertices[b]));

  // transform normal vector by ctm (Current transform matrix)
  vec4 n = ctm*vec4(n1[0], n1[1], n1[2], 0.0);

  // Compute the half vector for shading.
  vec4 half = normalize(light_position+viewer);
  half.w=0.0;

  // Variables to keep track of the separate contributions of each
  // type of light.
  color4 ambient_color, diffuse_color, specular_color;

  // Compute the ambient product
  ambient_color = product(material_ambient, light_ambient);

  // Compute the diffuse lighting equation
  float dd = dot(light_position, n);
  if(dd > 0.0) {
    diffuse_color = dd*product(light_diffuse, material_diffuse);
  } else {
    diffuse_color =  color4(0.0, 0.0, 0.0, 1.0); 
  }

  // Compute specular lighting
  dd = dot(half, n);
  if(dd > 0.0) {
    // next line computes power function.
    specular_color = exp(material_shininess*log(dd)) * 
                     product(light_specular, material_specular); 
  } else {
    specular_color = vec4(0.0, 0.0, 0.0, 1.0); 
  }
  // If we don't want specular color added
  if (!spec) {
    specular_color = vec4(0.0, 0.0, 0.0, 1.0); 
  }
  // Compute colors for vertices, and the positions of the points
  // based on ctm
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

  // Calculate the transformation matrix.
  ctm = RotateX(theta[0])*RotateY(theta[1])*RotateZ(theta[2]);

  // Recalculate the position of all the points.
  colorcube();

  // Send them.
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);

  // Is the next line necessary?
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(quad_color), quad_color);

  // Draw the object
  glDrawArrays(GL_TRIANGLES, 0, NumVertices); 
  glutSwapBuffers();
}

extern "C" void mouse(int btn, int state, int x, int y)
{
  // Change which axis we are rotating about based on mouse press.
  if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
  if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
  if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void spinCube()
{
  static GLint time=glutGet(GLUT_ELAPSED_TIME);
  // Rotate based on how much time has actually passed.
  theta[axis] += incr*(glutGet(GLUT_ELAPSED_TIME)-time);
  time = glutGet(GLUT_ELAPSED_TIME);

  // Take care of wrap around.
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
  glutCreateWindow("Color Cube0");
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
