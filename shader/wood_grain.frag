#version 460

layout(binding = 0) uniform sampler2D NoiseTex;

uniform vec4 DarkWoodColour = vec4(0.8, 0.5, 0.1, 1.0);
uniform vec4 LightWoodColour = vec4(1.0, 0.75, 0.25, 1.0);
uniform mat4 Slice;

in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

void main()
{
	// Transform the texture coordinates to define th "slice" of the log
	vec4 cyl = Slice * vec4(TexCoord, 0.0, 1.0);

	// The distance from the log's y axis (vector's length from 0, 0
	float dist = length(cyl.xz);

	// Perturb the distance using the noise texture
	vec4 noise = texture(NoiseTex, TexCoord);
	dist += noise.b;

	// Determine the colour as a mixture of light and dark wood colours
	float t = 1.0 - abs(fract(dist) * 10.0 - 1.0);
	t = smoothstep(0.2, 0.5, t);
	vec4 colour = mix(DarkWoodColour, LightWoodColour, t);

	FragColor = vec4(colour.rgb, 1.0);
}