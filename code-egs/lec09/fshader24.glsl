varying  vec4 color;

void
main()
{
    gl_FragColor = color;
    gl_FragColor.r-=0.4;
}
