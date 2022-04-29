#version 430

layout(binding = 0) uniform sampler2D NoiseTex;

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform struct LightInfo
{
	vec4 Position;
	vec3 L;
	vec3 La;
} Light;

uniform struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
} Material;

vec3 phongModel(vec3 kd)
{
	vec3 ambient = Material.Ka * Light.La;
	vec3 n = Normal;
    vec3 s = normalize(Light.Position.xyz - Position.xyz);
    vec3 v = normalize(-Position.xyz);
    vec3 r = reflect( -s, n );
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = Light.L * kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.L * Material.Ks *
            pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return ambient + diffuse + spec;
}

// The paint-spatter uniforms
uniform vec3 PaintColour = vec3(1.0);
uniform float Threshold = 0.65;

layout(location = 0) out vec4 FragColor;

void main()
{
    vec4 noise = texture(NoiseTex, TexCoord);
    vec3 colour = Material.Kd;

    if(noise.g > Threshold)
        colour = PaintColour;
    
    FragColor = vec4(phongModel(colour), 1.0);
}