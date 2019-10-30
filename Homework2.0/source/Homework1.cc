#include "Config.h"
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

// Game Functions
void windowsetup();
void setupglutcallbacks();
void setuproom();

//Callbacks for glut 
    void display();
    void reshape(int newwidth,int newheight);


int main(int argc, char** argv) {
    glutInit(&argc,argv);  // start Glut
    windowsetup();
    glewInit();
    setupglutcallbacks();
    setuproom();

    glutMainLoop();
}

void setupglutcallbacks(){
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    //Draw stuff here

    point4  eye(
        xeyeOffset   +   radius *  sin(theta) * cos(phi),
	      
        radius  *   sin(theta) *   sin(phi),
	      
        zeyeOffset   +   radius    *    cos(theta),
	      
        1.0);
    
    point4  at(xeyeOffset, 0.0, zeyeOffset, 1.0);

    Angel::vec4 up(0.0, 1.0, 0.0, 0.0);

    mat4  cv = LookAt(eye, at, up);

    glUseProgram(room1->shaders);

    glUniformMatrix4fv(room1->camera_view, 1, GL_TRUE, cv);

    mat4 mv = RotateZ(angle);
    glUniformMatrix4fv(room1->model_view, 1, GL_TRUE, mv);

    mat4  p = Perspective(fovy, aspect, zNear, zFar);
    glUniformMatrix4fv(room1->projection, 1, GL_TRUE, p);

    room1->draw();

    glutSwapBuffers();
}

void reshape(int w,int h){
    glViewport(0,0,w,h);
}

void setshaders(string vshader,string fshader){
   shaderProgram = InitShader(vshader.c_str(),fshader.c_str());
}


void windowsetup(){
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // RGB and double buffering in glut
    glutInitWindowPosition(200,200);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("Homework2");

    glClearColor(0.0, 0.0, 0.0, 1.0); // set background color to black
}

void setuproom(){
    room1 = new Room();
}