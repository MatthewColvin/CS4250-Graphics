attribute  vec4 vPosition;

varying vec4 color;

uniform mat4 ModelView;
uniform mat4 Projection;

void main() 
{
  color       = vec4(0.0,0.0,0.0,1.0); // black is default
  gl_Position = Projection*ModelView*vPosition;
}
