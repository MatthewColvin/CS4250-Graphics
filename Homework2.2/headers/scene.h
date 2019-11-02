#include <Angel.h>
#include "common.h"
#include "cube.h"
#include "cube_door.h"
#include "sphere.h"
#include "camera.h"

// Implementing a door that opens by swinging
GLfloat doorAngle = 0.0;       // Angle of cube's door's rotation.
GLfloat doorAngleIncr = 30.0;   // Amount to increment Angle of cube's
			       // door's rotation.
GLfloat trans = 0.0;   // trans of 2nd cube.
GLfloat transinc = 1.0;// trans increment of 2nd cube.






cube_door mycube;
cube mycube2;
sphere mysphere;
Camera camera;

void init();
extern "C" void display();
extern "C" void cube2idle();