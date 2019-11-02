#ifndef cube_h
#define cube_h

#include <vector>
#include "common.h"
#include "object.h"
using std::vector;

// Vertices of a unit cube centered at origin, sides aligned with axes
extern point4 vertices[8];

class cube : private object {
public:
  cube(){};
  cube(vector<point4>* globalpoints,vector<color4>* globalcolors){object(globalpoints,globalcolors);};
  void draw();
  void init(vector<color4> oldglobalcolors, GLint nmodel_view,int nInitial_Point, bool ninit);
  void set_mv(mat4 nmv){
    mv=nmv;
  }
  void colorcube(vector<vec4> oldglobalcolors);
  void MyQuad(int a, int b, int c, int d,vec4& color);
private:
  mat4 mv;
  GLint model_view;
  int Initial_Point;
  int NumVertices;
  vector<color4> wallcolors;
  
};
#endif
