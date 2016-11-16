#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec2 texCoord;

out Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} OUT;

void main()
{
	OUT.colour = colour ;
	OUT.texCoord = texCoord;
	
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix * normalize(normal));
	
	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz ;
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
}