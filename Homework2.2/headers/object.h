#ifndef OBJECT_H
#define OBJECT_H

#include<Angel.h>
#include<vector>
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;
typedef Angel::vec4 norm4;
using std::vector;


class object{
    public:
        object(vector<point4>& GlobalPoints);
        object(vector<point4>& GlobalPoints,vector<color4>& GlobalColors);
        void draw(); // Assumes you are drawing triangle soup 

    private:
      
    protected:
        int Initial_Point; // in vao
        mat4 mv;
        GLint model_view;
        int NumVertices;

        vector<point4> points; // objects current geoemetery
        vector<color4> colors; // objects current colors 
        
        // used to put triangles into the vao to be drawn
        void triangle(point4& a, point4& b, point4& c, vec4 color);
        

};

#endif 