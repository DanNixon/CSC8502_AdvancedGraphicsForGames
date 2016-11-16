#version 150 core

uniform sampler2D diffuseTex;

uniform vec3 cameraPos;

uniform float specularPower;
uniform float specularIntensity;

uniform int numLights;

uniform vec4 sun_colour;
uniform vec3 sun_position;
uniform float sun_radius;
uniform float sun_ambientIntensity;

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
	float sFactor = pow(rFactor, specularPower);
	
	vec3 diffuseColour = diffuse.rgb * sun_colour.rgb;
	vec3 specColour = (sun_colour.rgb * sFactor) * specularIntensity;
	
	fragColor = vec4((diffuseColour + specColour) * atten * lambert, diffuse.a);
	fragColor.rgb += diffuseColour * sun_ambientIntensity;
	
	//fragColor = vec4(0, 0, lambert, 1);
}