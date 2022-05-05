#version 430

const float PI = 3.14159265358979323846;

in vec3 Position;
in vec3 Normal;

uniform struct LightInfo
{
	vec4 Position;
	vec3 L;
} Light[3];

uniform struct MaterialInfo
{
	float Rough;
	bool Metal;
	vec3 Colour;
} Material;

layout(location = 0) out vec4 FragColor;

float ggxDistribution(float nDotH)
{
	float alpha2 = Material.Rough * Material.Rough * Material.Rough  * Material.Rough;
	float d = (nDotH * nDotH) * (alpha2 - 1) + 1;
	return alpha2 / (PI * d * d);
}

float geomSmith(float dotProd)
{
	float k = (Material.Rough + 1.0) * (Material.Rough + 1.0) / 8.0;
	float denom = dotProd * (1 - k) + k;
	return 1.0 / denom;
}

vec3 shlickFresnel(float lDotH)
{
	vec3 f0 = vec3(0.04);
	if(Material.Metal)
	{
		f0 = Material.Colour;
	}
	return f0 + (1 - f0) * pow(1.0 - lDotH, 5);
}

vec3 microfacetModel(int lightIdx, vec3 position, vec3 n)
{
	vec3 diffuseBRDF = vec3(0.0);

	if(!Material.Metal)
	{
		diffuseBRDF = Material.Colour;
	}

	vec3 l = vec3(0.0);
	vec3 lightI = Light[lightIdx].L;

	if(Light[lightIdx].Position.w == 0.0)
	{
		// Directional Light
		l = normalize(Light[lightIdx].Position.xyz);
	}
	else
	{
		l = Light[lightIdx].Position.xyz - position;
		float dist = length(l);
		l = normalize(l);
		lightI /= (dist * dist);
	}

	vec3 v = normalize(-position);
	vec3 h = normalize(v + l);
	float nDotH = dot(n, h);
	float lDotH = dot(l, h);
	float nDotL = max(dot(n, l), 0.0);
	float nDotV = dot(n, v);
	vec3 specBRDF = 0.25 * ggxDistribution(nDotH) * shlickFresnel(lDotH) * geomSmith(nDotL) * geomSmith(nDotV);

	return (diffuseBRDF + PI * specBRDF) * lightI * nDotL;
}

void main()
{
	vec3 sum = vec3(0);
	vec3 n = normalize(Normal);

	for(int i = 0; i < 3; i++)
	{
		sum += microfacetModel(i, Position, n);
	}

	// Gamma
	sum = pow(sum, vec3(1.0 / 2.2));

	FragColor = vec4(sum, 1);
}