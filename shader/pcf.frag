#version 460

uniform struct LightInfo
{
	vec4 Position;
	vec3 Intensity;
} Light;

uniform struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
} Material;

uniform sampler2DShadow ShadowMap;

in vec3 Normal;
in vec3 Position;
in vec4 ShadowCoord;

layout(location = 0) out vec4 FragColor;

vec3 phongModelDiffuseAndSpecular()
{
	vec3 n = Normal;
	vec3 s = normalize(vec3(Light.Position) - Position);
	vec3 v = normalize(-Position.xyz);
	vec3 r = reflect(-s, n);
	float sDotN = max(dot(s, n), 0.0);
	vec3 diffuse = Light.Intensity * Material.Kd * sDotN;
	vec3 specular = vec3(0.0);
	if(sDotN > 0.0)
	{
		specular = Light.Intensity * Material.Ks * 
			pow(max(dot(r, v), 0.0), Material.Shininess);
	}
	return diffuse + specular;
}

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

subroutine(RenderPassType)
void shadeWithShadow()
{
	vec3 ambient = vec3(0.2);
	vec3 diffuseAndSpecular = phongModelDiffuseAndSpecular();

	// The sum of the comparisons with nearby texels
	float sum = 0;

	// Sum contributions from texels around ShadowCoord
	sum += textureProjOffset(ShadowMap, ShadowCoord, ivec2(-1, -1));
	sum += textureProjOffset(ShadowMap, ShadowCoord, ivec2(-1, 1));
	sum += textureProjOffset(ShadowMap, ShadowCoord, ivec2(1, 1));
	sum += textureProjOffset(ShadowMap, ShadowCoord, ivec2(1, -1));

	float shadow = sum * 0.25f;
	
	FragColor = vec4(ambient + diffuseAndSpecular * shadow, 1.0);
}

subroutine(RenderPassType)
void recordDepth()
{
	// Depth records automatically
}

void main()
{
	RenderPass();
}