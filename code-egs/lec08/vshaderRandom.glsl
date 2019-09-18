attribute vec4 vPosition;
uniform vec4 vColor;
varying vec4 fColor;

void main()
{
  gl_Position.x = (1.0/300.0)*(vPosition.x-250.0);
  gl_Position.y = (1.0/300.0)*(vPosition.y-250.0);
  gl_Position.z=0.0;
  gl_Position.w = 1.0;

  fColor=vColor;
}
