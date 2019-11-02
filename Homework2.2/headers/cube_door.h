#ifndef cube_door_h
#define cube_door_h

#include <vector>
#include "common.h"
#include "cube.h"
using std::vector;

class cube_door: public cube
{
public:
  void init(vector<vec4> oldglobalcolors, GLint nmodel_view,int nInitial_Point, bool ninit);
  void draw();
  void set_doorAngle(GLfloat ndoorAngle){
    doorAngle=ndoorAngle;
  };
  void set_mv(mat4 nmv){
    mv=nmv;
  };
private:
  mat4 mv;
  GLint model_view;
  int Initial_Point;
  int NumVertices;
  GLfloat doorAngle;
};
#endif