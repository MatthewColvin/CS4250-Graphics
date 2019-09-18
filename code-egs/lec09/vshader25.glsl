uniform mat4  Proj;
uniform vec4 vColor;
varying vec4 fColor;
attribute vec4 vPosition;

void
main()
{
     gl_Position = Proj * vPosition;
     fColor = vColor;
}
