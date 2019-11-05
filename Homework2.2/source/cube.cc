#include "cube.h"
// MyQuad generates two triangles for each face and assigns colors
// to the vertices
void cube::MyQuad(int a, int b, int c, int d, vec4& color){
  
  colors.push_back(color); points.push_back(vertices[a]);
  colors.push_back(color); points.push_back(vertices[b]);
  colors.push_back(color); points.push_back(vertices[c]);

  colors.push_back(color); points.push_back(vertices[a]);
  colors.push_back(color); points.push_back(vertices[c]);
  colors.push_back(color); points.push_back(vertices[d]);
}

void cube::makecolorcube(vector<vec4> colors1){
  MyQuad(1, 0, 3, 2, colors1[0]);
  MyQuad(2, 3, 7, 6, colors1[1]);
  MyQuad(3, 0, 4, 7, colors1[2]);
  MyQuad(6, 5, 1, 2, colors1[3]);
  MyQuad(4, 5, 6, 7, colors1[4]);
  MyQuad(5, 4, 0, 1, colors1[5]);
}

cube::cube(vector<point4>& globalpoints,vector<color4>& globalcolors,vector<color4>& wallcolors,
    GLint shader_mv_loc,
    int startinvao
  ):object(globalpoints,globalcolors){
  
  makecolorcube(wallcolors);
  
  NumVertices=36;

  model_view=shader_mv_loc;
  StartInVao=startinvao;

  globalpoints.insert(globalpoints.end(),
    points.begin(),points.end()
  );
  globalcolors.insert(globalcolors.end(),
    colors.begin(),colors.end()
  );
}
