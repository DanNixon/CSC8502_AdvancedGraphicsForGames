#version 150 core

uniform sampler2D inTex;

uniform vec4 scale;
uniform vec4 bias;

in Vertex
{
  noperspective vec2 texCoord;
} IN;

out vec4 fragColour;

void main()
{
  fragColour = max(vec4(0.0), texture(inTex, IN.texCoord) + bias) * scale;
}