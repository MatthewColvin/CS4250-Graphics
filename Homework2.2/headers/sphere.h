#ifndef sphere_h
#define sphere_h

#include <vector>
#include "common.h"
using std::vector;


class sphere
{
public:
  sphere(){};
  void draw();
  void init(vec4 colors, GLint nmodel_view,int nInitial_Point, bool ninit);
  void set_mv(mat4 nmv){
    mv=nmv;
  }
private:
  mat4 mv;
  GLint model_view;
  int Initial_Point;
  int NumVertices;
  const int NumTimesToSubdivide = 5;
  const int NumTriangles        = 4096;  // (4 faces)^(NumTimesToSubdivide + 1)
};

#endif
