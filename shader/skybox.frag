#version 430

uniform samplerCube SkyboxTex;

in vec3 Position;

layout(location = 0) out vec4 FragColor;

void main()
{
	vec3 texColour = texture(SkyboxTex, normalize(Position)).rgb;

	// Apply gamma correction
	texColour = pow(texColour, vec3(1.0 / 2.2));
	FragColor = vec4(texColour, 1);
}