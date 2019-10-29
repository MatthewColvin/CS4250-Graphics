#include <Angel.h>
#include "room.h"

// Global Variables
    #include "ConfigVars.h"

// Game Functions
void windowsetup();
void setupglutcallbacks();
void setuproom();

//Callbacks for glut 
    void display();
    void reshape(int newwidth,int newheight);

// global Variables 
    Room* room1;


int main(int argc, char** argv) {
    glutInit(&argc,argv);  // start Glut
    windowsetup();
    glewInit();
    setupglutcallbacks();
    setuproom();

    glutMainLoop();
}

void setuproom(){
    room1 = new Room();
}

void setupglutcallbacks(){
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    //Draw stuff here
   

    glutSwapBuffers();
}

void reshape(int w,int h){
    glViewport(0,0,w,h);
    room1->setwindowdimentions(w,h);
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

