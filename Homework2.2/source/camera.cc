#include "camera.h"
#include <math.h>
void Camera::moveleft(float amount){
    translation *= Translate(-amount,0,0);
}
void Camera::moveright(float amount){
    translation *= Translate(amount,0,0);
}
void Camera::moveforward(float amount){
    translation *= Translate(0,0,-amount);
}
void Camera::moveback(float amount){
    translation *= Translate(0,0,amount);
}


void Camera::turnleft(float radians){  
    cameraangleinz += radians;
    while(cameraangleinz >= 2*M_PI){cameraangleinz -= 2 * M_PI;} 
    while(cameraangleinz <= 0){cameraangleinz += 2 * M_PI;} 
    at = Rotate(upvec,radians) * at;
}
void Camera::turnright(float radians){
    cameraangleinz -= radians;
    while(cameraangleinz <= 0){cameraangleinz += 2 * M_PI;} 
    while(cameraangleinz >= 2*M_PI){cameraangleinz -= 2 * M_PI;} 
    at = Rotate(upvec,-radians) * at;
}



void Camera::moveup(float amount){
    eye += amount * upvec;
}
void Camera::movedown(float amount){
    eye += -amount * upvec;
}