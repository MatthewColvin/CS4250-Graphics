attribute   vec4 vPosition;
attribute   vec4 vNormal;
attribute  vec2 vTexCoord;

uniform vec3 theta;

// output values that will be interpretated per-fragment
varying  vec3 fN;
varying  vec3 fE;
varying  vec3 fL;
varying vec2 texCoord;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

void main()
{
    const float  DegreesToRadians = 3.14159265 / 180.0;
    
    vec3 c = cos( DegreesToRadians * theta );
    vec3 s = sin( DegreesToRadians * theta );

    mat4 rx = mat4( 1.0, 0.0,  0.0, 0.0,
		    0.0, c.x, -s.x, 0.0,
		    0.0, s.x,  c.x, 0.0,
		    0.0, 0.0,  0.0, 1.0);

    mat4 ry = mat4(   c.y, 0.0, s.y, 0.0,
		      0.0, 1.0, 0.0, 0.0,
		     -s.y, 0.0, c.y, 0.0,
		      0.0, 0.0, 0.0, 1.0 );

    
    mat4 rz = mat4( c.z, -s.z, 0.0, 0.0,
		    s.z,  c.z, 0.0, 0.0,
		    0.0,  0.0, 1.0, 0.0,
		    0.0,  0.0, 0.0, 1.0 );

    mat4 mv = rz * ry * rx;
  fN = (mv * vNormal).xyz;
    fE = vPosition.xyz;
    fL = LightPosition.xyz;
    
    if(LightPosition.w != 0.0) {
	fL = LightPosition.xyz - vPosition.xyz;
    }

    gl_Position = Projection*ModelView*mv*vPosition;
    texCoord    = vTexCoord;
}
