#ifndef sphere_h
#define sphere_h

#include <vector>
#include "object.h"
using std::vector;


class sphere : protected object
{
public:
  sphere(vector<point4>& globalpoints,vector<color4>& globalcolors, color4 color ,GLint nmodel_view ,int nInitial_Point);
  void set_mv(mat4 nmv){
    mv=nmv;
  }
  void draw(){object::draw();};
  
private:
  const int NumTimesToSubdivide = 5;
  const int NumTriangles        = 4096;  // (4 faces)^(NumTimesToSubdivide + 1)
  
  void tetrahedron(int count, vec4 color);
  void divide_triangle(point4& a, point4& b, point4& c, int count,vec4 color);
  point4 unit(point4 p);
};

#endif
