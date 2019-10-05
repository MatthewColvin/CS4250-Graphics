#include "../headers/vertex.h"

Vertex::Vertex (vec3 position){
    pos = position; 
    col= vec4(1.0,0.0,0.0,1.0); 
}
Vertex::Vertex (vec3 position, vec4 color){
    pos = position;
    col = color; 
}
Vertex::Vertex (int x,int y ,int z){ 
    pos.x=x;
    pos.y=y; 
    pos.z=z; 
    col= vec4(1.0,0.0,0.0,1.0); 
}
