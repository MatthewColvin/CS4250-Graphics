attribute vec4 vPosition;

varying vec4 fcolor;

uniform ivec2 offset;
uniform float size;
uniform vec2 windowSize;
uniform vec4 vcolor;

void main() 
{
  gl_Position.x=  ( 2.0 * (size*(vPosition.x) + float(offset.x) ))  /  windowSize.x-1.0;
  gl_Position.y=  ( 2.0 * (size*(vPosition.y) + float(offset.y) ))  /  windowSize.y-1.0;
  gl_Position.z= 0.0;
  gl_Position.w= 1.0;

  fcolor = vcolor;
}
