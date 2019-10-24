#include "Angel.h"

void windowsetup();
void setupglutcallbacks();
void setupshaders();
void setupcharacter();

//Callbacks for glut 
void display();
void reshape(int newwidth,int newheight);

// global Variables 
    int windowWidth;
    int windowHeight;
    GLuint ShaderProgramID;


int main(int argc, char** argv) {
    glutInit(&argc,argv);  // start Glut
    windowsetup();
    glewInit();
    setupglutcallbacks();

    glutMainLoop();
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

    // set global values to match current window size
    windowWidth = w;
    windowHeight = h;
}


void windowsetup(){
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // RGB and double buffering in glut
    glutInitWindowPosition(200,200);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("Homework1");

    glClearColor(0.0, 0.0, 0.0, 1.0); // set background color to black
}

