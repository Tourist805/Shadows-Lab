#version 430

in vec2 TexCoord;
in vec3 ReflectDir;

uniform samplerCube CubeMapTex;
uniform sampler2D NoiseTex;

uniform float ReflectFactor;
uniform vec4 MaterialColour;

layout(location = 0) out vec4 FragColor;

uniform float Threshold = 0.58;

void main()
{
	// Retrieve values from the CPU calculated noise texture
	float noise = texture(NoiseTex, TexCoord).a;
	float scale = floor(noise + (1 - Threshold));

	// Retrieve the cube map texture
	vec3 cubeMapColour = texture(CubeMapTex, ReflectDir).rgb;

	// Apply gamma correction
	cubeMapColour = pow(cubeMapColour, vec3(1.0 / 2.2));

	vec3 rustColour = mix(MaterialColour.rgb, vec3(0.01), noise);

	FragColor = vec4(mix(cubeMapColour, rustColour, scale), 1);
}