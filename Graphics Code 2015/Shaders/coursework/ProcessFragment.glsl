#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D filterTex;

uniform float colourTemp;
uniform float filter;
uniform vec4 colourOverride;

in Vertex
{
  vec4 colour;
  vec2 texCoord;
} IN;

out vec4 fragColour;

// Colour temperature function adapted from: https://www.shadertoy.com/view/lsSXW1
vec3 colourTemperatureToRGB(float temperatureInKelvins)
{
  vec3 retColour;

  temperatureInKelvins = clamp(temperatureInKelvins, 1000.0, 40000.0) / 100.0;

  if (temperatureInKelvins <= 66.0)
  {
    retColour.r = 1.0;
    retColour.g = clamp(0.39008157876901960784 * log(temperatureInKelvins) - 0.63184144378862745098, 0.0, 1.0);
  }
  else
  {
    float t = temperatureInKelvins - 60.0;
    retColour.r = clamp(1.29293618606274509804 * pow(t, -0.1332047592), 0.0, 1.0);
    retColour.g = clamp(1.12989086089529411765 * pow(t, -0.0755148492), 0.0, 1.0);
  }

  if (temperatureInKelvins >= 66.0)
  {
    retColour.b = 1.0;
  }
  else if(temperatureInKelvins <= 19.0)
  {
    retColour.b = 0.0;
  }
  else
  {
    retColour.b = clamp(0.54320678911019607843 * log(temperatureInKelvins - 10.0) - 1.19625408914, 0.0, 1.0);
  }

  return retColour;
}

void main()
{
  fragColour = texture(diffuseTex, IN.texCoord);
  fragColour.rgb *= colourTemperatureToRGB(colourTemp);
  fragColour += texture(filterTex, IN.texCoord) * filter;
  if (colourOverride.a > 0.1)
    fragColour.rgb = colourOverride.rgb;
}
