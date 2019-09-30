attribute vec4 TrianglePos;
attribute vec4 vColor;
varying vec4 fColor;

void main()
{
  gl_Position.x = (1.0/500.0)*(TrianglePos.x);
  gl_Position.y = (1.0/500.0)*(TrianglePos.y-200.0);
  gl_Position.z = (1.0/500.0)*(TrianglePos.z);
  gl_Position.w = 2.0;

  fColor = vColor;


}
