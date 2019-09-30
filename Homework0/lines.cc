// Two-Dimensional Simple Lines
// Written by dmc 
// last-modified: Mon Sep 17 10:56:22 2012
// 
// 
////////////////////////////////////////////////////////
//
//    Homework 0 
//    Matthew Colvin
//    Date Updated : Thur Sep 20 2019
//
////////////////////////////////////////////////////////


#include <iostream>
#include <Angel.h>


const int NumTrianglePoints = 3;
const int NumPlankPoints = 4; 
const int offsettoplankpoints = NumTrianglePoints;
const int NumArrowPoints = 4;
const int offsettoarrowpoints = NumTrianglePoints + NumPlankPoints;
const int offsettotrianglecolor  = NumPlankPoints+ NumTrianglePoints + NumArrowPoints;
int redrawn = 0;
int picturechange = 0;


// Data storage for our geometry
vec3 Points[NumTrianglePoints+
            NumPlankPoints+
            NumArrowPoints];

// This function initializes the particular things for this program
void myinit()
{
  // Triangle
  Points[0]= vec3( 0 , 200, 0);
  Points[1]= vec3(400, -400, 0);
  Points[2]= vec3(-400, -400, 0);
  //Plank 
  Points[3] = vec3 (-800 , 200, 0);
  Points[4] = vec3 (800, 200, 0);
  Points[5] = vec3 (800, 300, 0);
  Points[6] = vec3 (-800, 300, 0);
  // Arrow
  Points[7] = vec3(0, 400, 0);
  Points[8] = vec3(400, 450, 0);
  Points[9] = vec3(800,600, 0);
  Points[10] = vec3(60,80, 0);
  //Triangle Color
  Points[11] = vec3(1.0, 0.5, 0.2);
  Points[12] = vec3(0.8, 0.5, 0.8);
  Points[13] = vec3(1.0, 0.8, 0.9);

  // attributes
  glClearColor(0.9, 0.8, 0.8, 1.0); // light grey background

}

// This function initializes the buffers and shaders
void init()
{ 
  // Create and initialize a buffer object for transferring data to
  // the GPU.
  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  
  glBufferData(GL_ARRAY_BUFFER,sizeof(Points),Points,GL_STATIC_DRAW);
  //glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(TrianglePoints),TrianglePoints);
  //glBufferSubData(GL_ARRAY_BUFFER,offsetToPlankPoints,sizeof(PlankPoints),PlankPoints);
  // Load shaders and use the resulting shader program
  
  GLuint program = InitShader("vshader21.glsl", "fshader21.glsl");
  
  // InitShader does the glUseProgram
  // So the following would be redundant
  // glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation(program, "TrianglePos");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  GLuint color = glGetAttribLocation(program,"vColor");
  glEnableVertexAttribArray(color);
  glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(offsettotrianglecolor));
}  

// The only required callback, it must draw everything when called.
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT);          //clear the window
  switch(picturechange%4){
    case 2:glDrawArrays(GL_TRIANGLES, 0, NumTrianglePoints);  // draw the lines
    case 1:glDrawArrays(GL_LINE_LOOP,offsettoplankpoints,NumPlankPoints); 
    case 0:glDrawArrays(GL_TRIANGLE_STRIP,offsettoarrowpoints,NumArrowPoints);
  }
  glFlush();                             // clear buffers
  
  if (redrawn % 10 == 0 ){ // allows the "animation" to be seen more esily 
    picturechange++;
  }
  redrawn++;

}


int main(int argc, char** argv)
{
  // Standard GLUT initialization
  glutInit(&argc,argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA); // default, not needed
  glutInitWindowSize(600,600);                   // 600 x 600 pixel window
  glutInitWindowPosition(0,0);                   // place window top left on display
  glutCreateWindow("Lines Display");             // window title

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
