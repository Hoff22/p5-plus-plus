#version 460 core
layout (location = 0) in vec4 aPos; 

out vec4 vertexPos;
out vec2 screencoords;

void main()
{
    vertexPos = aPos; 

    screencoords = (vertexPos.xy + vec2(1.0)) / 2;

    gl_Position = vertexPos;
}
