varying  vec4 color;
varying  vec2 texCoord;

uniform sampler2D texture;
uniform bool blend;

void main() 
{
  if (blend) {
    gl_FragColor = color * texture2D(texture, texCoord);
  } else {
    gl_FragColor = texture2D(texture, texCoord);
  }
} 

