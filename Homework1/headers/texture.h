#ifndef TEXTURE_H
#define VEXTURE_H

#include <string>
#include <Angel.h>

class Texture{

public:
    // must to be a path to an image file supported by stb_image library
    Texture(std::string filepath); 
    ~Texture();

    void Bind(unsigned int unit); // allows us to tell open gl to use this current textrue (State machine)

private: 
    
    GLuint openglID;
};


#endif
