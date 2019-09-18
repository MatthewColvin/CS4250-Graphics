// Textured plane using Athens data
// Prof. Chelberg (modified to allow reading in an external texture
// file in bmp format).
// 
// TODO:
//   Generate points for triangle strips
//   Change how data is passed to include #points*(sizeof(datatype))
// 

#include "Angel.h"
#include "bitmap.h"
#include <iostream>
#include <fstream>
#include <string>

using std::string;
using std::ifstream;
using std::cout;
using std::endl;

// Globals for bitmap texturing
#define NUM_TEXTURES 2
#define TOT_TEXTURES 4
BITMAPINFO *TexInfo[NUM_TEXTURES]; // Texture bitmap information
GLubyte    *TexBits[NUM_TEXTURES]; // Texture bitmap pixel bits 

// Adjust this value for your machine (to speed up, make bigger, to
// slow down rotation, make smaller
GLfloat incr =0.06;
// Should we rotate our world?
bool rot=false;

// Where the viewer is initially
vec3 viewer_pos(-1830.5, -1830.5, -2000.0);

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// Texture objects and storage for texture image
GLuint textures[TOT_TEXTURES];

const int  TextureSize  = 64;
GLubyte image[TextureSize][TextureSize][3];
GLubyte image2[TextureSize][TextureSize][3];

// Vertex data arrays

// Number of total vertices in grid
size_t NumVertices=0;

// Size of input heightmap
size_t Width;
size_t Height;

point4  *points;
vec3    *normals;
vec2    *tex_coords;

// Temporary array to hold height values read in.
float *z_vals;

// Model-view and projection matrices uniform location
GLuint ModelView;
GLuint Projection;

// Array of rotation angles (in degrees) for each coordinate axis
enum {Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};
int      Axis = Zaxis;
GLfloat  Theta[NumAxes] = {0.0, 0.0, 0.0};
//GLuint   theta;

// Uniform to control how texture information is combined with colors.
bool     blend=false;
GLuint   blendLoc;

// Uniform to control animation of wave over terrain.
GLint timeLoc;

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

// Auxilliary function to compute normals for the triangle strips.
void compute_normals()
{
  // The following generates the texture coords, and vertices, normals
  // will be a separate process.

  // Sides of triangles
  vec3 norm;

  //  Deal with first and last rows. For now, just make point up.
  size_t index1=0;
  size_t index2=2*Width*(Height-2)+1;
  for (size_t c=0; c< Width; ++c) {
    normals[index1]=vec3(0, 0, 1);
    index1+=2;
    normals[index2]=vec3(0, 0, 1);
    index2+=2;
  }
  // Deal with first and last columns. For now, just make point up.
  index1=0;
  index2=2*Width-2;
  for (size_t r=0; r<Height-1; ++r) {
    normals[index1]=vec3(0, 0, 1);
    normals[index1+1]=vec3(0, 0, 1);
    index1+=2*Width;
    normals[index2]=vec3(0, 0, 1);
    normals[index2+1]=vec3(0, 0, 1);
    index2+=2*Width;
  }
  size_t index=Width+1;
  for (size_t row=1; row<Height-2; ++row) {
    for (size_t col=1; col< Width-1; ++col) {
      // Note: repeated points between adjacent triangle strips.
      // need to adjust offsets into points array for boundaries
      // between strips.
      // 
      // 
      vec4 a=points[index-1]-points[index];
      vec4 b=points[index-2]-points[index];
      vec4 c=points[index]-points[index];
      vec4 d=points[index]-points[index];
      vec4 e=points[index]-points[index];
      vec4 f=points[index]-points[index];
      norm=cross(a,b) + cross(f,a) + cross(e,f) + cross(d,e) + cross(c,d) + cross(b,c);
      normals[index]=normalize(norm);
      normals[index+Width]=normals[index];
      index+=2;
    }
    // Skip last column, and first column of next row.
    index+=4;
  }
}

      // Average up to 6 nearby normals.
      /*
      if ((r==0) && (c==0)) {
	norm=cross(points[index+1]-points[index],points[index+2]-points[index]);
      } else if ((r==0) && (c==Width-1)) {
	norm=cross(points[index-1]-points[index],points[index+1]-points[index])+
             cross(points[index-2]-points[index],points[index-1]-points[index]);
      } else if ((
      */

// Now we build the terrain model as a set of triangle strips
void terrain_build ()
{
  // Each strip has 2*Width points and we have Height-1 strips
  points     = new vec4[2*Width*(Height-1)];
  normals    = new vec3[2*Width*(Height-1)];
  tex_coords = new vec2[2*Width*(Height-1)];
  
  // Here we need to generate the triangle strips.
  float x=0.0;
  float y=0.0;
  // For AthensMedium
  float dx=3661.0/Width;
  float dy=3308.0/Height;
  float s=0.0;
  float ds=1.0/(Width-1);
  float t=1.0;
  float dt=1.0/(Height-1);
  // How much to exaggerate vertical
  float stretch=2.0;
  // The following generates the texture coords, and vertices, normals
  // will be a separate process.
  for (size_t r=0; r<Height-1; ++r) {
    x=0.0;
    s=0.0;
    for (size_t c=0; c< Width; ++c) {
      // Generate one strip, from i to i+1 rows.
      // One quad (two triangles)
      points[NumVertices]       = vec4(x,    y, stretch*z_vals[r*Width+c], 1);
      tex_coords[NumVertices]   = vec2(s, t);
      points[NumVertices+1]     = vec4(x, y+dy, stretch*z_vals[(r+1)*Width+c], 1);
      tex_coords[NumVertices+1] = vec2(s, t-dt);
      x+=dx;
      s+=ds;
      NumVertices+=2;
    }
    y+=dy;
    t-=dt;
  }
  compute_normals();
}


