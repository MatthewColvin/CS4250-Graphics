#include "sphere.h"


sphere::sphere(vector<point4>& globalpoints,vector<color4>& globalcolors, color4 color, GLint nmodel_view,int nInitial_Point):
object(globalpoints,globalcolors){
  
  tetrahedron(NumTimesToSubdivide, color);
  NumVertices = 3 * NumTriangles;

  model_view=nmodel_view;
  Initial_Point=nInitial_Point;

  globalpoints.insert(globalpoints.end(),points.begin(),points.end());
  globalcolors.insert(globalcolors.end(),colors.begin(),colors.end());
}


point4 sphere::unit(point4 p){
  float len = p.x*p.x + p.y*p.y + p.z*p.z;
    
  point4 t;
  if (len > DivideByZeroTolerance) {
    t = p / sqrt(len);
    t.w = 1.0;
  }

  return t;
}

void sphere::divide_triangle(point4& a, point4& b, point4& c, int count,vec4 color){
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

void sphere::tetrahedron(int count, vec4 color){
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