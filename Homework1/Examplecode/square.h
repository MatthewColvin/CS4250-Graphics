#ifndef SQUARE_H
#define SQUARE_H
//Class definition of square object 
// 
// Written by Prof. David M. Chelberg
// 
// Assumes that the square object was defined in the VAO at location
// index

// last-modified: Fri Oct 25 15:22:05 2013

#include <Angel.h>
#include <iostream>
#include <stdlib.h>
#include "object.h"

class Square :public Object
{
public:
  // How many points are needed to specify the square
  static GLint NumPoints;

  // Default constructor
  Square();

  // Constructor if start of square vertices aren't at 0.
  Square(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc);

  // Initialize the points in the points array for square.
  void init_points();

  // Code to call to draw a square.
  // If select is true, then use the selection color.
  void draw(bool select=false);

  // Update the position of the square from time
  void update();
  // Update square's position based on ellipse code.
  void update_ellipse();

  // Change goal location for square
  void change_goal(GLint nx, GLint ny);

  // Change goal location for square
  void change_goal(vec2 npos);
  
  // Set the parameters of the ellipse movement.
  void set_ellipse_parameters(GLfloat minor_axis, GLfloat major_axis,
			      GLfloat angle_offset, GLfloat angular_velocity);

  // Get the parameters of the ellipse movement.
  void get_ellipse_parameters(GLfloat &minor_axis, GLfloat &major_axis,
			      GLfloat &angle_offset, GLfloat &angular_velocity);
private:
  static bool inited;

  // location of where square wants to go
  GLint goal_x;
  GLint goal_y;

  // Parameters of the ellipse movement
  GLfloat minor_axis;
  GLfloat major_axis;
  GLfloat angle_offset;
  GLfloat angular_velocity;
};

#endif
