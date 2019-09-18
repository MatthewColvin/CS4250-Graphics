attribute  vec4 vPosition;
varying vec4 color;

uniform vec2 windowSize;
uniform  vec4 vColor;

void main() 
{
  gl_Position.x=(2.0*(vPosition.x))/windowSize.x-1.0;
  gl_Position.y=(2.0*(vPosition.y))/windowSize.y-1.0;
  gl_Position.z=0.0;
  gl_Position.w=1.0;
  color = vColor;
  color.a = 1.0;
}
