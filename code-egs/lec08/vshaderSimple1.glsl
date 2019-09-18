attribute vec4 vPosition;

void main()
{
  /*
  gl_Position.x = (1.0/250.0)*(vPosition.x-250.0);
  gl_Position.y = (1.0/250.0)*(vPosition.y-250.0);
  gl_Position.z = 0.0;
  gl_Position.w = 1.0;
  */

  /*
  gl_Position.x = (1.0/500.0)*(vPosition.x-500.0);
  gl_Position.y = (1.0/500.0)*(vPosition.y-500.0);
  gl_Position.z = (1.0/500.0)*(vPosition.z-500.0);
  gl_Position.w = 1.0;
  */

  gl_Position.x = (1.0/500.0)*(vPosition.x-250.0);
  gl_Position.y = (1.0/500.0)*(vPosition.y-250.0);
  gl_Position.z = (1.0/500.0)*(vPosition.z-250.0);
  gl_Position.w = 1.0;

}
