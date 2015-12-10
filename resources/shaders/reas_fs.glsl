#version 330 core

in vec4 color;
in vec4 auxColor;

void main()
{
    float t = 0.3;
    gl_FragColor = vec4((t * color.rgb) + ((1.0-t) * auxColor.rgb), color.a);
    //gl_FragColor = auxColor;
    //gl_FragColor = vec4( 1.0 );
}
