#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform float cameraShakeIntensity;
uniform float cameraShakePhase;

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
  gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);

  OUT.colour = colour;
  OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;

  if (cameraShakeIntensity >= 0.0)
  {
    OUT.texCoord.x += cos(cameraShakePhase * 10) * cameraShakeIntensity;
    OUT.texCoord.y += cos(cameraShakePhase * 15) * cameraShakeIntensity;
  }
}
