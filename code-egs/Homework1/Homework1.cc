#include "character.h"
#include "vertex.h"

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

    Character* dog;



int main(int argc, char** argv) {
    glutInit(&argc,argv);  // start Glut
    windowsetup();
    setupglutcallbacks();
    setupcharacter();
    setupshaders();
    

    glutMainLoop();
}


void setupcharacter(){
    dog = new Character(50,50,vec4(0.0,0.0,1.0,1.0));
}


void setupshaders(){
    GLuint ShaderProgramID = InitShader("vshader.glsl", "fshader.glsl");
}


void setupglutcallbacks(){
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    //Draw stuff here
    
    dog->Draw();
    
    glFlush();
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
    glutInitWindowSize(500,500);
    glutCreateWindow("Homework1");

    glClearColor(0.0, 0.0, 0.0, 1.0); // set background color to black

}

