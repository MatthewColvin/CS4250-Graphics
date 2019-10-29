#include "room.h"

Room::Room(){
    
}

Room::Room(int l,int w,int h){
    
}

void Room::setwindowdimentions(int width,int height){
    windowheight = height;
    windowwidth = width;
}

void Room::draw(){
    glBindVertexArray(VAO);



    glBindVertexArray(0);
}