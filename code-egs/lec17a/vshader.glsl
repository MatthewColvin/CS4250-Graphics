attribute   vec4 vPosition;
attribute   vec4 vNormal;

// output values that will be interpretated per-fragment
varying  vec3 fN;
varying  vec3 fE;
varying  vec3 fL;
varying  vec3 fL2;

uniform mat4 Camera;
uniform mat4 ModelView;
uniform mat4 Projection;

uniform vec4 LightPosition;
uniform vec4 LightPosition2;

void main()
{
  mat4 mv = Camera*ModelView;
  fN = (mv*vNormal).xyz;
  fE = (mv*vPosition).xyz;

  // Lights aren't moved with object, so only camera transform
  // (Camera) needs to be applied to them.
  fL = (Camera*LightPosition).xyz;
  fL2 = (Camera*LightPosition2).xyz;

  gl_Position = Projection*mv*vPosition;
}
