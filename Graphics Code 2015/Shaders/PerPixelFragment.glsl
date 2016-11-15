#version 150 core

uniform sampler2D diffuseTex;

uniform vec3 cameraPos;

uniform int numLights;

uniform vec4 sun_colour;
uniform vec3 sun_position;
uniform float sun_radius;

in Vertex
{
	vec3 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 worldPos;
} IN;

out vec4 fragColor;

void main(void)
{
	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	
	vec3 incident = normalize(sun_position - IN.worldPos);
	float lambert = max(0.0, dot(incident, IN.normal));

	float dist = length(sun_position - IN.worldPos);
	float atten = 1.0 - clamp(dist / sun_radius, 0.0, 1.0);
	
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);

	float rFactor = max(0.0, dot(halfDir, IN.normal));
	float sFactor = pow(rFactor, 50.0);
	
	vec3 diffuseColour = diffuse.rgb * sun_colour.rgb;
	vec3 specColour = (sun_colour.rgb * sFactor) * 0.33;
	
	fragColor = vec4((diffuseColour + specColour) * atten * lambert, diffuse.a);
	//fragColor.rgb += diffuseColour * 0.1;
	
	//fragColor = vec4(0, 0, lambert, 1);
}