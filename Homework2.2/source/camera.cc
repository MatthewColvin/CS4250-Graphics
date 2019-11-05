#include "camera.h"

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
    at = Rotate(upvec,radians) * at;
}
void Camera::turnright(float radians){
    at = Rotate(upvec,-radians) * at;
}
void Camera::turnup(float radians){
    at = Rotate(strafedirection,-radians) * at;
}
void Camera::turndown(float radians){
    at = Rotate(strafedirection,radians) * at ;
}



void Camera::moveup(float amount){
    eye += amount * upvec;
}
void Camera::movedown(float amount){
    eye += -amount * upvec;
}