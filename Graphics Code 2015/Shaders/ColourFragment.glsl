#version 150 core

in Vertex
{
  vec4 colour;
} IN;

out vec4 fragCol;

void main()
{
  fragCol = IN.colour;
}
