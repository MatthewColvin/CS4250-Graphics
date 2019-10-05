#ifndef CHARACTER_H
#define CHARACTER_H


#include <iostream>
#include <stdlib.h>
#include "../headers/texture.h"
#include "../headers/vertex.h"

#include <GL/freeglut.h>
#include <GL/glew.h>

class Character{

    public:
        // The width and height passed in will be the size of the square generated
        // I hope to somehow add textures to this constructor 
       Character(int width,int height,vec4 selectcolor); 
       ~Character();

       void Draw();

    private:
        
        // The VAO will "store" all the vertex Array buffers
        GLuint vertexArrayObject;
        enum {LOCATION,COLOR};

        // 1 buffer for the location and one for the color 
        GLuint vertexArrayBuffer[2]; 

        unsigned int drawCount;


};

#endif