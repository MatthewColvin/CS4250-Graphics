#ifndef SQUARE_H
#define SQUARE_H

// Assumes that the square object was defined in the VAO at location
// index
//
// must call goal_to_pos in order to use any move_... functions
//

#include <Angel.h>
#include <iostream>
#include <stdlib.h>
#include "object.h"

class Square :public Object
{
public:
  // How many points are needed to specify the square
  static GLint NumPoints;

  Square();
  // Constructor if start of square vertices aren't at 0.
  Square(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc);

  // Initialize the points in the points array for square.
  void init_points();

  // If select is true, then use the selection color.
  void draw(bool select=false);

  // Update the position of the square from time
  void update();

  // Change goal location for square
  void change_goal(GLint nx, GLint ny);

  // Change goal location for square
  void change_goal(vec2 npos);  

  // must call goal_to_pos in order to use any move_... functions
  void goal_to_pos();

  void move_up(int amount);
  void move_down(int amount);
  void move_right(int amount);
  void move_left(int amount);

private:
  static bool inited;

  GLint goal_x;
  GLint goal_y;

protected:
  vec3 colorwhenselected = vec3(0.0,1.0,1.0);

};

#endif
