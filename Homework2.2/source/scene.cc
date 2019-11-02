#include <Angel.h>
#include "common.h"
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
  vector<vec4> colors1 {
    vec4(1,0,0,1), 
    vec4(0,1,0,1), 
    vec4(0,0,1,1), 
    vec4(1,1,0,1), 
    vec4(1,0,1,1), 
    vec4(0,1,1,1)
  };
  vector<vec4> colors2{
    vec4(0.5,0.5,0,1), 
    vec4(0,0.5,0,1),
    vec4(0,0,0.5,1),
    vec4(0.5,0.5,0,1), 
    vec4(0.5,0,0.5,1), 
    vec4(0,0.5,0.5,1)
  };

  mycube.init(
    colors1,
    model_view,
    0, 
    false
  );
  mycube2.init(
    colors2,
    model_view,
    globalpoints.size(), 
    false
  );
  mysphere.init(
    vec4(1.0, 0.5, 0.1, 1), 
    model_view, 
    globalpoints.size(), 
    false
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
		BUFFER_OFFSET(globalpoints.size()*sizeof(vec4))
  );
  


  glBufferData(
    GL_ARRAY_BUFFER, 
    globalpoints.size()*sizeof(vec4) + globalpoints.size()*sizeof(vec4),
	  NULL,
    GL_STATIC_DRAW
  );
  glBufferSubData(
    GL_ARRAY_BUFFER,
    0, 
    globalpoints.size()*sizeof(vec4), globalpoints[0]
  );
  glBufferSubData(
    GL_ARRAY_BUFFER, 
    globalpoints.size()*sizeof(vec4), 
    globalcolors.size()*sizeof(vec4), 
    globalcolors[0]
  );

  
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0); 
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
