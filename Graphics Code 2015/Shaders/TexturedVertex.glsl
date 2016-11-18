#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec4 colour;
in vec2 texCoord;

out Vertex
{
  vec4 colour;
  vec2 texCoord;
} OUT;

void main()
{
  mat4 mvp = projMatrix * viewMatrix * modelMatrix;
  gl_Position = mvp * vec4(position, 1.0);
  OUT.colour = colour;
  OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
}
