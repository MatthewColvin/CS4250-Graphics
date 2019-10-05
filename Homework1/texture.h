#ifndef TEXTURE_H
#define VEXTURE_H
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <Angel.h>
#include <GL/freeglut.h>
#include <GL/glew.h>
#include <string>
#include <stdio.h>
#include <iostream>

class Texture{

public:
    // must to be a path to an image file supported by stb_image library
    Texture(std::string filepath); 
    ~Texture();

    void Bind(unsigned int unit); // allows us to tell open gl to use this current textrue (State machine)

private: 
    
    GLuint openglID;
};


// will need to seprate from here down into cc file later on 

Texture::Texture(std::string filepath){
    // Actual RGB data
    unsigned char * data; 
    // width and height of the image 
    int width, height;
    // some images may have some component number 
    int components;

    data = stbi_load(filepath.c_str(),&width,&height,&components,4);

    if (data == NULL ){
        std::cerr<< "The image at" << filepath << "failed to load." << std::endl;
    }

    glGenTextures(1,&openglID);
    glBindTexture(GL_TEXTURE_2D,openglID);


   /*  glTextureParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); // reading outside texture width(S)
    glTextrueParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP); //  reading outside texture height(T) fill area with black "GL_CLAMP"

    glTextrueParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // TODO reasearch on texture parameters in open gl
    glTextureParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); */

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);

    stbi_image_free(data);
}

Texture::~Texture(){
    glDeleteTextures(1,&openglID);
}

void Texture::Bind(unsigned int unit){

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D,openglID);
}


#endif