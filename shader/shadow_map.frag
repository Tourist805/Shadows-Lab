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
	vec3 ambient = Light.Intensity * Material.Ka;
	vec3 diffuseAndSpecular = phongModelDiffuseAndSpecular();

	float shadow = 1.0;
	if(ShadowCoord.z >= 0)
	{
		shadow = textureProj(ShadowMap, ShadowCoord);
	}

	FragColor = vec4(diffuseAndSpecular * shadow + ambient, 1.0);

	FragColor = pow(FragColor, vec4(1.0 / 2.2));
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