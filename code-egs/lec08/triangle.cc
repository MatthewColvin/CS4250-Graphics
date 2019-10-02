// Two-Dimensional Simple Triangle
// Written by dmc 
// last-modified: Wed Sep 19 11:20:44 2012
//
// Designed to demonstrate smooth shading of vertex colors 
// by using a varying parameter in the vertex shader

#include <iostream>
#include <Angel.h>





// How many points are needed to specify the triangle
// Why 6?
const int NumPoints = 6;

// Data storage for our geometry for the triangles
vec3 points[NumPoints];

GLuint loadBMP_custom(const char * imagepath)
{
// Data read from the header of the BMP file
unsigned char header[54]; // Each BMP file begins by a 54-bytes header
unsigned int dataPos;     // Position in the file where the actual data begins
unsigned int width, height;
unsigned int imageSize;   // = width*height*3
// Actual RGB data
unsigned char * data;

FILE * file = fopen(imagepath,"rb");
if (!file){printf("Image could not be opened\n"); return 0;}

if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
    printf("Not a correct BMP file\n");
    return false;
}

if ( header[0]!='B' || header[1]!='M' ){
    printf("Not a correct BMP file\n");
    return 0;
}

// Read ints from the byte array
dataPos    = *(int*)&(header[0x0A]);
imageSize  = *(int*)&(header[0x22]);
width      = *(int*)&(header[0x12]);
height     = *(int*)&(header[0x16]);

// Some BMP files are misformatted, guess missing information
if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
if (dataPos==0)      dataPos=54; // The BMP header is done that way

// Create a buffer
data = new unsigned char [imageSize];

// Read the actual data from the file into the buffer
fread(data,1,imageSize,file);

//Everything is in memory now, the file can be closed
fclose(file);

// Create one OpenGL texture
GLuint textureID;
glGenTextures(1, &textureID);

// "Bind" the newly created texture : all future texture functions will modify this texture
glBindTexture(GL_TEXTURE_2D, textureID);

// Give the image to OpenGL
glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

return(textureID);

}



// This function initializes the particular things for this program
void myinit()
{
  // First points defined:
  points[0]=vec3(  0.0,   0.0, 0.0);
  points[1]=vec3(500.0,   0.0, 0.0);
  points[2]=vec3(  0.0, 500.0, 0.0);
  // Now colors defined:
  points[3]=vec3(1, 0, 0); // red
  points[4]=vec3(1, 1, 0); // yellow
  points[5]=vec3(0, 0, 1); // blue

  GLuint Texture = loadBMP_custom("dog.bmp");


  // attributes
  glClearColor(0.1, 0.1, 0.1, 0.5); // dark background */
  //glClearColor(0.8, 0.8, 0.8, 0.5); // light grey background
}

// This function initializes the buffers and shaders
void init()
{
  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
    
  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshaderTriangle.glsl", "fshaderTriangle.glsl");
  glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  // Initialize the colors for the vertices for the fragment shader.
  GLuint col=  glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(col);

  // 3 points, each is 3 floating point coordinates after which is the
  // color data.
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(3*3*sizeof(float)));

  // Or you could use the following (since the color data is halfway through the vector):
  /*
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)/2));
  */
}  

// The only required callback, it must draw everything when called.
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT);          // clear the window
  glDrawArrays(GL_TRIANGLES, 0, 3);      // draw the triangles
  glFlush();                             // clear buffers
}

int main(int argc, char** argv)
{
  // Standard GLUT initialization
  glutInit(&argc,argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA); // default, not needed
  glutInitWindowSize(600,600);                   // 600 x 600 pixel window
  glutInitWindowPosition(0,0);                   // place window top left on display
  glutCreateWindow("Triangle Display");         // window title

  // Add callbacks here, only required is displayFunc
  glutDisplayFunc(display);                      // display callback
						 // invoked when
						 // window opened 

  // Initialize the "magic" that glues all the code together.
  glewInit();

  myinit();             // set attributes
  init();               // set up shaders and display environment
  glutMainLoop();       // enter event loop

  return(EXIT_SUCCESS); // return successful exit code
}
