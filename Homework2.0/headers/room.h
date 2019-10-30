#include <Angel.h>
#include <vector>
#include <string> 

using std::vector;
using std::string;

class Room{
    public:
        
        Room();
        Room(int lenth, int width, int height);
        
        void draw();

        GLuint  camera_view;  // camera-view matrix uniform shader variable location
        GLuint  model_view;  // model-view matrix uniform shader variable location
        GLuint  projection; // projection matrix uniform shader variable location
        GLuint  shaders;

    private:
        typedef Angel::vec4  color4;
        typedef Angel::vec4  point4;

        void init_points();
        void MyQuad(int a, int b, int c, int d);
        void colorcube();
        inline int NumVertices(){return points.size();} 


        color4 vertex_colors[8];
        point4 vertices[8];

        vector<point4> points;
        vector<color4> colors;
        
        GLuint VAO;
        // Attributes
        //GLuint vPosition;
        //GLuint vColor;
        
        
};