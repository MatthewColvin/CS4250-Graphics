#ifndef CAMERA_H
#define CAMERA_H
#include<Angle.h>

class Camera{
    public:
        
        void moveforward(float amount);
        void moveback(float amount);
        
        void moveleft(float amount);
        void moveright(float amount);
        
        void movedown(float amount);
        void moveup(float amount);

        void turnleft(float degrees);
        void turnright(float degrees);

        inline mat4 generate_modelview(){Look};

    private:
        vec4 eyecoords;
        vec4 atcoords;
        vec4 upvec;

};

#endif