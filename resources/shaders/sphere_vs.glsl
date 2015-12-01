#version 330 core

layout( location = 0 ) in vec3 attribPosition;
layout( location = 1 ) in vec4 attribColor;
layout( location = 2 ) in mat4 attribTransform;

uniform mat4 uniformVP;

out vec4 color;

void main()
{
    mat4 mvp = uniformVP * attribTransform;
    gl_Position = mvp * vec4( attribPosition, 1.0 );
    color = attribColor;
}


