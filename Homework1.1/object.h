#ifndef OBJECT_H
#define OBJECT_H
//Class definition of object object 
// 
// Written by Prof. David M. Chelberg
// 
// Object base class
//   implements common functions to all objects that will be drawn/animated

// last-modified: Fri Oct  6 07:41:54 2017

#include <Angel.h>
#include <iostream>
#include <stdlib.h>

class Object 
{
  public:

  // Default constructor
  Object();
  
  // Move the object to (x, y) in screen coordinates
  void move(GLfloat nx, GLfloat ny);

  // Move the object to where in screen coordinates
  void move(vec2 where);

  // Change the object's color to r, g, b value
  void color(GLfloat nr, GLfloat ng, GLfloat nb);

  // Change the object's color to ncolor's value
  void color(vec3 ncolor);

  // Get the object's color as a vec3
  vec3 getColor();

  // Change the object's select color to r, g, b value
  void selectColor(GLfloat nr, GLfloat ng, GLfloat nb);

  // Change the object's select color to ncolor's value
  void selectColor(vec3 ncolor);

  // Get the object's select color as a vec3
  vec3 getSelectColor();

  // Change the size of the object.
  void change_size(GLfloat size);

  // Get the current object's position
  vec2 get_pos();

  // Update the last updated time to now.
  void set_last_time();

  // Reset the simulation time for this object to 0.
  void reset_time();

  // Returns the time since the last update
  GLint compute_time();

  // Sets flag that this object is selected
  void Selected();

  // Clears flag that this object is selected
  void notSelected();

  // Returns the selected state of this object
  bool GetSelected();

  inline bool isSelected(){return selected;};

  inline GLfloat getSpeed(){return(speed);};
  inline void setSpeed(GLfloat movementspeed){ speed = movementspeed;};

  GLfloat get_size();


protected:
  GLfloat size; // size of object (see individual objects for specific meaning)

  // location of object (x, y)
  GLfloat x;      
  GLfloat y; 

  // Color of object
  GLfloat r;
  GLfloat g;
  GLfloat b;
  
  //Speed of the object
  GLfloat speed;

  // Color for selection drawing of object
  GLfloat sr;
  GLfloat sg;
  GLfloat sb;

  // Start of vertices to draw in VAO
  GLuint index;
  // Pointer to the points array
  vec2 *points;

  // Locations of variables in shaders.
  // Offset of object
  GLint offsetLoc;
  // Size of object
  GLint sizeLoc;
  // Color of object 
  GLint colorLoc;
  
  // For smooth animation consistent across machines
  GLint last_time;
  GLint current_time;

  // Whether the current object is selected
  bool selected;
};

// Comparator for unsigned vs. float colors
// Returns true if they are the same color.
bool cmpcolor(unsigned char colora[], vec3 colorb);

#endif
