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

        void setwindowdimentions(int width,int height);

    private:
        //width and height of glut window        
        int windowwidth;
        int windowheight;


        GLuint shaderprogram;
        GLuint VAO;
        vector <GLuint> attributes;

        //error handeling
        bool ShadersSet= false;
};