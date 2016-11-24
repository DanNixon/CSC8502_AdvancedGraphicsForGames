#version 150 core

uniform sampler2D inTex;

uniform int numSamples;

uniform float dispersal;
uniform float haloWidth;
uniform float distortion;

in Vertex
{
  noperspective vec2 texCoord;
} IN;

out vec4 fragColour;

vec4 textureDistorted(sampler2D tex, vec2 coord, vec2 direction, in vec3 distortionVec)
{
	return vec4(
		texture(tex, coord + direction * distortionVec.r).r,
		texture(tex, coord + direction * distortionVec.g).g,
		texture(tex, coord + direction * distortionVec.b).b,
		1.0
	);
}

void main()
{
  // Flip texture coordinates
	vec2 texcoord = -IN.texCoord + vec2(1.0);
	
	vec2 texelSize = 1.0 / vec2(textureSize(inTex, 0));
	
	vec2 ghostVec = (vec2(0.5) - texcoord) * dispersal;
	vec2 haloVec = normalize(ghostVec) * haloWidth;
	
	vec3 distortionVec = vec3(-texelSize.x * distortion, 0.0, texelSize.x * distortion);

	fragColour = vec4(0.0, 0.0, 0.0, 0.0);
	
	for (int i = 0; i < numSamples; ++i) {
		vec2 offset = fract(texcoord + ghostVec * float(i));
		
		float weight = length(vec2(0.5) - offset) / length(vec2(0.5));
		weight = pow(1.0 - weight, 10.0);
	
		fragColour += textureDistorted(inTex, offset, normalize(ghostVec), distortionVec) * weight;
	}

	float weight = length(vec2(0.5) - fract(texcoord + haloVec)) / length(vec2(0.5));
	weight = pow(1.0 - weight, 10.0);
	fragColour += textureDistorted(inTex, fract(texcoord + haloVec), normalize(ghostVec), distortionVec) * weight;
}