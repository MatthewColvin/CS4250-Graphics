#include "cube.h"

// Next stuff is internal to cube class, not visible to outside world

// 8 points of the cube
point4 vertices[8] = {
    point4(-0.5, -0.5,  0.5, 1.0),
    point4(-0.5,  0.5,  0.5, 1.0),
    point4( 0.5,  0.5,  0.5, 1.0),
    point4( 0.5, -0.5,  0.5, 1.0),
    point4(-0.5, -0.5, -0.5, 1.0),
    point4(-0.5,  0.5, -0.5, 1.0),
    point4( 0.5,  0.5, -0.5, 1.0),
    point4( 0.5, -0.5, -0.5, 1.0)
};

//----------------------------------------------------------------------------
// MyQuad generates two triangles for each face and assigns colors
//    to the vertices
void MyQuad(int a, int b, int c, int d, vec4& color)
{
  colors.push_back(color); points.push_back(vertices[a]);
  colors.push_back(color); points.push_back(vertices[b]);
  colors.push_back(color); points.push_back(vertices[c]);

  colors.push_back(color); points.push_back(vertices[a]);
  colors.push_back(color); points.push_back(vertices[c]);
  colors.push_back(color); points.push_back(vertices[d]);
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void cube::colorcube(vector<vec4> colors1)
{
  MyQuad(1, 0, 3, 2, colors1[0]);
  MyQuad(2, 3, 7, 6, colors1[1]);
  MyQuad(3, 0, 4, 7, colors1[2]);
  MyQuad(6, 5, 1, 2, colors1[3]);
  MyQuad(4, 5, 6, 7, colors1[4]);
  MyQuad(5, 4, 0, 1, colors1[5]);
}

// Next stuff is in the definition of cube class, and visible to
// outside world.
// Drawing function
void cube::draw()
{
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLES, Initial_Point, NumVertices);
}

// Initialization function.
void cube::init(vector<vec4> ncolors, GLint nmodel_view,int nInitial_Point, bool ninit)
{
  if (!ninit) {
    colorcube(ncolors);
  }
  NumVertices=36;

  model_view=nmodel_view;
  Initial_Point=nInitial_Point;
}
