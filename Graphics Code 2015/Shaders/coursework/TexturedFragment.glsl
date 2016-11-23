#version 150 core

uniform sampler2D diffuseTex;

in Vertex
{
	vec4 colour;
  vec2 texCoord;
} IN;

out vec4 fragColour;

void main()
{
	fragColour = texture(diffuseTex, IN.texCoord.xy);
}