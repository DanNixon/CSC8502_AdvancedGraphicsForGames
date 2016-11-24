#version 150 core

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 2;

uniform sampler2D diffuseTex;

uniform vec3 cameraPos;

uniform float specularPower;
uniform float specularIntensity;

uniform int numPointLights;
uniform int numSpotLights;

struct Light
{
  vec3 position;
  vec4 colour;
  float ambientIntensity;
  float reach;
};

struct PointLight
{
  Light light;
};

struct SpotLight
{
  Light light;
  vec3 direction;
  float cutoff;
};

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

in Vertex
{
  vec3 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 worldPos;
} IN;

out vec4 fragColour;

vec3 processPointLight(vec4 diffuse, Light light)
{
  vec3 incident = normalize(light.position - IN.worldPos);
  float lambert = max(0.0, dot(incident, IN.normal));

  float dist = length(light.position - IN.worldPos);
  float atten = 1.0 - clamp(dist / light.reach, 0.0, 1.0);

  vec3 viewDir = normalize(cameraPos - IN.worldPos);
  vec3 halfDir = normalize(incident + viewDir);

  float rFactor = max(0.0, dot(halfDir, IN.normal));
  float sFactor = pow(rFactor, specularPower);

  vec3 lColour = light.colour.rgb * light.colour.a;
  vec3 diffuseColour = diffuse.rgb * lColour;
  vec3 specColour = (lColour * sFactor) * specularIntensity;

  vec3 colour = vec3((diffuseColour + specColour) * atten * lambert);
  colour += diffuseColour * light.ambientIntensity;

  return colour;
}

vec3 processSpotLight(vec4 diffuse, SpotLight light)
{
  vec3 lightToFragment = normalize(IN.worldPos - light.light.position);
  float factor = dot(lightToFragment, normalize(light.direction));
  vec3 colour = vec3(0.0, 0.0, 0.0);

  if (factor > light.cutoff)
  {
    colour = processPointLight(diffuse, light.light);
    colour *= (1.0 - (1.0 - factor) * 1.0 / (1.0 - light.cutoff));
  }

  return colour;
}

void main()
{
  vec4 diffuse = texture(diffuseTex, IN.texCoord);

  fragColour = vec4(0.0, 0.0, 0.0, diffuse.a);

  for (int i = 0; i < numPointLights; i++)
    fragColour.rgb += processPointLight(diffuse, pointLights[i].light);

  for (int i = 0; i < numSpotLights; i++)
    fragColour.rgb += processSpotLight(diffuse, spotLights[i]);
}
