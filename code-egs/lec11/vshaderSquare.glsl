// Basic shader with offset to move an object, and scaling to work
// with resized windows.
//
// Written by dmc 
// last-modified: Wed Sep 28 11:37:34 2016

attribute  vec4 vPosition;
//attribute  vec4 vColor;
//varying vec4 color;

uniform ivec2 offset;
uniform float size;
uniform vec2 windowSize;

void main() 
{
  gl_Position.x=(2.0*(size*(vPosition.x)+float(offset.x)))/windowSize.x-1.0;
  gl_Position.y=(2.0*(size*(vPosition.y)+float(offset.y)))/windowSize.y-1.0;
  //  gl_Position.x=(2.0*(size*(vPosition.x+float(offset.x))))/windowSize.x-1.0;
  //  gl_Position.y=(2.0*(size*(vPosition.y+float(offset.y))))/windowSize.y-1.0;

  gl_Position.z=0.0;
  gl_Position.w=1.0;
}
