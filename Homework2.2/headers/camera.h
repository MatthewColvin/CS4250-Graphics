#ifndef CAMERA_H
#define CAMERA_H
#include<Angel.h>

//world coordinates * camera matrix = 

class Camera{
    public:
        
        Camera(){
            // find the vector orthognal to the upvector and the view direction
            strafedirection = cross(at,upvec);
        };
        //"walking"
        void moveforward(float amount);
        void moveback(float amount);
        void moveleft(float amount);
        void moveright(float amount);
        //"flying"
        void movedown(float amount);
        void moveup(float amount);
        // "looking around"
        void turnleft(float radians);
        void turnright(float radians);
        void turnup(float radians);
        void turndown(float radians);

        inline mat4 generate_view_matrix(){
            return(LookAt(eye,eye+at,upvec)*translation);}


    private:
        vec4 eye = vec4(0,0,-15,0);
        vec4 at = vec4(0,0,1,0);
        vec3 upvec = vec3(0.0, 1.0, 0.0);
        vec3 strafedirection;

    

        mat4 translation = mat4(1.0, 0.0 ,0.0, 0.0,
                                0.0, 1.0 ,0.0, 0.0,
                                0.0, 0.0 ,1.0, 0.0,
                                0.0, 0.0 ,0.0, 1.0);
        
};

#endif