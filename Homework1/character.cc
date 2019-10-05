#include "character.h"
#include <GL/freeglut.h>
#include <GL/glew.h>


Character::Character(int width, int height){

    Vertex topright(0.5, 0.5, 0); // vertex 1
    Vertex topleft(-0.5, 0.5, 0); // 2
    Vertex bottomright(0.5,-0.5,0); // 3
    Vertex bottomleft(-0.5,-0.5,0); // 4

    Vertex BoundingBox[4] = {topright,topleft,bottomright,bottomright};

    // element array for 2 triangles that make a square.
    int draworder[6] = {1,3,4,   1,2,4} ;
    
     // create A VAO for the character and bind it. 
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    GLuint elementarray; 

    glGenBuffers(1,&elementarray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementarray);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,6*sizeof(draworder[0]),draworder,GL_STATIC_DRAW);

    glGenBuffers(2,vertexArrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,vertexArrayBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER,4 * sizeof(BoundingBox[0]), BoundingBox, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(LOCATION);
    glVertexAttribPointer(LOCATION,3,GL_FLOAT,GL_FALSE,7,0);


    glEnableVertexAttribArray(COLOR);
    glVertexAttribPointer(COLOR,4,GL_FLOAT,GL_FALSE,7,BUFFER_OFFSET(4));


    glBindVertexArray(0); // unbind all vertex arrays

}

Character::~Character(){

glDeleteVertexArrays(1,&vertexArrayObject);


}

void Character::Draw(){
    glBindVertexArray(vertexArrayObject);

    glDrawArrays(GL_TRIANGLES,0,drawCount);

    glBindVertexArray(0);
}

