//
// Perspective view of several objects using LookAt() and Perspective()
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.
//
// Modified by David Chelberg
// last-modified: Fri Nov  1 11:42:30 2019
// Added classes for sphere, cube
// Many mods throughout, including using vectors for points, colors,
// and normals.
//
// last-modified: Fri Oct 28 14:06:29 2016
// Added an animated rotation to the code.
// Separated the transform for the camera (camera_view) from the
//   object (model_view).
// Added code to draw a "door"
// Added code to draw a moving second cube (animated moving back and
//   forth in a straight line).
// Added code to account for elapsed time in the animations.
//
// Modified by David Chelberg
// last-modified: Wed Oct  2 10:54:10 2019
// Moved more common code to common.cc and common.h

#include <Angel.h>
#include "common.h"
#include "cube.h"
#include "cube_door.h"
#include "sphere.h"

// Implementing a door that opens by swinging
GLfloat doorAngle = 0.0;       // Angle of cube's door's rotation.
GLfloat doorAngleIncr = 30.0;   // Amount to increment Angle of cube's
			       // door's rotation.
GLfloat trans = 0.0;   // trans of 2nd cube.
GLfloat transinc = 1.0;// trans increment of 2nd cube.

cube_door mycube;
cube mycube2;
sphere mysphere;

//----------------------------------------------------------------------------
// OpenGL initialization
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

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader41.glsl", "fshader41.glsl");
  glUseProgram(program);

  model_view = glGetUniformLocation(program, "model_view");
  camera_view = glGetUniformLocation(program, "camera_view");
  projection = glGetUniformLocation(program, "projection");
  shade_loc = glGetUniformLocation(program, "shade");
    
  // First set up all the models
  vector<vec4> colors1 {vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1), 
                        vec4(1,1,0,1), vec4(1,0,1,1), vec4(0,1,1,1)};
  mycube.init(colors1,model_view,0, false);

  vector<vec4> colors2{vec4(0.5,0.5,0,1), vec4(0,0.5,0,1), vec4(0,0,0.5,1),
                       vec4(0.5,0.5,0,1), vec4(0.5,0,0.5,1), vec4(0,0.5,0.5,1)};
  mycube2.init(colors2,model_view,points.size(), false);
  
  mysphere.init(vec4(1.0, 0.5, 0.1, 1), model_view, points.size(), false);

  // Now send the data to the GPU
  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  GLuint vColor = glGetAttribLocation(program, "vColor"); 
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(points.size()*sizeof(vec4)));
  glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(vec4) + points.size()*sizeof(vec4),
	       NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, points.size()*sizeof(vec4), points[0]);
  glBufferSubData(GL_ARRAY_BUFFER, points.size()*sizeof(vec4), colors.size()*sizeof(vec4), colors[0]);

  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0); 
}

//----------------------------------------------------------------------------
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  point4  eye(mvx+radius*sin(theta)*cos(phi),
	      mvy+radius*sin(theta)*sin(phi),
	      mvz+radius*cos(theta),
	      1.0);
  point4  at(0, 0, 0, 1.0);
  vec4    up(0.0, 1.0, 0.0, 0.0);

  mat4  cv = LookAt(eye, at, up);
  glUniformMatrix4fv(camera_view, 1, GL_TRUE, cv);

  mat4  p = Perspective(fovy, aspect, zNear, zFar);
  glUniformMatrix4fv(projection, 1, GL_TRUE, p);

  glUniform1i(shade_loc, false);

  mat4 mv = RotateZ(angle);
  mycube.set_mv(mv);
  mycube.draw();

  mv = Translate(0, 4.2,0)*RotateX(angle)*Scale(1, 2, 3);
  mycube2.set_mv(mv);
  mycube2.draw();

  glUniform1i(shade_loc, true);
  mv = Translate(0, -5.2,0)*RotateX(angle)*Scale(1, 4, 2);
  mysphere.set_mv(mv);
  mysphere.draw();

  glutSwapBuffers();
}

// Simple animation
GLint lasttime=0;
extern "C" void cube2idle()
{
  // Added code to account for glutElapsedTime
  GLint time = glutGet(GLUT_ELAPSED_TIME);

  // Code to animate cube goes here.
  if (rotatep) {
    angle+=30.0/1000.0*(time-lasttime);
  }

  doorAngle+=doorAngleIncr/1000.0*(time-lasttime);
  if (doorAngle > 60.0) {
    doorAngleIncr*=-1.0;
  }
  if (doorAngle < 0.0){
    doorAngle = 0.0;
    doorAngleIncr*=-1.0;
  }
  mycube.set_doorAngle(doorAngle);

  // Do the animation code here in idle, not in display.
  //  Code for moving 2nd cube
  trans += transinc/1000.0*(time-lasttime);
  if (trans > 1.5) {
    trans = 1.5;
    transinc*=-1;
  }
  if (trans < -1.5) {
    trans = -1.5;
    transinc*=-1;
  }
  lasttime=time;

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
  glutSpecialFunc(special);
  glutIdleFunc(cube2idle);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
