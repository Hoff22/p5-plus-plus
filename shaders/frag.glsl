# version 460 core

layout(location = 0) out vec4 FragColor;

in vec4 vertexCol; // the input variable from the vertex shader (same name and same type)  
in vec2 screencoords;

void main()
{
    //FragColor = vec4(texcoords, 0.0, 1.0);
    FragColor = vec4(0.2, 1.0, 0.0, 1.0);
}