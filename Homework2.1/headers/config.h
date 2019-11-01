#include <Angel.h>
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
// Game Configuration Global Variable code conventions
    // camelCase variables 
        //change during program execution 
        //including OpenGL execution.
    // PascalCase variables 
        // will have effect on the program only if
        // changed before running OpenGL code
    // UNDERSCORE_CONSTANT variables 
        // will be the format for constant variables 
        // to never be changed during execution
//

    GLfloat xEyeOffset = 0;
    GLfloat zEyeOffset = 0;
    GLfloat xAtOffset = 0;
    GLfloat zAtOffset = 0;

    int windowWidth = 512;
    int windowHeight = 512;

    point4 vertices[8] = {
        point4(-0.5, -0.5,  0.5, 1.0),
        point4(-0.5,  0.5,  0.5, 1.0),
        point4( 0.5,  0.5,  0.5, 1.0),
        point4( 0.5, -0.5,  0.5, 1.0),
        point4(-0.5, -0.5, -0.5, 1.0),
        point4(-0.5,  0.5, -0.5, 1.0),
        point4( 0.5,  0.5, -0.5, 1.0),
        point4( 0.5, -0.5, -0.5, 1.0)
    };
    // RGBA colors
    color4 vertex_colors[8] = {
        color4(0.0, 0.0, 0.0, 1.0),  // black
        color4(1.0, 0.0, 0.0, 1.0),  // red
        color4(1.0, 1.0, 0.0, 1.0),  // yellow
        color4(0.0, 1.0, 0.0, 1.0),  // green
        color4(0.0, 0.0, 1.0, 1.0),  // blue
        color4(1.0, 0.0, 1.0, 1.0),  // magenta
        color4(1.0, 1.0, 1.0, 1.0),  // white
        color4(0.0, 1.0, 1.0, 1.0)   // cyan
    };

    // Viewing transformation parameters
    GLfloat radius = 3.0;
    GLfloat theta = 0.0;
    GLfloat phi = 0.0;

    const GLfloat  dr = 5.0 * DegreesToRadians;

    // Rotation of first cube
        bool rotatep;                  // whether to rotate or not
        GLfloat angle = 0.0;           // Angle of cube rotation.

    // Implementing a door that opens by swinging
        GLfloat doorAngle = 20.0;       // Angle of cube's door's rotation.
        GLfloat doorAngleIncr = 30.0;   // Amount to increment Angle of cube's
                        // door's rotation.
    
        GLfloat trans = 0.0;   // trans of 2nd cube.
        GLfloat transinc = 1.0;// trans increment of 2nd cube.
    //
    // Matrix Uniforms
        GLuint  camera_view;  // camera-view matrix uniform shader variable location
        GLuint  model_view;  // model-view matrix uniform shader variable location
        GLuint  projection; // projection matrix uniform shader variable location

    //
    // Projection transformation parameters
        GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
        GLfloat  aspect;       // Viewport aspect ratio
        GLfloat  zNear = 0.5, zFar = 3.0;
    //






//Glut Callbacks
    extern "C" void passivemouse(int btn, int state, int x, int y);
    extern "C" void special(int key, int x, int y);
    extern "C" void display();
    extern "C" void reshape(int width, int height);
    extern "C" void idle();
    extern "C" void special(int key, int x, int y);
//

//Setup Functions
    void createglutwindow();
    void setupglutcallbacks();
//

// Vertices of a unit cube centered at origin, sides aligned with axes
