#version 150 core

uniform vec4 scale;
uniform vec4 bias;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec4 colour;
in vec2 texCoord;

out Vertex
{
  noperspective vec2 texCoord;
} OUT;

void main()
{
  gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
  OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
}
