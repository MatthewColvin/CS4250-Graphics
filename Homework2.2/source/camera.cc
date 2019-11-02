#include "camera.h"

Camera::Camera(){

}

void Camera::moveup(float amount){
    translation *= Translate(0.0,0.0,amount);
}
void Camera::movedown(float amount){
    translation*= Translate(-amount,0.0,0.0);
}
void Camera::moveleft(float amount){
    translation*= Translate(-amount,0.0,0.0);
}
void Camera::moveright(float amount){
    translation*= Translate(amount,0.0,0.0);
}

void Camera::moveforward(float amount){
    translation *= Translate(0.0,0.0,amount);
}
void Camera::moveback(float amount){
    translation *= Translate(0.0,0.0,-amount);
}

void Camera::turnleft(float degrees){
    rotation *= RotateY(-degrees);
}
void Camera::turnright(float degrees){
    rotation *= RotateY(degrees);
}