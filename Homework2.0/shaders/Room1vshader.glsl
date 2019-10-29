attribute vec4 vPosition;

varying vec4 fcolor;

uniform ivec2 offset;
uniform float size;
uniform vec2 windowSize;
attribute vec4 vcolor;

void main() 
{
  gl_Position.x = vPosition.x / windowSize.x;
  gl_Position.y = vPosition.y / windowSize.y;
  gl_Position.z = vPosition.z;
  gl_Position.w=1.0;

  fcolor = vcolor;
}
