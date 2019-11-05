#include "cube_door.h"

// Next stuff is in the definition of cube class, and visible to
// outside world.
// Drawing function
void cube_door::draw()
{
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLES, StartInVao+6, NumVertices-6);
  
  mat4 mv1 = mv*Translate(0.5, 0.0, 0.5)*RotateY(doorAngle)*Translate(-0.5, 0.0, -0.5);
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv1);
  glDrawArrays(GL_TRIANGLES, StartInVao, 6);

}

// Initialization function.
cube_door::cube_door(vector<point4>& globalpoints,vector<color4>& globalcolors,vector<color4>& wallcolors,
GLint shader_mv_loc,int startinvao):
cube(globalpoints,globalcolors,wallcolors,shader_mv_loc,startinvao){

  doorAngle=0.0;
}
