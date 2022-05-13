#version 430

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoords;

out Vertex_Output
{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
} vertex_output;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

uniform bool inverse_normals;

void main()
{
	vertex_output.Position = vec3(Model * vec4(VertexPosition, 1.0));
	vertex_output.TexCoords = VertexTexCoords;

	vec3 normal = inverse_normals ? -VertexNormal : VertexNormal;

	mat3 normalMatrix = transpose(inverse(mat3(Model)));
	vertex_output.Normal = normalize(normalMatrix * normal);

	gl_Position = Projection * View * Model * vec4(VertexPosition, 1.0);
}