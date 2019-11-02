#ifndef CAMERA_H
#define CAMERA_H
#include<Angel.h>

class Camera{
    public:
        
        Camera();

        void moveforward(float amount);
        void moveback(float amount);
        
        void moveleft(float amount);
        void moveright(float amount);
        
        void movedown(float amount);
        void moveup(float amount);

        void turnleft(float degrees);
        void turnright(float degrees);

        inline mat4 generate_modelview(){return(LookAt(eye,at,upvec));}

    private:
        vec4 eye;
        vec4 at;
        vec4 upvec;

};

#endif