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
out vec2 screencoords;

uniform vec2 screen_size;

layout(std430, binding = 0) buffer bufferData{
    ivec4 shapeData[64];
    vec4 colorData[64];
    int shapes_end;
};

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

    screencoords = screen_size * ((vertexPos.xy + vec2(1.0)) / 2);

    gl_Position = vertexPos; // see how we directly give a vec3 to vec4's constructor
}
