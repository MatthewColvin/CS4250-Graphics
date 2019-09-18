// output values that will be interpretated per-fragment
varying  vec3 fN;
varying  vec3 fE;
varying  vec3 fL;

uniform vec4 LightPosition;

attribute  vec4 vPosition;
attribute  vec3 vNormal;
attribute  vec2 vTexCoord;

varying vec4 color;
varying vec2 texCoord;

uniform vec3 theta;

uniform mat4 ModelView;
uniform mat4 Projection;

void main() 
{
  fN = (ModelView*vec4(vNormal,0)).xyz;
  fE = vPosition.xyz;
  fL = LightPosition.xyz;
    
  if(LightPosition.w != 0.0) {
    fL = LightPosition.xyz - vPosition.xyz;
  }

  color       = vec4(1.0,1.0,1.0,1.0); // white is default
  texCoord    = vTexCoord;
  gl_Position = Projection*ModelView*vPosition;
}
