#include "room.h"

Room::Room(){
    // initialize points 
    init_points();
    colorcube();
    // Vertices of a unit cube centered at origin, sides aligned with axes
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(
      GL_ARRAY_BUFFER, 
      sizeof(points) * points.size() + sizeof(colors) * colors.size(),
      NULL,
      GL_STATIC_DRAW
    );

    glBufferSubData(
      GL_ARRAY_BUFFER,
      0,
      sizeof(points) * points.size() ,
      &points.front()
    );
    
    glBufferSubData(
      GL_ARRAY_BUFFER, 
      sizeof(points) * points.size() , 
      sizeof(colors) * colors.size() , 
      &colors.front()
    );


  // Load shaders and use the resulting shader program
  GLuint shaders = InitShader("./shaders/vshader41.glsl", "./shaders/fshader41.glsl");
  glUseProgram(shaders);

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(shaders, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  GLuint vColor = glGetAttribLocation(shaders, "vColor"); 
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)*points.size()));

  model_view = glGetUniformLocation(shaders, "model_view");
  camera_view = glGetUniformLocation(shaders, "camera_view");
  projection = glGetUniformLocation(shaders, "projection");
    
  glEnable(GL_DEPTH_TEST);


  glBindVertexArray(0); // unbind vertex array object 
}

Room::Room(int l,int w,int h){
    
}

void Room::draw(){
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, NumVertices());

    glBindVertexArray(0);
}


void Room::init_points(){
    vertices[0] = point4(-0.5, -0.5,  0.5, 1.0);
    vertices[1] = point4(-0.5,  0.5,  0.5, 1.0);
    vertices[2] = point4( 0.5,  0.5,  0.5, 1.0);
    vertices[3] = point4( 0.5, -0.5,  0.5, 1.0);
    vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
    vertices[5] = point4(-0.5,  0.5, -0.5, 1.0);
    vertices[6] = point4( 0.5,  0.5, -0.5, 1.0);
    vertices[7] = point4( 0.5, -0.5, -0.5, 1.0);

    vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0);  // black
    vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0);  // red
    vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0);  // yellow
    vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0);  // green
    vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0);  // blue
    vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0);  // magenta
    vertex_colors[6] = color4(1.0, 1.0, 1.0, 1.0);  // white
    vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0);   // cyan
    
}

void Room::MyQuad(int a, int b, int c, int d)
{
  colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
  colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
  colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
  colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
  colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
  colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

void Room::colorcube(){
  points.resize(36);
  colors.resize(36);
  MyQuad(1, 0, 3, 2);
  MyQuad(2, 3, 7, 6);
  MyQuad(3, 0, 4, 7);
  MyQuad(6, 5, 1, 2);
  MyQuad(4, 5, 6, 7);
  MyQuad(5, 4, 0, 1);
}