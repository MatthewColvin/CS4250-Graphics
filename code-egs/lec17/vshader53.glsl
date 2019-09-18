attribute  vec4 vPosition;
attribute  vec3 vNormal;
varying vec4 color;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;

// Bool expressed as an int to pass to shader
uniform bool Spec;

void main()
{
  // Transform vertex  position into eye coordinates
  vec3 pos = (ModelView * vPosition).xyz;
  //    vec3 pos = (vPosition).xyz;
	
  vec3 L;
  if (LightPosition.w == 1.0) {
    L = normalize(LightPosition.xyz - pos);
  }else {
    L = normalize((LightPosition).xyz);
    //    L = normalize((ModelView*LightPosition).xyz);
  }

  vec3 E = normalize(-pos);

  vec3 H = normalize(L + E);

  // Transform vertex normal into eye coordinates
  vec3 N = normalize(ModelView*vec4(vNormal, 0.0)).xyz;

  // What would the following do?
  //vec3 N = normalize(vec4(vNormal, 0.0)).xyz;

  // Compute terms in the illumination equation
  vec4 ambient = AmbientProduct;
  //    ambient = vec4(0.0, 0.0, 0.0, 1.0);

  float Kd = max(dot(L, N), 0.0);
  vec4  diffuse = Kd*DiffuseProduct;
  //    diffuse = vec4(0.0, 0.0, 0.0, 1.0);

  float Ks = pow( max(dot(N, H), 0.0), Shininess );
  vec4  specular = Ks * SpecularProduct;
  //    specular = vec4(0.0, 0.0, 0.0, 0.0);
    
  if(dot(L, N) < 0.0) {
    specular = vec4(0.0, 0.0, 0.0, 1.0);
  } 
  if (!Spec) {
    specular = vec4(0.0, 0.0, 0.0, 1.0);
  }

  gl_Position = Projection * ModelView * vPosition;

  color = ambient + diffuse + specular;
  color.a = 1.0;
}
