//Class implementation of circle object 
// 
// Written by Prof. David M. Chelberg
// 
// Assumes that the circle object was defined in the VAO at location
// index

// last-modified: Fri Oct 25 15:21:24 2013

#include "circle.h"

bool Circle::inited=false;
GLuint Circle::NumPoints=18;

void Circle::init_points()
{
  if (!inited && points!=NULL) {
    points[index]= vec2( 0,  0);
    GLfloat delta=(2.0*M_PI)/float(NumPoints-2);
    GLfloat angle=0.0;
    for (size_t ind = 1; ind < NumPoints; ++ind) {
      points[index+ind]=vec2(cos(angle), sin(angle));
      angle+=delta;
    }
    inited=true;
    // Actually send the data we've created to the GPU.
    //    glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(vec2), NumPoints*sizeof(vec2), points);
  }
}

// Default constructor
Circle::Circle() : Object(), goal_x(0.0), goal_y(0.0)
{
  // Defaults
  minor_axis=100.0;
  major_axis=200.0;
  angle_offset=3.14;
  angular_velocity=1.0/330.0;

  init_points();
}

// Constructor if start of circle vertices aren't at 0.
Circle::Circle(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc): Object()
{
  // Default index is the start (0).
  index = nindex;

  // Update the location of the points array in the main program.
  points = npoints;

  // Update the values of where the variables are in the shaders.
  offsetLoc=noffsetLoc;
  sizeLoc=nsizeLoc;
  colorLoc=ncolorLoc;

  // Default position is (0, 0)
  goal_x=goal_y=0.0;

  // Defaults
  minor_axis=100.0;
  major_axis=200.0;
  angle_offset=3.14;
  angular_velocity=1.0/330.0;

  init_points();
}

// Setter to modify the parameters of the ellipse orbit
void Circle::set_ellipse_parameters(GLfloat nminor_axis, GLfloat nmajor_axis,
				    GLfloat nangle_offset, 
				    GLfloat nangular_velocity)
{
  minor_axis=nminor_axis;
  major_axis=nmajor_axis;
  angle_offset=nangle_offset;
  angular_velocity=nangular_velocity;
}

// Get the parameters of the ellipse movement.
void Circle::get_ellipse_parameters(GLfloat &nminor_axis, GLfloat &nmajor_axis,
				    GLfloat &nangle_offset, 
				    GLfloat &nangular_velocity)
{
  nminor_axis=minor_axis;
  nmajor_axis=major_axis;
  nangle_offset=angle_offset;
  nangular_velocity=angular_velocity;
}

// Code to call to draw a circle.
void Circle::draw(bool select_mode)
{
  // Pass the current size of the circle
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

  //  std::cout << "Drawing circle at (" << x << " " << y << ") color = (" << r << ", " << g << ", " << b << ")" << std::endl;
  glDrawArrays(GL_TRIANGLE_FAN, index, NumPoints);
}

// Update the posiiton of the circle from time
void Circle::update()
{
  const GLfloat max_speed= 200.0/1000.0; // pixels/msec max speed
  
  vec2 dir = vec2(goal_x -x, goal_y- y);
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

// Update the posiiton of the circle from time
void Circle::update_ellipse()
{
  current_time += compute_time();
  x = goal_x + major_axis*sin(current_time*angular_velocity+angle_offset);
  y = goal_y + minor_axis*cos(current_time*angular_velocity+angle_offset);
  set_last_time();
}
 
// Change goal location for circle
void Circle::change_goal(GLint nx, GLint ny)
{
  goal_x = nx;
  goal_y = ny;
}

// Change goal location for circle
void Circle::change_goal(vec2 npos)
{
  goal_x = npos.x;
  goal_y = npos.y;
}
