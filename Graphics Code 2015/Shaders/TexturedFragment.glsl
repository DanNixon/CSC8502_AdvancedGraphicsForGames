#version 150 core

uniform sampler2D diffuseTex;
uniform float blendFactor;

in Vertex
{
  vec4 colour;
  vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void)
{
  vec4 texCol = texture(diffuseTex, IN.texCoord) * blendFactor;
  vec4 staticCol = IN.colour * (1.0 - blendFactor);
  vec4 col = texCol + staticCol;

  if (col.a < 0.05f)
    discard;

  gl_FragColor = col;
}
