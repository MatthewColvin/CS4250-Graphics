// per-fragment interpolated values from the vertex shader
varying  vec3 fN;
varying  vec3 fL;
varying  vec3 fE;

// Texture variables
varying  vec2 texCoord;
uniform sampler2D texture;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform float Shininess;

// For light sources.
uniform vec4 EmmissiveColor;
uniform bool EmmissiveP;
uniform bool blend;

void main() 
{ 
  if (EmmissiveP) {
    gl_FragColor = EmmissiveColor;
  } else {
    // Normalize the input lighting vectors
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);

    vec3 H = normalize(L + E);
    
    vec4 ambient = AmbientProduct;

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DiffuseProduct;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularProduct;

    // discard the specular highlight if the light's behind the vertex
    if(dot(L, N) < 0.0) {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
    }

    //    gl_FragColor =
    //    (1.0/(0.25+0.25*pow(length(fL),2.0)+0.25*length(fL)))*(ambient
    //    + diffuse + specular);
  if (blend) {
    gl_FragColor = (ambient + diffuse + specular)*texture2D(texture, texCoord);
  } else {
    gl_FragColor = Kd*texture2D(texture, texCoord);
  }
    gl_FragColor.a = 1.0;
  }
} 

