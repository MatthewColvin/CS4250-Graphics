#include "cube_door.h"

// Next stuff is in the definition of cube class, and visible to
// outside world.
// Drawing function
void cube_door::draw()
{
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLES, Initial_Point+6, NumVertices-6);
  
  mat4 mv1 = mv*Translate(0.5, 0.0, 0.5)*RotateY(doorAngle)*Translate(-0.5, 0.0, -0.5);
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);
  glDrawArrays(GL_TRIANGLES, Initial_Point, 6);

}

// Initialization function.
void cube_door::init(vector<vec4> ncolors, GLint nmodel_view,int nInitial_Point, bool ninit)
{
  if (!ninit) {
    colorcube(ncolors);
  }
  NumVertices=36;

  model_view=nmodel_view;
  Initial_Point=nInitial_Point;
  doorAngle=0.0;
}
