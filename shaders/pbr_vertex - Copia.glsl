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

vec4 displace(vec4 pos, vec2 coord){

    vec4 n_normal;
    vec4 n_tangent = vec4(0.0);
    vec4 n_bitangent = vec4(0.0);

    float ddx = 0.0;
    float ddz = 0.0;
    float acc_ddx = 0.0;
    float acc_ddz = 0.0;

    float f = wave_frq;
    float a = wave_amp;
    float amt = 0.0;
    int t = 1;

    float fc = clamp(1.0, 0.0, clamp(5000.0, 0.0, distance(camera_pos, aPos) - 500.0) / 500.0);
    float ep = mix(0.000001, 0.0001, fc);

    while(t>0 && a>ep){
        vec2 cur_coord = coord + vec2(ddx,ddz);
        vec2 wd = normalize(vec2(sin(random(vec2(a,t)) * PI * 2.0), cos(random(vec2(a,t)) * PI * 2.0)));
        vec2 td = wd * time * random(vec2(a,t)) * 0.25;
        
        amt += a * pow(e, sin(f * dot(wd, cur_coord - td))-1.0);
        ddx = a * wd.x * f * pow(e, sin(f * dot(wd, cur_coord - td))-1.0) * cos(f * dot(wd, cur_coord - td));
        ddz = a * wd.y * f * pow(e, sin(f * dot(wd, cur_coord - td))-1.0) * cos(f * dot(wd, cur_coord - td));
        acc_ddx += ddx;
        acc_ddz += ddz;

        f *= 1.18;
        a *= 0.83;
        t--;
    }

    n_tangent   = normalize(vec4(1.0, acc_ddx, 0.0, 0.0));
    n_bitangent = normalize(vec4(0.0, acc_ddz, 1.0, 0.0));
    n_normal = vec4(normalize(cross(n_bitangent.xyz,n_tangent.xyz)), 0.0);

    vertexNormal = n_normal;
    vertexTangent = n_tangent;

    return pos + vNormal * amt;
}

void main()
{
    texcoords = (uv * 5.0) - time;
    texcoords_factor = uv_factor;

    vertexNormal = normalize(inverse(transpose(mode_m)) * vNormal);
    vertexTangent = normalize(inverse(transpose(mode_m)) * vTan);
    
    vertexPos = displace(aPos, aPos.xz * uv_factor);

    fmode_m = mode_m;
    worldPos = mode_m * vertexPos;

    vertexCol = vec4(0.0, 1.0, 0.85, 1.0);
    //vertexCol = vec4(0.5, 0.5, 0.5, 1.0);
    gl_Position = proj_m * view_m * worldPos; // see how we directly give a vec3 to vec4's constructor
}
