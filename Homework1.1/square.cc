//Class implementation of square object 
// 
// Written by Prof. David M. Chelberg
// 
// Assumes that the square object was defined in the VAO at location
// index

// last-modified: Fri Oct  6 07:42:10 2017

#include "square.h"

GLint Square::NumPoints=4;
bool Square::inited=false;

void Square::init_points(){
  if (!inited) {
    points[0+index]= vec2( 1,  1);
    points[1+index]= vec2(-1,  1);
    points[3+index]= vec2(-1, -1);
    points[2+index]= vec2( 1, -1);
    inited=true;
    // Actually send the data we've created to the GPU.
    // Can't do this here as we aren't sure we have an open OpenGL window yet.
    //    glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(vec2), NumPoints*sizeof(vec2), points);
  }
}

// Default constructor
Square::Square() : Object(), goal_x(0.0), goal_y(0.0){
  init_points();
}

// Constructor if start of square vertices aren't at 0.
Square::Square(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc): Object(){
  // Default index is the start (0).
  index = nindex;

  // Update the location of the points array from the main program.
  points = npoints;

  // Update the values of where the variables are in the shaders.
  offsetLoc=noffsetLoc;
  sizeLoc=nsizeLoc;
  colorLoc=ncolorLoc;

  // Default position is (0, 0)
  goal_x=goal_y=0.0;

  init_points();
}
// Code to call to draw a square.
void Square::draw(bool select_mode){
  // Pass the current size of the square
  glUniform1f(sizeLoc, size);
  glUniform2i(offsetLoc, int(x), int(y));
  // Set color on GPU
  if (select_mode) {
    glUniform4f(colorLoc, sr, sg, sb, 1.0);
  } else if (selected) {
    glUniform4f(colorLoc, 0.0, 1.0, 1.0, 1.0);
  } else {
    glUniform4f(colorLoc, r, g, b, 1.0);
  }
  //  std::cout << "Drawing square at (" << x << " " << y << ") color = (" << r << ", " << g << ", " << b << ")" << std::endl;
  glDrawArrays(GL_TRIANGLE_STRIP, index, NumPoints);
}

// Update the position of the square from time
void Square::update(){
  const GLfloat max_speed= speed; // pixels/msec max speed
  
  vec2 dir = vec2(goal_x -x, goal_y- y);
  //  if (length(dir)==0.0) {
    
  //  }else{
  if (length(dir) > 2.0) { 
    dir = (compute_time())*max_speed*normalize(dir);
    // Update location
    x+=dir.x;
    y+=dir.y;
  } else {
    x = goal_x;
    y = goal_y;
  }
  set_last_time();
}

// Change goal location for square
void Square::change_goal(GLint nx, GLint ny){
  goal_x = nx;
  goal_y = ny;
}

// Change goal location for square
void Square::change_goal(vec2 npos){
  goal_x = npos.x;
  goal_y = npos.y;
}

void Square::move_up(int amount){
  y+=amount;
}

void Square::move_down(int amount){
  y-=amount;
}

void Square::move_left(int amount){
  x-=amount;
}

void Square::move_right(int amount){
  x+=amount;
}