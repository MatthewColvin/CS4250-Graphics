attribute vec4 vPosition;

void main()
{
  gl_Position.x = (1.0/500.0)*(vPosition.x-400.0);
  gl_Position.y = (1.0/500.0)*(vPosition.y-400.0);
  gl_Position.z = (1.0/500.0)*(vPosition.z-400.0);
  gl_Position.w = 1.0;
}
