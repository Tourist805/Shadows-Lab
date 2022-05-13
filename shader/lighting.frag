#version 430

out vec4 FragColor;

in Vertex_Output
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
} fragment_input;

uniform struct Light
{
	vec3 Position;
	vec3 Colour;
} lights[16];

uniform sampler2D diffuseTexture;
uniform vec3 viewPosition;

void main()
{
	vec3 colour = texture(diffuseTexture, fragment_input.TexCoords).rgb;
	vec3 normal = normalize(fragment_input.Normal);

	vec3 ambient = 0.0 * colour;

	vec3 lighting = vec3(0.0);
	for(int i = 0; i < 16; i++)
	{
		// calculate the diffuse
		vec3 lightDirection = normalize(lights[i].Position - fragment_input.Position);
		float diff = max(dot(lightDirection, normal), 0.0);
		vec3 diffuse = lights[i].Colour * diff * colour;
		vec3 result = diffuse;

		float distanceLen = length(fragment_input.Position - lights[i].Position);
		result *= 1.0 / (distanceLen * distanceLen);
		lighting += result;
	}

	FragColor = vec4(ambient + lighting, 1.0);
}