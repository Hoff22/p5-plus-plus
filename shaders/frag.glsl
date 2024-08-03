# version 460 core

layout(location = 0) out vec4 FragColor;

in vec4 vertexPos; // the input variable from the vertex shader (same name and same type)  
in vec2 screencoords;

layout(std430, binding = 0) buffer bufferData{
    int cell_count;
    uint state[10000]; // actually carries 4x amount of cells
};


const int pallet_offset = 8356;
const uint colors[100] = {
    0xB9D7D9FF,
    0x668284FF,
    0x2A2829FF,
    0x493736FF,
    0x7B3B3BFF,
    0xD3D5B0FF,
    0xB5CEA4FF,
    0x9DC19DFF,
    0x8C7C62FF,
    0x71443FFF,
    0xFFEFD3FF,
    0xFFFEE4FF,
    0xD0ECEAFF,
    0x9FD6D2FF,
    0x8B7A5EFF,
    0x4D3B3BFF,
    0xDE6262FF,
    0xFFB88CFF,
    0xFFD0B3FF,
    0xF5E0D3FF,
    0x1B676BFF,
    0x519548FF,
    0x88C425FF,
    0xBEF202FF,
    0xEAFDE6FF,
    0x379F7AFF,
    0x78AE62FF,
    0xBBB749FF,
    0xE0FBACFF,
    0x1F1C0DFF,
    0xF0D8A8FF,
    0x3D1C00FF,
    0x86B8B1FF,
    0xF2D694FF,
    0xFA2A00FF,
    0x23192DFF,
    0xFD0A54FF,
    0xF57576FF,
    0xFEBF97FF,
    0xF5ECB7FF,
    0xF2E8C4FF,
    0x98D9B6FF,
    0x3EC9A7FF,
    0x2B879EFF,
    0x616668FF,
    0x805841FF,
    0xDCF7F3FF,
    0xFFFCDDFF,
    0xFFD8D8FF,
    0xF5A2A2FF,
    0x1D1313FF,
    0x24B694FF,
    0xD22042FF,
    0xA3B808FF,
    0x30C4C9FF,
    0xA69E80FF,
    0xE0BA9BFF,
    0xE7A97EFF,
    0xD28574FF,
    0x3B1922FF,
    0xE4DED0FF,
    0xABCCBDFF,
    0x7DBEB8FF,
    0x181619FF,
    0xE32F21FF,
    0x1C2130FF,
    0x028F76FF,
    0xB3E099FF,
    0xFFEAADFF,
    0xD14334FF,
    0x452E3CFF,
    0xFF3D5AFF,
    0xFFB969FF,
    0xEAF27EFF,
    0x3B8C88FF,
    0xCC0C39FF,
    0xE6781EFF,
    0xC8CF02FF,
    0xF8FCC1FF,
    0x1693A7FF,
    0x5E9FA3FF,
    0xDCD1B4FF,
    0xFAB87FFF,
    0xF87E7BFF,
    0xB05574FF,
    0x4D3B3BFF,
    0xDE6262FF,
    0xFFB88CFF,
    0xFFD0B3FF,
    0xF5E0D3FF,
    0x027B7FFF,
    0xFFA588FF,
    0xD62957FF,
    0xBF1E62FF,
    0x572E4FFF,
    0x99B898FF,
    0xFECEA8FF,
    0xFF847CFF,
    0xE84A5FFF,
    0x2A363BFF
};

uint getValueAt(ivec2 pos){
    int byte_idx = pos.y * cell_count + pos.x;
    int int_idx = byte_idx / 4;
    int off = byte_idx % 4;

    uint mask = ((1<<8)-1);

    return (state[int_idx] >> (off * 8)) & mask;
}

vec4 getColor(uint i){
    return vec4(
        1.0 * ((colors[(i + pallet_offset) % 100] / 256 / 256 / 256) % 256) / 255.0,
        1.0 * ((colors[(i + pallet_offset) % 100] / 256 / 256) % 256) / 255.0,
        1.0 * ((colors[(i + pallet_offset) % 100] / 256) % 256) / 255.0,
        1.0 * ((colors[(i + pallet_offset) % 100]) % 256) / 255.0
    );
}

void main()
{
    //FragColor = vec4(texcoords, 0.0, 1.0);

    vec2 sc = screencoords;

    ivec2 pos = ivec2(floor(sc.x * cell_count), floor(sc.y * cell_count));

    vec4 col1 = vec4(vec2(pos.x, pos.y) / cell_count, 0.0 , 1.0);
    FragColor = normalize(vec4(getColor(getValueAt(pos))) + col1 * 0.0);

    // int byte_idx = pos.y * cell_count + pos.x;
    // int off = byte_idx % 4;

    // if(off == 0){
    //     FragColor += vec4(1.0,0.0,0.0,1.0);
    // }
    // else if(off == 1){
    //     FragColor += vec4(0.0,1.0,0.0,1.0);
    // }
    // else if(off == 2){
    //     FragColor += vec4(0.0,0.0,1.0,1.0);
    // }
    // else{
    //     FragColor += vec4(1.0,0.0,1.0,1.0);
    // }

    // FragColor = vec4(normalize(FragColor.xyz), 1.0);
}