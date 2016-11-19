#version 410 core

layout (quads, ccw) in;

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

void main()
{
	// Texture coordinate
	vec2 tc_a = mix(IN[0].texCoord, IN[1].texCoord, gl_TessCoord.x);
  vec2 tc_b = mix(IN[2].texCoord, IN[3].texCoord, gl_TessCoord.x);
  OUT.texCoord = mix(tc_a, tc_b, gl_TessCoord.y);

	// Normal
  vec3 p0 = gl_in[0].gl_Position.xyz;
  vec3 p1 = gl_in[1].gl_Position.xyz;
  vec3 p2 = gl_in[2].gl_Position.xyz;

  vec3 v0 = p0 - p1;
  vec3 v1 = p2 - p1;

	OUT.normal = normalize(cross(v1, v0));

	// Position
	vec3 pos_a = mix(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_TessCoord.x);
	vec3 pos_b = mix(gl_in[2].gl_Position.xyz, gl_in[3].gl_Position.xyz, gl_TessCoord.x);
	vec3 pos = mix(pos_a, pos_b, gl_TessCoord.y);
	
	vec4 worldPos = modelMatrix * vec4(pos, 1.0);
	float height = texture(heightmapTex, OUT.texCoord).r;
	worldPos.y += (0.5 - (height - 0.5)) * 50.0;
	
  OUT.worldPos = worldPos.xyz;

  gl_Position = projMatrix * viewMatrix * worldPos;
}
