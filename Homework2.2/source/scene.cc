#include <Angel.h>
#include "common.h"
#include "cube.h"
#include "cube_door.h"
#include "sphere.h"
#include "camera.h"
#include "scene.h"

extern "C" void special(int key, int x, int y);
extern "C" void keyboard(unsigned char key, int x, int y);
extern "C" void reshape(int width, int height);
extern "C" void idle();
 
GLfloat incr=0.1;
// For perspective camera
GLfloat fovy=45;
GLfloat aspect=1.0;

GLfloat radius=15.0;
GLfloat angle = 0.0;
GLfloat cameraangle = 0.0;


GLfloat left = -1.0, right = 1.0;
GLfloat bottom = -1.0, top = 1.0;

// Camera and view parameters
GLfloat zNear = 0.1;
GLfloat zFar = 300.0;

GLfloat mvx=0.0;
GLfloat mvy=0.0;
GLfloat mvz=0.0;

bool rotatep=false;          // whether to rotate or not

const GLfloat dr = 90.0 * DegreesToRadians;



GLint  camera_view;// camera-view matrix uniform shader variable location
GLint  model_view; // model-view matrix uniform shader variable locatio
GLint  projection; // projection matrix uniform shader variable location
GLint  shade_loc;  // shade uniform shader variable location



//----------------------------------------------------------------------------
// OpenGL initialization
void init(){
  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("./shaders/vshader41.glsl", "./shaders/fshader41.glsl");
  glUseProgram(program);

  model_view = glGetUniformLocation(program, "model_view");
  camera_view = glGetUniformLocation(program, "camera_view");
  projection = glGetUniformLocation(program, "projection");
  shade_loc = glGetUniformLocation(program, "shade");
    
  // First set up all the models
  vector<vec4> colors1 {
    vec4(1,0,0,1), 
    vec4(0,1,0,1), 
    vec4(0,0,1,1), 
    vec4(1,1,0,1), 
    vec4(1,0,1,1), 
    vec4(0,1,1,1)
  };
  vector<vec4> colors2{
    vec4(0.5,0.5,0,1), 
    vec4(0,0.5,0,1),
    vec4(0,0,0.5,1),
    vec4(0.5,0.5,0,1), 
    vec4(0.5,0,0.5,1), 
    vec4(0,0.5,0.5,1)
  };

  mycube.init(
    colors1,
    model_view,
    0, 
    false
  );
  mycube2.init(
    colors2,
    model_view,
    globalpoints.size(), 
    false
  );
  mysphere.init(
    vec4(1.0, 0.5, 0.1, 1), 
    model_view, 
    globalpoints.size(), 
    false
  );

  // Now send the data to the GPU
  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(
    vPosition, 
    4, 
    GL_FLOAT, 
    GL_FALSE, 
    0,
		BUFFER_OFFSET(0)
  );


  GLuint vColor = glGetAttribLocation(program, "vColor"); 
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(
    vColor, 
    4, 
    GL_FLOAT, 
    GL_FALSE, 
    0,
		BUFFER_OFFSET(globalpoints.size()*sizeof(vec4))
  );
  


  glBufferData(
    GL_ARRAY_BUFFER, 
    globalpoints.size()*sizeof(vec4) + globalpoints.size()*sizeof(vec4),
	  NULL,
    GL_STATIC_DRAW
  );
  glBufferSubData(
    GL_ARRAY_BUFFER,
    0, 
    globalpoints.size()*sizeof(vec4), globalpoints[0]
  );
  glBufferSubData(
    GL_ARRAY_BUFFER, 
    globalpoints.size()*sizeof(vec4), 
    globalcolors.size()*sizeof(vec4), 
    globalcolors[0]
  );

  
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0); 
}

//----------------------------------------------------------------------------
extern "C" void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  point4  eye(0,0,15,1.0);

  point4  at(0,0,3, 1.0);

  vec4    up(0.0, 1.0, 0.0, 0.0);
  mat4  cv = LookAt(eye, at, up);
  
  cv = cv * Translate(mvx,mvy,mvz);
  
  glUniformMatrix4fv(camera_view, 1, GL_TRUE, cv);

  mat4  p = Perspective(fovy, aspect, zNear, zFar) ;

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
extern "C" void cube2idle(){
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
extern "C" void special(int key, int x, int y){
  switch(key) {
  case GLUT_KEY_UP:
    mvz+=incr;
    break;
  case GLUT_KEY_DOWN:
    mvz-=incr;
    break;
  case GLUT_KEY_LEFT:
    mvx+=incr;
    break;
  case GLUT_KEY_RIGHT:
    mvx-=incr;
    break;
  }
}

extern "C" void keyboard(unsigned char key, int x, int y){
  switch(key) {
  case 033: // Escape Key
  case 'q': case 'Q':
    exit(EXIT_SUCCESS);
    break;

    // Speed up/slow down movements
  case '+':
    incr*=2.0;
    break;
  case '-':
    incr/=2.0;
    break;

  case 'x': left *= 1.1; right *= 1.1; break;
  case 'X': left /= 1.1; right /= 1.1; break;
    //  case 'y': bottom *= 1.1; top *= 1.1; break;
    //  case 'Y': bottom /= 1.1; top /= 1.1; break;
  case 'z': zNear  *= 1.1; zFar /= 1.1; break;
  case 'Z': zNear /= 1.1; zFar *= 1.1; break;
  case 'Y':
    mvy+=incr;
    break;
  case 'y':
    mvy-=incr;
    break;

  case 'r': radius *= 1.5; break;
  case 'R': radius /= 1.5; break;

  case 'T': cameraangle += dr; break;
  case 't': cameraangle -= dr; break;

  case 'v': 
    fovy-=5; 
    if (fovy < 0) {
      // Min angle of view 1 degree
      fovy = 1;
    }
    break;
  case 'V': fovy+=5; break;
    if (fovy > 179) {
      // Max angle of view 179 degrees
      fovy = 179;
    }
    break;

  case ' ':  // reset values to their defaults
    rotatep=!rotatep;

    incr=0.1;
    left = -1.0;
    right = 1.0;
    bottom = -1.0;
    top = 1.0;
    mvx = 0.0;
    mvy = 0.0;
    mvz = 0.0;

    break;
  }

  glutPostRedisplay();
}
extern "C" void reshape(int width, int height){
  glViewport(0, 0, width, height);

  aspect = GLfloat(width)/height;
}

extern "C" void idle(){
  // Code to animate cube goes here.
  if (rotatep) {
    angle+=1;
  }
  glutPostRedisplay();
}

//----------------------------------------------------------------------------
int main(int argc, char **argv){
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
