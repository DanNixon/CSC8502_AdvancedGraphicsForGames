#version 410 core

layout (quads, equal_spacing, ccw) in;

uniform sampler2D heightmapTex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

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
  vec3 normal;
	vec3 worldPos;
} OUT;

vec2 interpolate_v2(in vec2 v0, in vec2 v1, in vec2 v2, in vec2 v3)
{
  vec2 a = mix(v0, v1, gl_TessCoord.x);
  vec2 b = mix(v3, v2, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);
}

void main()
{
	vec4 pos_a = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 pos_b = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	vec4 pos = mix(pos_a, pos_b, gl_TessCoord.y);

	vec2 tc_a = mix(IN[0].texCoord, IN[1].texCoord, gl_TessCoord.x);
  vec2 tc_b = mix(IN[2].texCoord, IN[3].texCoord, gl_TessCoord.x);
  OUT.texCoord = mix(tc_a, tc_b, gl_TessCoord.y);

  vec3 p0 = gl_in[0].gl_Position.xyz;
  vec3 p1 = gl_in[1].gl_Position.xyz;
  vec3 p2 = gl_in[2].gl_Position.xyz;

  vec3 v0 = p0 - p1;
  vec3 v1 = p2 - p1;

  vec3 norm = cross(v1, v0);
  norm = normalize(norm);
	OUT.normal = norm;

  vec4 texCol = texture(heightmapTex, OUT.texCoord);

  vec4 worldPos = modelMatrix * vec4(pos.xyz, 1.0);
	worldPos.y += (texCol.r * 50.0) - 30.0;
	
  OUT.worldPos = worldPos.xyz;

  gl_Position = projMatrix * viewMatrix * worldPos;
}
