#ifndef cube_h
#define cube_h

#include <vector>
#include "common.h"
using std::vector;

// Vertices of a unit cube centered at origin, sides aligned with axes
extern point4 vertices[8];

class cube
{
public:
  cube(){};
  void draw();
  void init(vector<color4> colorvectorrrr, GLint nmodel_view,int nInitial_Point, bool ninit);
  void set_mv(mat4 nmv){
    mv=nmv;
  }
  void colorcube(vector<vec4> colorvectorrrr);
  void MyQuad(int a, int b, int c, int d,vec4& color);
private:
  mat4 mv;
  GLint model_view;
  int Initial_Point;
  int NumVertices;
  vector<color4> wallcolors;
  
};
#endif
