#version 150 core

uniform samplerCube skyboxTexture;
uniform vec3 cameraPos;

in Vertex
{
  vec3 normal;
} IN;

out vec4 fragColor;

void main()
{
  fragColor = texture(skyboxTexture, normalize(IN.normal));
}
