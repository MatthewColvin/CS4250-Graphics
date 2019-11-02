#include "camera.h"

Camera::Camera(){

}

void Camera::moveup(float amount){
    eye.y += amount;
    at.y += amount;
}
void Camera::movedown(float amount){
    eye.y -= amount;
    at.y -= amount;
}
void Camera::moveleft(float amount){
    eye.x -= amount;
    at.x -= amount;
}
void Camera::moveright(float amount){
    eye.x += amount;
    at.x += amount;
}

void Camera::moveforward(float amount){
    eye.z += amount;
    at.z += amount;
}
void Camera::moveback(float amount){
    eye.z -= amount;
    at.z -= amount;
}

void Camera::turnleft(float degrees){
    at.x += degrees;
}
void Camera::turnright(float degrees){
    at.x -= degrees;
}