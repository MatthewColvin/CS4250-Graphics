#include "cube.h"

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
void cube::MyQuad(int a, int b, int c, int d, vec4& color){
  
  cubescolor.push_back(color); cubespoints.push_back(vertices[a]);
  cubescolor.push_back(color); cubespoints.push_back(vertices[b]);
  cubescolor.push_back(color); cubespoints.push_back(vertices[c]);

  cubescolor.push_back(color); cubespoints.push_back(vertices[a]);
  cubescolor.push_back(color); cubespoints.push_back(vertices[c]);
  cubescolor.push_back(color); cubespoints.push_back(vertices[d]);
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void cube::makecolorcube(vector<vec4> colors1){
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
void cube::draw(){
  glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  glDrawArrays(GL_TRIANGLES, Initial_Point, NumVertices);
}

cube::cube(vector<point4>& globalpoints,vector<color4>& globalcolors,vector<color4>& wallcolors,
    GLint shader_mv_loc,
    int startinvao
  ){
  makecolorcube(wallcolors);
  NumVertices=36;


  model_view=shader_mv_loc;
  Initial_Point=startinvao;

  globalpoints.insert(globalpoints.end(),
    cubespoints.begin(),cubespoints.end()
  );
  globalcolors.insert(globalcolors.end(),
    cubescolor.begin(),cubescolor.end()
  );
}
