#include "object.h" 
using std::vector;

object::object(vector<point4>& GlobalPoints){
    toGlobalPoints = &GlobalPoints;
}
object::object(vector<color4>& GlobalPoints,vector<point4>& GlobalColors){
    toGlobalPoints = &GlobalPoints;
    toGlobalColors = &GlobalColors;
}
object::object(vector<color4>& GlobalPoints,vector<point4>& GlobalColors,vector<norm4>& Normals){
    toGlobalColors = &GlobalColors;
    toGlobalPoints = &GlobalPoints;
    toGlobalnorms = &Normals;
}