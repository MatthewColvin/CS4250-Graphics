varying vec3 R;
uniform samplerCube texMap;

void main()
{
     vec4 texColor = textureCube(texMap, R);
     texColor.a=1.0;

    gl_FragColor = texColor;
}
