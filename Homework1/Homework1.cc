#include "Angel.h" //  includes glut and glew
#include <iostream>

void windowsetup();
void setupglutcallbacks();
GLuint loadBMP(const char * imagepath);

//Callbacks for glut 
void display();
void reshape(int newwidth,int newheight);

// global Variables 
    int windowWidth;
    int windowHeight;


int main(int argc, char** argv) {
    glutInit(&argc,argv);  // start Glut
    windowsetup();
    setupglutcallbacks();
    
    

    glutMainLoop();
}



void setupglutcallbacks(){
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    //Draw stuff here

    glFlush();
}

void reshape(int w,int h){
    glViewport(0,0,w,h);

    // set global values to match current window size
    windowWidth = w;
    windowHeight = h;
}

void init()
{
  // Offset of square
  GLint offsetLoc;
  // Size of square
  GLint sizeLoc;
  // Color of square 
  GLint colorLoc;

  // Create a vertex array object on the GPU
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object for transferring data to
  // the GPU.
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader.glsl", "fshader.glsl");

  windowSizeLoc = glGetUniformLocation(program, "windowSize");
  if (windowSizeLoc==-1) {
    std::cerr << "Unable to find windowSize parameter" << std::endl;
  }

  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glUniform2f(offsetLoc, 0.0, 0.0);   

  glBufferData(GL_ARRAY_BUFFER, (Square::NumPoints+Circle::NumPoints)*sizeof(vec2), points, GL_STATIC_DRAW);
}



void windowsetup(){
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // RGB and double buffering in glut
    glutInitWindowPosition(200,200);
    glutInitWindowSize(500,500);
    glutCreateWindow("Homework1");

    glClearColor(0.0, 0.0, 0.0, 1.0); // set background color to black

}

GLuint loadBMP_custom(const char * imagepath){
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;

    // Open the file
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

}
