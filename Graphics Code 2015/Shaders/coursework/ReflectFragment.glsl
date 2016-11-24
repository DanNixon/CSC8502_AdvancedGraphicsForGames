#version 150 core

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 2;

uniform sampler2D waterTexture;
uniform samplerCube cubeTex;

uniform vec3 cameraPos;

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

uniform vec3 fogBaseColour;
uniform float fogExp;
uniform float fogAtten;

in Vertex
{
  vec4 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 worldPos;
} IN;

out vec4 fragColour;

vec3 applyFog(vec3 rgb, vec3 rayDir, Light light, float f)
{
  float fogAmount = 1.0 - exp( -length(rayDir) * fogExp );
  float sunAmount = max(dot( rayDir, light.position - IN.worldPos ), 0.0);
  vec3 fogColour = mix(fogBaseColour, light.colour.rgb * light.colour.a, f);
  return mix(rgb, fogColour, fogAmount * fogAtten);
}

vec3 processPointLight(vec4 diffuse, Light light)
{
  float dist = length(light.position - IN.worldPos);
  float atten = 1.0 - clamp(dist / light.reach, 0.2, 1.0);
  vec3 colour = (light.colour * diffuse * atten).rgb;
  return applyFog(colour, IN.worldPos - cameraPos, light, atten);
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
  vec4 diffuse = texture(waterTexture, IN.texCoord) * IN.colour;
  vec3 incident = normalize(IN.worldPos - cameraPos);

  vec3 lighting = vec3(0, 0, 0);

  for (int i = 0; i < numPointLights; i++)
    lighting += processPointLight(diffuse, pointLights[i].light);

  for (int i = 0; i < numSpotLights; i++)
    lighting += processSpotLight(diffuse, spotLights[i]);

  vec4 reflection = texture(cubeTex, reflect(incident, normalize(IN.normal)));

  fragColour = vec4(lighting, 1.0) * (diffuse + reflection);
}
