#ifndef VERTEX_H
#define VERTEX_H

#include <Angel.h>


class Vertex{
    public:
        Vertex (vec3 position);
        Vertex (vec3 position, vec4 color);
        Vertex (int x,int y,int z);
    
    // TODO implement function for color and getters and setters 

    private:
    // this will hold the attributes of the vertex 
        vec3 pos;
        vec4 col;
};

#endif