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
    cube(
      vector<point4>& globalpoints,
      vector<color4>& globalcolors,
      vector<color4>& wallcolors,
      GLint shader_mv_loc,
      int startinvao
      );
    void draw();
    //void init(vector<color4> wallcolors, GLint nmodel_view,int nInitial_Point, bool ninit);
    void set_mv(mat4 nmv){
      mv=nmv;
    }
    void makecolorcube(vector<vec4> wallcolors);
    void MyQuad(int a, int b, int c, int d,vec4& color);
  private:
    mat4 mv;
    vector<point4> cubespoints;
    vector<color4> cubescolor;
  protected:
    int Initial_Point;
    GLint model_view;
    int NumVertices;

};
#endif
