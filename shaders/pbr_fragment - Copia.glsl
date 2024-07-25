# version 460 core

layout(location = 0) out vec4 FragColor;

in vec4 vertexNormal; // the input variable from the vertex shader (same name and same type)  
in vec4 vertexTangent; // the input variable from the vertex shader (same name and same type)  
in vec4 vertexPos; // the input variable from the vertex shader (same name and same type)  
in vec4 worldPos; // the input variable from the vertex shader (same name and same type)  
in vec4 vertexCol; // the input variable from the vertex shader (same name and same type)  
in vec2 texcoords; // the input variable from the vertex shader (same name and same type)  
in mat4 fmode_m;
in float texcoords_factor;

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

uniform bool is_instance;

#define PI 3.1415926535897932384626433832795028841971
#define e 2.718281828459045235360287
#define solid_color VectorParameter0
#define camera_pos VectorParameter1
#define camera_dir VectorParameter2
#define object_pos VectorParameter3
#define d_factor (VectorParameter4.x / 100.0)
#define s_factor (VectorParameter4.y / 100.0)
#define r_factor (VectorParameter4.z / 100.0)
#define specular_q (VectorParameter4.w)
#define wave_dir VectorParameter5
#define wave_amp FloatParameter1
#define wave_frq FloatParameter2
#define time FloatParameter0

vec4 light_direction = normalize(vec4(-2.0,-0.7,1.5,0.0));
vec4 light_color = vec4(1.0, 0.9, 0.6, 1.0) * 1.5;
vec4 camera_vector = normalize(camera_pos - worldPos);

float luminance(vec4 rgba_color){
	return ((rgba_color.x + rgba_color.y + rgba_color.z) / 3.0) * rgba_color.w;
}

float random (vec2 st) {
    return fract(sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123);
}

vec4 normalRecompute(vec2 coord){
	vec4 biTan = normalize(vec4(cross(vertexNormal.xyz, vertexTangent.xyz), 0.0));
	vec4 n_normal;
	vec4 n_tangent = vec4(0.0);
	vec4 n_bitangent = vec4(0.0);

	float f = wave_frq;
    float a = wave_amp;
    float ddx = 0.0;
    float ddz = 0.0;
    float acc_ddx = 0.0;
    float acc_ddz = 0.0;
    int i = 1;

    float fc = clamp(1.0, 0.0, clamp(5000.0, 0.0, distance(camera_pos, worldPos) - 500.0) / 500.0);
    float ep = mix(0.000001, 0.0001, fc);

    while(i>0 && a>ep){
    	vec2 cur_coord = coord + vec2(ddx,ddz);
    	vec2 wd = normalize(vec2(sin(random(vec2(a,i)) * PI * 2.0), cos(random(vec2(a,i)) * PI * 2.0)));
    	vec2 td = wd * time * random(vec2(a,i)) * 0.25;
        ddx = a * wd.x * f * pow(e, sin(f * dot(wd, cur_coord - td))-1.0) * cos(f * dot(wd, cur_coord - td));
        ddz = a * wd.y * f * pow(e, sin(f * dot(wd, cur_coord - td))-1.0) * cos(f * dot(wd, cur_coord - td));
        acc_ddx += ddx;
        acc_ddz += ddz;
        f *= 1.18;
        a *= 0.83;
        i--;
    }

    n_tangent 	= normalize(vec4(1.0, acc_ddx, 0.0, 0.0));
    n_bitangent = normalize(vec4(0.0, acc_ddz, 1.0, 0.0));
	n_normal = vec4(normalize(cross(n_bitangent.xyz,n_tangent.xyz)), 0.0);
	//n_normal = vec4(n_normal.x, -n_normal.y, n_normal.z, n_normal.w);

	return n_normal;
}

void main()
{
	vec4 displaceNormal = vertexNormal;
	displaceNormal = normalRecompute(vertexPos.xz * texcoords_factor);

	vec4 color;
	vec4 diffuse;
	vec4 specular;
	vec4 reflection;

	vec4 phong_angle = normalize(light_direction + 2 * displaceNormal * dot(displaceNormal, -light_direction));
	
	vec4 reflection_angle = normalize(-camera_vector + 2 * displaceNormal * dot(displaceNormal, camera_vector));
	
	vec2 envtex_coords = vec2(
		1.0 - (atan(reflection_angle.z, reflection_angle.x) + PI) / (2.0*PI),
		(asin(reflection_angle.y/1.0) + (PI/2.0)) / PI
	);

	float fresnel_f = pow((1.0-dot(displaceNormal, camera_vector)), 5);

	float lambt = max(dot(-displaceNormal, light_direction), 0.0);

	diffuse = vertexCol * lambt * d_factor;

	specular = light_color * pow(dot(phong_angle, camera_vector) * 0.5 + 0.5, specular_q) * lambt * mix(s_factor, 1.0, fresnel_f);

	reflection = texture(TextureImage1, envtex_coords) * mix(r_factor, 1.0, fresnel_f);

	color = diffuse + (specular + reflection);

	float f = clamp(1.0, 0.0, clamp(5000.0, 0.0, distance(camera_pos, worldPos) - 1400.0) / 500.0);

	FragColor = mix(vec4(1.0, 0.9, 0.6, 1.0)*1.5, color,pow(1.0-f,1));
	FragColor = displaceNormal;
}