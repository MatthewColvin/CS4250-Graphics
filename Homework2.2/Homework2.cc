#include "scene.h"
#include <string>
using std::string;

Scene scene;
float stepsize = 0.1; 
float camrotationamount = 0.1;
// usedd for mouse look around
int oldx,oldy,deltax,deltay;


extern "C" void display();
extern "C" void cube2idle();
extern "C" void special(int key, int x, int y);
extern "C" void keyboard(unsigned char key, int x, int y);
extern "C" void reshape(int width, int height);
extern "C" void lookaround(int x, int y);
// Simple animation
GLint lasttime=0;
extern "C" void cube2idle(){
  // Added code to account for glutElapsedTime
  GLint time = glutGet(GLUT_ELAPSED_TIME);

  // Code to animate cube goes here.
  // if (scene.rotatep) {
  //   scene.angle+=30.0/1000.0*(time-lasttime);
  // }

  // scene.doorAngle+=scene.doorAngleIncr/1000.0*(time-lasttime);
  // if (scene.doorAngle > 60.0) {
  //   scene.doorAngle = 60;
  //   scene.doorAngleIncr*=-1.0;
  // }
  // if (scene.doorAngle < 0.0){
  //   scene.doorAngle = 0.0;
  //   scene.doorAngleIncr*=-1.0;
  // }
  //scene.mycube->set_doorAngle(scene.doorAngle);

  // Do the animation code here in idle, not in display.
  //  Code for moving 2nd cube
  // scene.trans += scene.transinc/1000.0*(time-lasttime);
  // if (scene.trans > 1.5) {
  //   scene.trans = 1.5;
  //   scene.transinc*=-1;
  // }
  // if (scene.trans < -1.5) {
  //   scene.trans = -1.5;
  //   scene.transinc*=-1;
  // }
  lasttime=time;

  glutPostRedisplay();
}
extern "C" void special(int key, int x, int y){
  switch(key) {
  case GLUT_KEY_UP:    scene.camera.moveforward(stepsize); break;
  case GLUT_KEY_DOWN:  scene.camera.moveback(stepsize);    break;
  case GLUT_KEY_LEFT:  scene.camera.moveleft(stepsize);    break;
  case GLUT_KEY_RIGHT: scene.camera.moveright(stepsize);   break;
  }
}
extern "C" void keyboard(unsigned char key, int x, int y){
  switch(key) {
  case 033: // Escape Key
  case 'q': case 'Q':
    exit(EXIT_SUCCESS);
    break;

    // Speed up/slow down movements
  
  case '+': scene.incr*=2.0; break;
  case '-': scene.incr/=2.0; break;

  case 'z': scene.zNear  *= 1.1; scene.zFar /= 1.1; break;
  case 'Z': scene.zNear /= 1.1; scene.zFar *= 1.1;  break;
  
  case 'Y': scene.camera.moveup(stepsize);   break;
  case 'y': scene.camera.movedown(stepsize); break;
<<<<<<< HEAD

  case 'a': scene.camera.moveright(stepsize); break;
  case 'd': scene.camera.moveleft(stepsize);  break;

  case 'w': scene.camera.moveforward(stepsize); break;
  case 's': scene.camera.moveback(stepsize);    break;

=======
  
  case 'w': scene.camera.turnup(camrotationamount);    break;
  case 's': scene.camera.turndown(camrotationamount);  break;
  case 'a': scene.camera.turnleft(camrotationamount);  break;
  case 'd': scene.camera.turnright(camrotationamount); break;
>>>>>>> 187150c3f668d61b5d86ef83239894461004c8b4

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

    break;
  }

  glutPostRedisplay();
}
extern "C" void reshape(int width, int height){
  glViewport(0, 0, width, height);

  scene.aspect = GLfloat(width)/height;
}
extern "C" void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4 cv = scene.camera.generate_view_matrix();
  
  glUniformMatrix4fv(scene.camera_view_loc, 1, GL_TRUE, cv);

  mat4  p = Perspective(scene.fovy, scene.aspect, scene.zNear, scene.zFar) ;

  glUniformMatrix4fv(scene.projection_loc, 1, GL_TRUE, p);

  glUniform1i(scene.shade_loc, false);

  string cameraangle = "CA:" + std::to_string(scene.camera.get_cameraangel());
  string deltaax = "DelX:" + std::to_string(deltax);
  string newtitle = cameraangle + deltaax ;


   glutSetWindowTitle(newtitle.c_str());

  mat4 mv = RotateZ(scene.angle);
  scene.mycube->set_mv(mv);
  scene.mycube->draw();

  mv = Translate(0, 4.2,0)*RotateX(scene.angle)*Scale(2, 4, 6);
  scene.mycube2->set_mv(mv);
  scene.mycube2->draw();

  glUniform1i(scene.shade_loc, true);
  mv = Translate(0, -5.2,0)*RotateX(scene.angle)*Scale(1, 4, 2);
  
  scene.mysphere->set_mv(mv);
  scene.mysphere->draw();

  glutSwapBuffers();
}

float lookaroundsensetivity=0.008;
extern "C" void lookaround(int x, int y){
  deltax = oldx - x;
  deltay = oldy -y;
  
  if(deltax <= 0){
    scene.camera.turnleft(-deltax*lookaroundsensetivity);
  }else{
    scene.camera.turnright(deltax*lookaroundsensetivity);
  }

  oldx = x;
  oldy = y;
}

int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Color Cube");

  glewInit();

  scene.init();
  
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutIdleFunc(cube2idle);
  glutReshapeFunc(reshape);
  glutPassiveMotionFunc(lookaround);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
