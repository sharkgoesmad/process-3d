#version 330 core

layout( location = 0 ) in vec3 attribPosition;
layout( location = 1 ) in vec4 attribColor;
layout( location = 2 ) in mat4 attribInstanceTransform;
layout( location = 6 ) in vec4 attribInstanceColor;

uniform mat4 uniformVP;

out vec4 color;
out vec4 auxColor;

void main()
{
    mat4 mvp = uniformVP * attribInstanceTransform;
    gl_Position = mvp * vec4( attribPosition, 1.0 );
    color = attribColor;
    auxColor = attribInstanceColor;
}