//----------------------------------------------------------------------------
void init()
{
  z_vals=terrain_read("AthensMedium.txt");
  terrain_build();  

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
  // Check for failure (null pointer returned).
  if (TexBits[0] == 0) {
    std::cout << "Error reading in texture file. Filename =" << "AthensMedium.bmp" << std::endl;
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
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
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
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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
	       sizeof(vec4)*NumVertices +
	       sizeof(vec2)*NumVertices +
	       sizeof(vec3)*NumVertices,
	       //	       sizeof(vec4)*NumVertices + sizeof(vec2)*NumVertices,
	       NULL, GL_STATIC_DRAW);

  // Specify an offset to keep track of where we're placing data in our
  //   vertex array buffer.  We'll use the same technique when we
  //   associate the offsets with vertex attribute pointers.
  GLintptr offset = 0;
  glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vec4)*NumVertices, points);
  offset += sizeof(vec4)*NumVertices;

  glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vec2)*NumVertices, tex_coords);
  offset += sizeof(vec2)*NumVertices;

  glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vec3)*NumVertices, normals);
    

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader74a.glsl", "fshader74.glsl");
  glUseProgram(program);

  // set up vertex arrays
  offset = 0;
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(offset));
  offset += sizeof(vec4)*NumVertices;

  GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
  glEnableVertexAttribArray(vTexCoord);
  glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(offset));
  offset += sizeof(vec2)*NumVertices;

  GLuint vNormal = glGetAttribLocation(program, "vNormal"); 
  glEnableVertexAttribArray(vNormal);
  glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
  			BUFFER_OFFSET(offset));
  offset += sizeof(vec3)*NumVertices;

  timeLoc=glGetUniformLocation(program, "time");
  glUniform1f(timeLoc, 0);

  // Initialize shader lighting parameters
  point4 light_position(0.0, 0.0, -100.0, 1.0);
  color4 light_ambient(0.2, 0.2, 0.2, 1.0);
  color4 light_diffuse(0.8, 0.8, 0.8, 1.0);
  //color4 light_diffuse(0.0, 0.0, 1.0, 1.0);
  color4 light_specular(0.5, 0.5, 0.5, 1.0);
  //color4 light_specular(0.0, 0.0, 0.0, 1.0);

  color4 material_ambient(1.0, 1.0, 1.0, 1.0);
  color4 material_diffuse(1.0, 1.0, 1.0, 1.0);
  color4 material_specular(1.0, 1.0, 1.0, 1.0);
  float  material_shininess = 2.0;

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
		 
  // Set the value of the fragment shader texture sampler variable
  //   ("texture") to the the appropriate texture unit. In this case,
  //   zero, for GL_TEXTURE0 which was previously set by calling
  //   glActiveTexture().
  glUniform1i(glGetUniformLocation(program, "texture"), 0);

  // Retrieve transformation uniform variable locations
  ModelView = glGetUniformLocation(program, "ModelView");
  Projection = glGetUniformLocation(program, "Projection");
  //  theta = glGetUniformLocation(program, "theta");
    
  blendLoc = glGetUniformLocation(program, "blend");

  glUniform1i(blendLoc, blend);

  glEnable(GL_DEPTH_TEST);
    
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

extern "C" void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //  mat4 projection = Ortho(0.0, 3661.0, 0, 3661.0, 1.0, 4500.0);
  mat4 projection = Perspective(50.0, 1.0, 1.0, 45000.0);

  mat4  model_view = (Translate(viewer_pos) *
		      Translate(1830.5, 1830.5, 0.0) *
		      RotateX(Theta[Xaxis]) *
		      RotateY(Theta[Yaxis]) *
		      RotateZ(Theta[Zaxis]) *
		      Translate(-1830.5, -1830.5, 0.0));
    
  glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
  glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

  // Draw the triangle strips (Height-1) of them
  for (size_t strip=0;strip < NumVertices;strip+=Width*2) {
    // mode, first, count
    glDrawArrays(GL_TRIANGLE_STRIP, strip, Width*2);
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
  
  glUniform1f(timeLoc, glutGet(GLUT_ELAPSED_TIME)/1000.0);

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
  case 'B': 
  case 'b':
    // Turn blend on/off
    blend = !blend;
    glUniform1i(blendLoc, blend);
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
  case ' ':
    // Reset Viewing
    viewer_pos=vec3(-1830.5, -1830.5, -2000.0);
    Theta[0]=Theta[1]=Theta[2]=0.0;
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
  glutCreateWindow("Terrain");

  glewInit();

  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
