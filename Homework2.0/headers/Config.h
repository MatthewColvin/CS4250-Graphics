#include <Angel.h>
#include "room.h"
// Game Configuration 
    // camelCase variables 
        //change during program execution 
        //including OpenGL execution.
    // PascalCase variables 
        // will have effect on the program only if
        // changed before running OpenGL code
    // UNDERSCORE_CONSTANT variables 
        // will be the format for constant variables 
        // to never be changed during execution



GLfloat xeyeOffset = 0.0;
GLfloat zeyeOffset = 0.0;


GLuint shaderProgram = 0; // Shader Program ID 

Room *room1;


// Viewing transformation parameters
GLfloat radius = 3.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

const GLfloat  dr = 5.0 * DegreesToRadians;

// Rotation of first cube
bool rotatep;                  // whether to rotate or not
GLfloat angle = 0.0;           // Angle of cube rotation.

// Implementing a door that opens by swinging
GLfloat doorAngle = 0.0;       // Angle of cube's door's rotation.
GLfloat doorAngleIncr = 30.0;   // Amount to increment Angle of cube's
			       // door's rotation.

GLfloat trans = 0.0;   // trans of 2nd cube.
GLfloat transinc = 1.0;// trans increment of 2nd cube.



// Projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 3.0;
//GLfloat  zNear = 0.1;
//GLfloat zFar = 300.0;


// Global to keep track of what vertex we are setting.
int Index = 0;
