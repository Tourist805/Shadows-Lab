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
uniform sampler3D OffsetTex;

uniform float Radius;
uniform vec3 OffsetTexSize; // Width, Height, Depth

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

	ivec3 offsetCoord;
	offsetCoord.xy = ivec2(mod(gl_FragCoord.xy, OffsetTexSize.xy));

	float sum = 0.0, shadow = 1.0;
	int samplesDiv2 = int(OffsetTexSize.z);
	vec4 sc = ShadowCoord;

	// Don't test point behind the light source
	if(sc.z >= 0)
	{
		for(int i = 0; i < 4; i++)
		{
			offsetCoord.z = i;
			vec4 offsets = texelFetch(OffsetTex, offsetCoord, 0) * Radius * ShadowCoord.w;

			sc.xy = ShadowCoord.xy + offsets.xy;
			sum += textureProj(ShadowMap, sc);
			sc.xy = ShadowCoord.xy + offsets.zw;
			sum += textureProj(ShadowMap, sc);
		}
		shadow = sum / 8.0;

		if( shadow != 1.0 && shadow != 0.0 ) {
            for( int i = 4; i < samplesDiv2; i++ ) {
                offsetCoord.z = i;
                vec4 offsets = texelFetch(OffsetTex, offsetCoord,0) * Radius * ShadowCoord.w;

                sc.xy = ShadowCoord.xy + offsets.xy;
                sum += textureProj(ShadowMap, sc);
                sc.xy = ShadowCoord.xy + offsets.zw;
                sum += textureProj(ShadowMap, sc);
            }
            shadow = sum / float(samplesDiv2 * 2.0);
        }
	}

	FragColor = vec4(diffuseAndSpecular * shadow + ambient, 1.0);

	// Gamma Correction
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