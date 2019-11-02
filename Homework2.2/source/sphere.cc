#include "sphere.h"

void triangle(point4& a, point4& b, point4& c, vec4 &color){
  oldglobalcolors.push_back(color); oldglobalpoints.push_back(a);a.w=0.0;oldglobalnormals.push_back(a);a.w=1.0;
  oldglobalcolors.push_back(color); oldglobalpoints.push_back(b);b.w=0.0;oldglobalnormals.push_back(b);b.w=1.0;
  oldglobalcolors.push_back(color); oldglobalpoints.push_back(c);c.w=0.0;oldglobalnormals.push_back(c);c.w=1.0;
}

//----------------------------------------------------------------------------

point4 unit(point4 p){
  float len = p.x*p.x + p.y*p.y + p.z*p.z;
    
  point4 t;
  if (len > DivideByZeroTolerance) {
    t = p / sqrt(len);
    t.w = 1.0;
  }

  return t;
}

void divide_triangle(point4& a, point4& b, point4& c, int count,vec4 color){
  if (count > 0) {
    point4 v1 = unit(a + b);
    point4 v2 = unit(a + c);
    point4 v3 = unit(b + c);
    divide_triangle( a, v1, v2, count - 1, color);
    divide_triangle( c, v2, v3, count - 1, color);
    divide_triangle( b, v3, v1, count - 1, color);
    divide_triangle(v1, v3, v2, count - 1, color);
  }
  else {
    triangle(a, b, c, color);
  }
}

void tetrahedron(int count, vec4 color){
  point4 v[4] = {
    vec4(0.0, 0.0, 1.0, 1.0),
    vec4(0.0, 0.942809, -0.333333, 1.0),
    vec4(-0.816497, -0.471405, -0.333333, 1.0),
    vec4(0.816497, -0.471405, -0.333333, 1.0)
  };

  divide_triangle(v[0], v[1], v[2], count, color);
  divide_triangle(v[3], v[2], v[1], count, color);
  divide_triangle(v[0], v[3], v[1], count, color);
  divide_triangle(v[0], v[2], v[3], count, color);
}

void sphere::draw(){
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLES, Initial_Point, NumVertices);
}

void sphere::init(vec4 color, GLint nmodel_view,int nInitial_Point, bool ninit){
  if (!ninit) {
    tetrahedron(NumTimesToSubdivide, color);
  }
  NumVertices         = 3 * NumTriangles;

  model_view=nmodel_view;
  Initial_Point=nInitial_Point;
}
