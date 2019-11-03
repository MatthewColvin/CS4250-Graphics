#include "scene.h"

Scene scene;

extern "C" void display();
extern "C" void cube2idle();
extern "C" void special(int key, int x, int y);
extern "C" void keyboard(unsigned char key, int x, int y);
extern "C" void reshape(int width, int height);
extern "C" void idle();


// Simple animation
GLint lasttime=0;
extern "C" void cube2idle(){
  // Added code to account for glutElapsedTime
  GLint time = glutGet(GLUT_ELAPSED_TIME);

  // Code to animate cube goes here.
  if (scene.rotatep) {
    scene.angle+=30.0/1000.0*(time-lasttime);
  }

  scene.doorAngle+=scene.doorAngleIncr/1000.0*(time-lasttime);
  if (scene.doorAngle > 60.0) {
    scene.doorAngleIncr*=-1.0;
  }
  if (scene.doorAngle < 0.0){
    scene.doorAngle = 0.0;
    scene.doorAngleIncr*=-1.0;
  }
  //scene.mycube->set_doorAngle(scene.doorAngle);

  // Do the animation code here in idle, not in display.
  //  Code for moving 2nd cube
  scene.trans += scene.transinc/1000.0*(time-lasttime);
  if (scene.trans > 1.5) {
    scene.trans = 1.5;
    scene.transinc*=-1;
  }
  if (scene.trans < -1.5) {
    scene.trans = -1.5;
    scene.transinc*=-1;
  }
  lasttime=time;

  glutPostRedisplay();
}
extern "C" void special(int key, int x, int y){
  switch(key) {
  case GLUT_KEY_UP:
    scene.mvz+=scene.incr;
    break;
  case GLUT_KEY_DOWN:
    scene.mvz-=scene.incr;
    break;
  case GLUT_KEY_LEFT:
    scene.mvx+=scene.incr;
    break;
  case GLUT_KEY_RIGHT:
    scene.mvx-=scene.incr;
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
    scene.incr*=2.0;
    break;
  case '-':
    scene.incr/=2.0;
    break;

  case 'x': scene.left *= 1.1; scene.right *= 1.1; break;
  case 'X': scene.left /= 1.1; scene.right /= 1.1; break;
    //  case 'y': bottom *= 1.1; top *= 1.1; break;
    //  case 'Y': bottom /= 1.1; top /= 1.1; break;
  case 'z': scene.zNear  *= 1.1; scene.zFar /= 1.1; break;
  case 'Z': scene.zNear /= 1.1; scene.zFar *= 1.1; break;
  case 'Y':
    scene.mvy+=scene.incr;
    break;
  case 'y':
    scene.mvy-=scene.incr;
    break;

  case 'r': scene.radius *= 1.5; break;
  case 'R': scene.radius /= 1.5; break;

  case 'T': scene.cameraangle += scene.dr; break;
  case 't': scene.cameraangle -= scene.dr; break;

  case 'v': 
    scene.fovy-=5; 
    if (scene.fovy < 0) {
      // Min angle of view 1 degree
      scene.fovy = 1;
    }
    break;
  case 'V': scene.fovy+=5; break;
    if (scene.fovy > 179) {
      // Max angle of view 179 degrees
      scene.fovy = 179;
    }
    break;

  case ' ':  // reset values to their defaults
    scene.rotatep=!scene.rotatep;

    scene.incr=0.1;
    scene.left = -1.0;
    scene.right = 1.0;
    scene.bottom = -1.0;
    scene.top = 1.0;
    scene.mvx = 0.0;
    scene.mvy = 0.0;
    scene.mvz = 0.0;

    break;
  }

  glutPostRedisplay();
}
extern "C" void reshape(int width, int height){
  glViewport(0, 0, width, height);

  scene.aspect = GLfloat(width)/height;
}

extern "C" void idle(){
  // Code to animate cube goes here.
  if (scene.rotatep) {
    scene.angle+=1;
  }
  glutPostRedisplay();
}

extern "C" void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  point4  eye(0,0,15,1.0);

  point4  at(0,0,3, 1.0);

  vec4    up(0.0, 1.0, 0.0, 0.0);
  mat4  cv = LookAt(eye, at, up);
  
  cv = cv * Translate(scene.mvx,scene.mvy,scene.mvz);
  
  glUniformMatrix4fv(scene.camera_view, 1, GL_TRUE, cv);

  mat4  p = Perspective(scene.fovy, scene.aspect, scene.zNear, scene.zFar) ;

  glUniformMatrix4fv(scene.projection, 1, GL_TRUE, p);

  glUniform1i(scene.shade_loc, false);

  mat4 mv = RotateZ(scene.angle);
  scene.mycube->set_mv(mv);
  scene.mycube->draw();

  mv = Translate(0, 4.2,0)*RotateX(scene.angle)*Scale(1, 2, 3);
  scene.mycube2->set_mv(mv);
  scene.mycube2->draw();

  glUniform1i(scene.shade_loc, true);
  mv = Translate(0, -5.2,0)*RotateX(scene.angle)*Scale(1, 4, 2);
  
  // scene.mysphere->set_mv(mv);
  // scene.mysphere->draw();

  glutSwapBuffers();
}


int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Color Cube");

  glewInit();

  std::cout << scene.AllVertices.size() << std::endl;
  scene.init();
  
  std::cout.flush();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutIdleFunc(cube2idle);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
