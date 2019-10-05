#ifndef VERTEX_H
#define VERTEX_H

#include <Angel.h>

class Vertex{
    public:
        inline Vertex (vec3 position){pos = position; col= vec4(1.0,0.0,0.0,1.0); };
        inline Vertex (vec3 position, vec4 color){pos = position; col = color; };
        inline Vertex (int x,int y ,int z){ pos.x=x; pos.y=y; pos.z=z; };
        inline Vertex (int r, int g, int b, int a); 
    
    // TODO implement function for color and getters and setters 

    private:
    // this will hold the attributes of the vertex 
        vec3 pos;
        vec4 col;
};

#endif