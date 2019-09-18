// Three-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection
// 
// Uses keyboard function for user interaction.
//
// Modified by Prof. David Chelberg to add more interactivity
// And to clean up the code and add more comments.
//
// last-modified: Wed Sep 16 12:58:04 2015
// 
#include <Angel.h>

// Global user variables
int NumPoints = 1000;  // Global to hold how many points we want to generate
vec3 *points;          // Global to hold the generated points
		       // (dynamically allocated).
bool rot = false;      // controls whether object is rotated.

// Global OpenGL Variables
GLuint buffer; // Identity of buffer object
GLuint vao;    // Identity of Vertex Array Object
GLuint loc;    // Identity of location of vPosition in shader storage

//----------------------------------------------------------------------------
// Start with a tetrahedron.  Pick any point inside the tetrahedron, and
// find the point halfway towards any vertex (draw this point), and
// repeat with this new point as the point inside the tetrahedron until
// you have sufficient points.
//
void generate_points ()
{
  points = new vec3[NumPoints];
  // Specifiy the vertices for a tetrahedron
  vec3 vertices[4] = {vec3(-1.0, -1.0, 0.0), 
		      vec3( 0.0,  1.0, 0.0), 
		      vec3( 1.0, -1.0, 0.0),
		      vec3( 0.5,  0.5, 1.0)
  };

  // Select an arbitrary initial point inside of the tetrahedron
  points[0] = vec3(0.25, 0.50, 0.5);

  // compute and store N-1 new points
  for (int i = 1; i < NumPoints; ++i) {
    int j = rand() % 4;   // pick a vertex at random

    // Compute the point halfway between the selected vertex
    //   and the previous point
    points[i] = (points[i - 1] + vertices[j]) / 2.0;
  }
}

// Set up shaders, etc.
void init()
{
  // Create the points array with all the points in it.
  generate_points();

  // Create a vertex array object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
    
  // Create and initialize a buffer object
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, NumPoints*sizeof(vec3), points, GL_STATIC_DRAW);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshaderSimple.glsl", "fshader21.glsl");
  glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

//----------------------------------------------------------------------------
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);          // clear the window
  glDrawArrays(GL_POINTS, 0, NumPoints); // draw the points
  glutSwapBuffers();
}

//----------------------------------------------------------------------------
// Callback to process normal keyboard characters entered.
// 
extern "C" void keyboard(unsigned char key, int x, int y)
{
  switch (key) {

  case 'h':
    // Help -- give instructions
    std::cout << "Commands are p/P for point size, +/- for number of points" << std::endl;
    break;

    // Escape key (quit)
  case 033:
  case 'Q':
  case 'q':
    exit(EXIT_SUCCESS);
    break;

  case 'p':
    // Make points small
    // The next line may make the code draw faster
    //    glDisable(GL_POINT_SMOOTH);
    glPointSize(1.0);
    glutPostRedisplay();
    break;
  case 'P':
    // Make points big
    glEnable(GL_POINT_SMOOTH);
    // In order to enable antialiasing, we need the following two lines
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glPointSize(2.0);
    glutPostRedisplay();
    break;

  case '+':
    // Increase number of points.
    NumPoints*=2;
    // Let the user know how many points are being displayed
    std::cout << "NumPoints = " << NumPoints << std::endl;
    // Reclaim memory for current points array
    delete points;
    generate_points();

    // Send the new points to the GPU
    // Next line isn't necessary, but doesn't hurt anything either.
    glBufferData(GL_ARRAY_BUFFER, NumPoints*sizeof(vec3), points, GL_STATIC_DRAW);      
    glutPostRedisplay();

    break;
  case '-':
    // Decrease number of points.
    NumPoints/=2;
    // Be sure we don't go to 0.
    if (NumPoints <1){
      NumPoints=1;
    }
    // Let the user know how many points are being displayed
    std::cout << "NumPoints = " << NumPoints << std::endl;
    // Reclaim memory for current points array
    delete points;
    generate_points();

    // Send the new points to the GPU
    glBufferData(GL_ARRAY_BUFFER, NumPoints*sizeof(vec3), points, GL_STATIC_DRAW);      
    glutPostRedisplay();

    break;

  case 'r':
    rot = !rot;
    break;
  default:
    // Do nothing.
    break;
  }
}

void idle()
{
  if (rot) {
    // Code to do rotation here (rotate about Y axis for starters)
    for (size_t i=0;i<(size_t)NumPoints;++i) {
      vec4 temp = RotateY(1)*vec4(points[i].x, points[i].y, points[i].z, 1);
      points[i].x=temp.x;
      points[i].y=temp.y;
      points[i].z=temp.z;
    }
    glBufferData(GL_ARRAY_BUFFER, NumPoints*sizeof(vec3), points, GL_STATIC_DRAW);      
    glutPostRedisplay();
  }
}

//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  // Make sure the random number generator gets different values each
  // time the program is run.
  srand(time(NULL));

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(512, 512);

  glutCreateWindow("Sierpinski Gasket");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return EXIT_SUCCESS;
}
