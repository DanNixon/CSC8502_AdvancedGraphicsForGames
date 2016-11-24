#version 150 core

uniform sampler2D diffuseTex;

uniform vec3 cameraPos;

uniform float specularPower;
uniform float specularIntensity;

uniform int numLights;

uniform vec4 sun_colour;
uniform vec3 sun_position;
uniform float sun_radius;
uniform float sun_ambientIntensity;

uniform vec4 moon_colour;
uniform vec3 moon_position;
uniform float moon_radius;
uniform float moon_ambientIntensity;

in Vertex
{
  vec3 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 worldPos;
} IN;

out vec4 fragColour;

vec3 processLight(vec4 diffuse, vec4 lightCol, vec3 lightPos, float lightRadius, float lightAmbInt)
{
  vec3 incident = normalize(lightPos - IN.worldPos);
  float lambert = max(0.0, dot(incident, IN.normal));

  float dist = length(lightPos - IN.worldPos);
  float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

  vec3 viewDir = normalize(cameraPos - IN.worldPos);
  vec3 halfDir = normalize(incident + viewDir);

  float rFactor = max(0.0, dot(halfDir, IN.normal));
  float sFactor = pow(rFactor, specularPower);

  vec3 lColour = lightCol.rgb * lightCol.a;
  vec3 diffuseColour = diffuse.rgb * lColour;
  vec3 specColour = (lColour * sFactor) * specularIntensity;

  vec3 finalColour = vec3((diffuseColour + specColour) * atten * lambert);
  finalColour += diffuseColour * lightAmbInt;

  return finalColour;
}

void main(void)
{
  vec4 diffuse = texture(diffuseTex, IN.texCoord);

  fragColour = vec4(0.0, 0.0, 0.0, diffuse.a);
  fragColour.rgb += processLight(diffuse, sun_colour, sun_position, sun_radius, sun_ambientIntensity);
  fragColour.rgb += processLight(diffuse, moon_colour, moon_position, moon_radius, moon_ambientIntensity);
}
