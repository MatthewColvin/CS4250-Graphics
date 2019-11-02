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
        object(){};
        object(vector<point4>* GlobalPoints);
        object(vector<color4>* GlobalPoints,vector<point4>* GlobalColors);
        object(vector<color4>* GlobalPoints,vector<point4>* GlobalColors,vector<norm4>* Normals);

    private:
        vector<point4>* toGlobalPoints;
        vector<color4>* toGlobalColors;
        vector<norm4>*  toGlobalnorms;
};

#endif 