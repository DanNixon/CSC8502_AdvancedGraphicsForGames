#version 150 core

uniform sampler2D effectTex;
uniform sampler2D lensFlareTex;

in Vertex
{
  vec2 texCoord;
} IN;

out vec4 fragColour;

void main()
{
  vec4 effectColour = texture(effectTex, IN.texCoord);
	vec4 lensFlareColour = texture(lensFlareTex, IN.texCoord);
  fragColour = effectColour + lensFlareColour;
}
