#version 460 core
layout (location = 0) in vec4 aPos; 
layout (location = 1) in vec4 vNormal; 
layout (location = 2) in vec4 vTan; 
layout (location = 3) in vec2 uv;
layout (location = 4) in vec4 instance_M0; 
layout (location = 5) in vec4 instance_M1; 
layout (location = 6) in vec4 instance_M2; 
layout (location = 7) in vec4 instance_M3;

out vec4 vertexNormal;
out vec4 vertexTangent;
out vec4 vertexPos;
out vec4 vertexCol;
out vec2 texcoords;

void main()
{
    // model matrix

    // view matrix

    // projection matrix

    texcoords = uv;

    vertexPos = aPos;
    vertexNormal = vNormal;
    vertexTangent = vTan;   
    vertexCol = vNormal;

    gl_Position = vertexPos; // see how we directly give a vec3 to vec4's constructor
}
