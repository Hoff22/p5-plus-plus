# version 460 core

layout(location = 0) out vec4 FragColor;

in vec4 vertexNormal; // the input variable from the vertex shader (same name and same type)  
in vec4 vertexTangent; // the input variable from the vertex shader (same name and same type)  
in vec4 vertexPos; // the input variable from the vertex shader (same name and same type)  
in vec4 vertexCol; // the input variable from the vertex shader (same name and same type)  
in vec2 texcoords; // the input variable from the vertex shader (same name and same type)  
in vec2 screencoords;

uniform vec2 screen_size;

layout(std430, binding = 0) buffer bufferData{
    ivec4 shapeData[64];
    vec4 colorData[64];
    int shapes_end;
};

void main()
{
    //FragColor = vec4(texcoords, 0.0, 1.0);

    FragColor = vec4(vec3(0.12),1.0);
    //FragColor = vec4(vec3(screencoords.x / screen_size.x),1.0);
    
    for(int i = 0; i < shapes_end; i++){
        int x = shapeData[i].x;
        int y = shapeData[i].y;
        int size_x = shapeData[i].z;
        int size_y = shapeData[i].w;

        // vec4 col = vec4(shapes_color[i * 4], shapes_color[i * 4 + 1], shapes_color[i * 4 + 2], shapes_color[i * 4 + 3]);
        vec4 col = colorData[i];

        if(x <= screencoords.x && screencoords.x <= x+size_x && y <= screencoords.y && screencoords.y <= y+size_y){
            FragColor = col;
        }
    }

}