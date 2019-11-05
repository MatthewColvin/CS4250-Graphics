#include "camera.h"

void Camera::moveleft(float amount){
    eye += -amount * strafedirection;
}
void Camera::moveright(float amount){
    eye += amount * strafedirection;
}
void Camera::moveforward(float amount){
    eye += amount * at;
}
void Camera::moveback(float amount){
   eye += -amount * at;
}

void Camera::turnleft(float degrees){  
    at = Rotate(upvec,degrees) * at;
}
void Camera::turnright(float degrees){
    at = Rotate(upvec,-degrees) * at;
}



void Camera::moveup(float amount){
    eye += amount * upvec;
}
void Camera::movedown(float amount){
    eye += amount * upvec;
}