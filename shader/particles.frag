#version 430

in vec3 Position;
uniform vec4 Color;

layout( location = 0 ) out vec4 FragColor;

void main() {
	vec4 colour = vec4(clamp(Position + 0.1, 0.4, 0.75), 1.0);
	FragColor = colour;
}