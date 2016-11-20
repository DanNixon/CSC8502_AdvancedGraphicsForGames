#version 410 core

layout (quads, ccw) in;

uniform sampler2D heightmapTex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in Vertex
{
	vec4 colour;
  vec2 texCoord;
	vec3 normal;
} IN[];

out Vertex
{
  vec4 colour;
	vec2 texCoord;
  vec4 bump;
	vec3 worldPos;
} OUT;

const ivec3 off = ivec3(-1, 0, 1);

float n(float low, float high, float v)
{
	return clamp(clamp(v - low, 0.0, 1.0) / (high - low), 0.0, 1.0);
}

void main()
{
	// Texture coordinate
	vec2 tc_a = mix(IN[0].texCoord, IN[1].texCoord, gl_TessCoord.x);
  vec2 tc_b = mix(IN[2].texCoord, IN[3].texCoord, gl_TessCoord.x);
  vec2 tempTexCoord = mix(tc_a, tc_b, gl_TessCoord.y);

	// Normal
	float height = texture(heightmapTex, tempTexCoord).r;
  float hL = textureOffset(heightmapTex, tempTexCoord, off.xy).r;
  float hR = textureOffset(heightmapTex, tempTexCoord, off.zy).r;
  float hD = textureOffset(heightmapTex, tempTexCoord, off.yx).r;
  float hU = textureOffset(heightmapTex, tempTexCoord, off.yz).r;
	
	vec3 normal = vec3(hL - hR, 0.0, hD - hU) * 50.0; 
	normal.y = 2.0;
	
	OUT.bump = vec4(normalize(normal), height);

	// Position
	vec3 pos_a = mix(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_TessCoord.x);
	vec3 pos_b = mix(gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz, gl_TessCoord.x);
	vec3 pos = mix(pos_a, pos_b, gl_TessCoord.y);
	
	vec4 worldPos = modelMatrix * vec4(pos, 1.0);
	worldPos.y += mix(0.0, 50.0, n(0.4, 0.7, height));
	
  OUT.worldPos = worldPos.xyz;
	OUT.texCoord = (textureMatrix * vec4(tempTexCoord, 0.0, 1.0)).xy;

  gl_Position = projMatrix * viewMatrix * worldPos;
}
