#include <Angel.h>
#include "cube.h"
#include "cube_door.h"
#include "sphere.h"
#include "camera.h"
#include "scene.h"



//----------------------------------------------------------------------------
// OpenGL initialization
void Scene::init(){
  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("./shaders/vshader41.glsl", "./shaders/fshader41.glsl");
  glUseProgram(program);

  model_view = glGetUniformLocation(program, "model_view");
  camera_view = glGetUniformLocation(program, "camera_view");
  projection = glGetUniformLocation(program, "projection");
  shade_loc = glGetUniformLocation(program, "shade");
    
  // First set up all the models
  vector<color4> colors1 {
    vec4(1,0,0,1), 
    vec4(0,1,0,1), 
    vec4(0,0,1,1), 
    vec4(1,1,0,1), 
    vec4(1,0,1,1), 
    vec4(0,1,1,1)
  };
  vector<color4> colors2{
    vec4(0.5,0.5,0,1), 
    vec4(0,0.5,0,1),
    vec4(0,0,0.5,1),
    vec4(0.5,0.5,0,1), 
    vec4(0.5,0,0.5,1), 
    vec4(0,0.5,0.5,1)
  };

  mycube = new cube_door(
    AllVertices,
    AllColors,
    colors1,
    model_view,
    AllVertices.size()
  );
  
  mycube2 = new cube(
    AllVertices,
    AllColors,
    colors2,
    model_view,
    AllVertices.size()
  );

  mysphere = new sphere(
    AllVertices,
    AllColors,
    vec4(1.0, 0.5, 0.1, 1), 
    model_view, 
    AllVertices.size()
  );


  // Now send the data to the GPU
  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(
    vPosition, 
    4, 
    GL_FLOAT, 
    GL_FALSE, 
    0,
		BUFFER_OFFSET(0)
  );


  GLuint vColor = glGetAttribLocation(program, "vColor"); 
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(
    vColor, 
    4, 
    GL_FLOAT, 
    GL_FALSE, 
    0,
		BUFFER_OFFSET(AllVertices.size()*sizeof(vec4))
  );
  


  glBufferData(
    GL_ARRAY_BUFFER, 
    AllVertices.size()*sizeof(vec4) + AllColors.size()*sizeof(vec4),
	  NULL,
    GL_STATIC_DRAW
  );
  glBufferSubData(
    GL_ARRAY_BUFFER,
    0, 
    AllVertices.size()*sizeof(vec4), AllVertices[0]
  );
  glBufferSubData(
    GL_ARRAY_BUFFER, 
    AllVertices.size()*sizeof(vec4), 
    AllColors.size()*sizeof(vec4), 
    AllColors[0]
  );

  
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0); 
}

//----------------------------------------------------------------------------