//Class definition of circle object 
// 
// Written by Prof. David M. Chelberg
// 
// Assumes that the circle object was defined in the VAO at location
// index

// last-modified: Fri Oct 25 15:21:30 2013

#ifndef CIRCLE_H
#define CIRCLE_H

#include <Angel.h>
#include <iostream>
#include <stdlib.h>
#include "object.h"

class Circle : public Object
{
public:
  // How many points are needed to specify the circle
  static GLuint NumPoints;

  // Default constructor
  Circle();

  // Constructor if start of circle vertices aren't at 0.
  Circle(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc);

  // Initialize the points in the points array for circle.
  void init_points();

  // Code to call to draw a circle.
  void draw(bool select_mode=false);

  // Update the position of the circle from time
  void update();
  // Update circle's position based on ellipse code.
  void update_ellipse();

  // Change goal location for circle
  void change_goal(GLint nx, GLint ny);

  // Change goal location for circle
  void change_goal(vec2 npos);

  // Set the parameters of the ellipse movement.
  void set_ellipse_parameters(GLfloat minor_axis, GLfloat major_axis,
			      GLfloat angle_offset, GLfloat angular_velocity);
  // Get the parameters of the ellipse movement.
  void get_ellipse_parameters(GLfloat &minor_axis, GLfloat &major_axis,
			      GLfloat &angle_offset, GLfloat &angular_velocity);
private:
  static bool inited;

  // location of where circle wants to go
  GLint goal_x;
  GLint goal_y;

  // Parameters of the ellipse movement
  GLfloat minor_axis;
  GLfloat major_axis;
  GLfloat angle_offset;
  GLfloat angular_velocity;
};

#endif
