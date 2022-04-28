#version 460

layout(binding = 0) uniform sampler2D NoiseTex;

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform float LowThreshold;
uniform float HighThreshold;

layout(location = 0) out vec4 FragColor;

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

vec3 phongModel(vec4 position, vec3 normal)
{
	vec3 ambient = Light.La * Material.Ka;
	vec3 s = normalize(Light.Position.xyz - position.xyz);
	float sDotN = max(dot(s, normal), 0.0);
	vec3 diffuse = Material.Kd * sDotN;

	vec3 specular = vec3(0.0);
	if(sDotN > 0.0)
	{
		vec3 v = normalize(-position.xyz);
		vec3 h = normalize(v + s);
		specular = Material.Ks * pow(max(dot(h, normal), 0.0), Material.Shininess);
	}
	return ambient + Light.L * (specular + diffuse);
}

void main()
{
	// Get the noise value at TexCoord
	vec4 noise = texture(NoiseTex, TexCoord);

	if(noise.a < LowThreshold || noise.a > HighThreshold)
		discard;

	// olour the fragment using the shading model
	vec3 colour = phongModel(Position, Normal);
	FragColor = vec4(colour, 1.0);
}