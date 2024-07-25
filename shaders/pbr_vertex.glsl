#version 460 core
layout (location = 0) in vec4 aPos; 
layout (location = 1) in vec4 vNormal; 
layout (location = 2) in vec4 vTan; 
layout (location = 3) in vec2 uv;
layout (location = 4) in vec4 instance_M0; 
layout (location = 5) in vec4 instance_M1; 
layout (location = 6) in vec4 instance_M2; 
layout (location = 7) in vec4 instance_M3;

mat4 inst_m = mat4(instance_M0, instance_M1, instance_M2, instance_M3); 

uniform mat4 mode_m;
uniform mat4 view_m;
uniform mat4 proj_m;

uniform float time;

uniform bool is_instance;

uniform float FloatParameter0;
uniform float FloatParameter1;
uniform float FloatParameter2;
uniform float FloatParameter3;
uniform float FloatParameter4;
uniform float FloatParameter5;

uniform vec4 VectorParameter0;
uniform vec4 VectorParameter1;
uniform vec4 VectorParameter2;
uniform vec4 VectorParameter3;
uniform vec4 VectorParameter4;
uniform vec4 VectorParameter5;

uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;

#define PI 3.14159265359
#define e 2.718281828459045235360287
#define camera_pos VectorParameter1
#define wave_dir VectorParameter5
#define wave_amp FloatParameter1
#define wave_frq FloatParameter2
#define wave_speed FloatParameter4
#define uv_factor FloatParameter3
#define time FloatParameter0
//#define f 100
//#define n 0.1

out vec4 vertexNormal;
out vec4 vertexTangent;
out vec4 vertexPos;
out vec4 worldPos;
out vec4 vertexCol;
out vec2 texcoords;
out mat4 fmode_m;
out float texcoords_factor;


float luminance(vec4 rgba_color){
    return ((rgba_color.x + rgba_color.y + rgba_color.z) / 3.0) * rgba_color.w;
}

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

vec2 randomVec(vec2 st){
    float a = random(st) * 2.0 * PI;
    return vec2(sin(a), cos(a));
}

vec4 displace(vec2 coord){
    float f = wave_frq;
    float a = wave_amp;
    float amt = 0.0;
    int i = 35;

    float fc = clamp(1.0, 0.0, clamp(5000.0, 0.0, distance(camera_pos, aPos) - 500.0) / 500.0);
    float ep = mix(0.0001, 0.0001, fc);

    while(i>0 && a>ep){
        vec2 w = randomVec(vec2(a,i));
        vec2 t = randomVec(vec2(i,a)) * time * (1.0/i) * wave_speed;
        amt += a * pow(e, sin(f * dot(w, coord - t))-1.0);
        f *= 1.18;
        a *= 0.75;
        i--;
    }

    return vNormal * amt;
}

void main()
{
    texcoords = (uv * 5.0) - time;
    texcoords_factor = uv_factor;

    vertexNormal = normalize(inverse(transpose(mode_m)) * vNormal);
    vertexTangent = normalize(inverse(transpose(mode_m)) * vTan);
    
    vertexPos = aPos + displace(aPos.xz * uv_factor);

    fmode_m = mode_m;
    worldPos = mode_m * vertexPos;

    vertexCol = vec4(0.0, 1.0, 0.85, 1.0);
    //vertexCol = vec4(0.5, 0.5, 0.5, 1.0);
    gl_Position = proj_m * view_m * worldPos; // see how we directly give a vec3 to vec4's constructor
}
