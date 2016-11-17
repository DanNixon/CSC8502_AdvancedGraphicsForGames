#version 150 core

uniform sampler2D waterTexture;
uniform samplerCube cubeTex;

uniform vec3 cameraPos;

uniform int numLights;

uniform vec4 sun_colour;
uniform vec3 sun_position;
uniform float sun_radius;

uniform vec4 moon_colour;
uniform vec3 moon_position;
uniform float moon_radius;

in Vertex
{
  vec4 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 worldPos;
} IN;

out vec4 fragColour;

vec3 processLight(vec4 diffuse, vec4 lightColour, vec3 lightPos, float lightRadius)
{
  float dist = length(lightPos - IN.worldPos);
  float atten = 1.0 - clamp(dist / lightRadius, 0.2, 1.0);
  return (lightColour * diffuse * atten).rgb;
}

void main()
{
  vec4 diffuse = texture(waterTexture, IN.texCoord) * IN.colour;
  vec3 incident = normalize(IN.worldPos - cameraPos);

  vec3 lighting = vec3(0, 0, 0);
  lighting += processLight(diffuse, sun_colour, sun_position, sun_radius);
  lighting += processLight(diffuse, moon_colour, moon_position, moon_radius);

  vec4 reflection = texture(cubeTex, reflect(incident, normalize(IN.normal)));

  fragColour = vec4(lighting, 1.0) * (diffuse * reflection);
}
