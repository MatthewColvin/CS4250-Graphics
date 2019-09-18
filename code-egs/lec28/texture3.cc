// rotating cube with two texture objects
// Prof. Chelberg (modified to allow reading in an external texture
// file in bmp format).
// 

#include "Angel.h"
#include "bitmap.h"

// Globals for bitmap texturing
#define NUM_TEXTURES 2
#define TOT_TEXTURES 4
BITMAPINFO *TexInfo[NUM_TEXTURES]; // Texture bitmap information
GLubyte    *TexBits[NUM_TEXTURES]; // Texture bitmap pixel bits 

const int  NumTriangles = 12; // (6 faces)(2 triangles/face)
const int  NumVertices  = 3 * NumTriangles;
const int  TextureSize  = 64;

// Adjust this value for your machine (to speed up, make bigger, to
// slow down rotation, make smaller
GLfloat incr =0.06;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// Texture objects and storage for texture image
GLuint textures[TOT_TEXTURES];

GLubyte image[TextureSize][TextureSize][3];
GLubyte image2[TextureSize][TextureSize][3];

// Vertex data arrays
point4  points[NumVertices];
color4  quad_colors[NumVertices];
vec2    tex_coords[NumVertices];

// Array of rotation angles (in degrees) for each coordinate axis
enum {Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = {0.0, 0.0, 0.0};
GLuint   theta;

// Uniform to control how texture information is combined with colors.
bool     blend=true;
GLuint   blendLoc;

//----------------------------------------------------------------------------
int Index = 0;

void Myquad(int a, int b, int c, int d)
{
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

  color4 colors[8] = {
    color4(0.0, 0.0, 0.0, 1.0),  // black
    color4(1.0, 0.0, 0.0, 1.0),  // red
    color4(1.0, 1.0, 0.0, 1.0),  // yellow
    color4(0.0, 1.0, 0.0, 1.0),  // green
    color4(0.0, 0.0, 1.0, 1.0),  // blue
    color4(1.0, 0.0, 1.0, 1.0),  // magenta
    color4(0.0, 1.0, 1.0, 1.0),  // white
    color4(1.0, 1.0, 1.0, 1.0)   // cyan
  };

  quad_colors[Index] = colors[a];
  points[Index] = vertices[a];
  tex_coords[Index] = vec2(0.0, 0.0);
  Index++;
    
  quad_colors[Index] = colors[a];
  points[Index] = vertices[b];
  tex_coords[Index] = vec2(0.0, 1.0);
  Index++;
    
  quad_colors[Index] = colors[a];
  points[Index] = vertices[c];
      tex_coords[Index] = vec2(1.0, 1.0);
  //  tex_coords[Index] = vec2(1/2.0, 1/2.0);
  Index++;
    
  quad_colors[Index] = colors[a];
  points[Index] = vertices[a];
  tex_coords[Index] = vec2(0.0, 0.0);
  Index++;
    
  quad_colors[Index] = colors[a];
  points[Index] = vertices[c];
  tex_coords[Index] = vec2(1.0, 1.0);
  //  tex_coords[Index] = vec2(1/2.0, 1/2.0);
  Index++;
    
  quad_colors[Index] = colors[a];
  points[Index] = vertices[d];
  tex_coords[Index] = vec2(1.0, 0.0);
  Index++;
}

//----------------------------------------------------------------------------
void colorcube()
{
  Myquad(1, 0, 3, 2);
  Myquad(2, 3, 7, 6);
  Myquad(3, 0, 4, 7);
  Myquad(6, 5, 1, 2);
  Myquad(4, 5, 6, 7);
  Myquad(5, 4, 0, 1);
}

//----------------------------------------------------------------------------
void init()
{
  colorcube();

  // Create a checkerboard pattern
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 64; j++) {
      GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0)) * 255;
      image[i][j][0]  = c;
      image[i][j][1]  = c;
      image[i][j][2]  = c;
      image2[i][j][0] = c;
      image2[i][j][1] = 0;
      image2[i][j][2] = c;
    }
  }

  // Initialize texture objects
  glGenTextures(4, textures);

  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0,
	       GL_RGB, GL_UNSIGNED_BYTE, image);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0,
	       GL_RGB, GL_UNSIGNED_BYTE, image2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Set which texture subsequent commands will apply to.
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  // Read the image file.
  TexBits[0] = LoadDIBitmap("AthensMedium.bmp", &TexInfo[0]);
  //TexBits[0] = LoadDIBitmap("Me.bmp", &TexInfo[0]);
  // Check for failure (null pointer returned).
  if (TexBits[0] == 0) {
    std::cout << "Error reading in texture file. Filename " << "AthensMedium.bmp" << std::endl;
    exit(EXIT_FAILURE);
  }
  // Send the texture image to the graphics card.
  glTexImage2D(GL_TEXTURE_2D, 0, 3, TexInfo[0]->bmiHeader.biWidth,
	       TexInfo[0]->bmiHeader.biHeight, 0, GL_RGB,
	       GL_UNSIGNED_BYTE, TexBits[0]);
  // Set up texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Set which texture subsequent commands will apply to.
  glBindTexture(GL_TEXTURE_2D, textures[3]);
  // Read the image file.
  TexBits[1] = LoadDIBitmap("Me.bmp", &TexInfo[1]);
  // Check for failure (null pointer returned).
  if (TexBits[1] == 0) {
    std::cout << "Error reading in texture file. Filename =" << "Me.bmp" << std::endl;
    exit(EXIT_FAILURE);
  }
  // Send the texture image to the graphics card.
  glTexImage2D(GL_TEXTURE_2D, 0, 3, TexInfo[1]->bmiHeader.biWidth,
	       TexInfo[1]->bmiHeader.biHeight, 0, GL_RGB,
	       GL_UNSIGNED_BYTE, TexBits[1]);
  // Set up texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER,
	       sizeof(points) + sizeof(quad_colors) + sizeof(tex_coords),
	       NULL, GL_STATIC_DRAW);

  // Specify an offset to keep track of where we're placing data in our
  //   vertex array buffer.  We'll use the same technique when we
  //   associate the offsets with vertex attribute pointers.
  GLintptr offset = 0;
  glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(points), points);
  offset += sizeof(points);

  glBufferSubData(GL_ARRAY_BUFFER, offset,
		  sizeof(quad_colors), quad_colors);
  offset += sizeof(quad_colors);
    
  glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(tex_coords), tex_coords);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader71.glsl", "fshader74a.glsl");
  glUseProgram(program);

  // set up vertex arrays
  offset = 0;
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(offset));
  offset += sizeof(points);

  GLuint vColor = glGetAttribLocation(program, "vColor"); 
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(offset));
  offset += sizeof(quad_colors);

  GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
  glEnableVertexAttribArray(vTexCoord);
  glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(offset));

  // Set the value of the fragment shader texture sampler variable
  //   ("texture") to the the appropriate texture unit. In this case,
  //   zero, for GL_TEXTURE0 which was previously set by calling
  //   glActiveTexture().
  glUniform1i(glGetUniformLocation(program, "texture"), 0);

  theta = glGetUniformLocation(program, "theta");
    
  blendLoc = glGetUniformLocation(program, "blend");

  glUniform1i(blendLoc, blend);

  glEnable(GL_DEPTH_TEST);
    
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

extern "C" void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniform3fv(theta, 1, Theta);

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
extern "C" void keyboard(unsigned char key, int mousex, int mousey)
{
  switch(key) {
  case 033: // Escape Key
  case 'q': 
  case 'Q':
    exit(EXIT_SUCCESS);
    break;
  case 'B': 
  case 'b':
    // Turn blend on/off
    blend = !blend;
    glUniform1i(blendLoc, blend);
    break;
  case '+':
    incr*=1.5;
    break;
  case '-':
    incr/=1.5;
    break;
  case ' ':
    if (incr!=0) {
      incr = 0;
    } else {
      incr = 0.06;
    }
    break;
  case '1':
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    break;

  case '2':
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    break;
  case '3':
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    break;
  case '4':
    glBindTexture(GL_TEXTURE_2D, textures[3]);
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
  glutCreateWindow("Color Cube");

  glewInit();

  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
