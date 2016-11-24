#version 150 core

uniform sampler2D diffuseTex;

uniform float colourTempFactor;

in Vertex
{
  vec4 colour;
  vec2 texCoord;
} IN;

out vec4 fragColour;

void main()
{
  fragColour = texture(diffuseTex, IN.texCoord.xy);

  // Hacky colour temperature adjustment
  if (colourTempFactor > 0.0)
  {
    // Higher, attenuate green and blue channels
    fragColour.g *= (1.0 - colourTempFactor);
    fragColour.b *= (1.0 - colourTempFactor);
  }
  else if (colourTempFactor < 0.0)
  {
    // Lower, attenuate red channel
    fragColour.r *= (1.0 + colourTempFactor);
  }
}
