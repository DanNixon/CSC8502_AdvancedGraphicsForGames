#version 150 core

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 2;

const int NUM_TEX_LEVELS = 3;

uniform sampler2D levelTex[NUM_TEX_LEVELS];

uniform vec3 cameraPos;

uniform float specularPower;
uniform float specularIntensity;

uniform int numPointLights;
uniform int numSpotLights;

struct Light                                                                    
{ 
	vec3 position;                                                                                                                                          
  vec4 colour;                                                                     
  float ambientIntensity;                                                         
	float reach;
};                                                                                  

struct PointLight                                                                           
{                                                                                           
  Light light;                                                        
};                                                                                          

struct SpotLight                                                                            
{                                                                                           
  Light light;                                                                        
  vec3 direction;                                                                         
  float cutoff;                                                                           
};

uniform PointLight pointLights[MAX_POINT_LIGHTS];                                          
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];                                             

uniform vec3 fogBaseColour;
uniform float fogExp;
uniform float fogAtten;

in Vertex
{
  vec3 colour;
  vec2 texCoord;
  vec4 bump;
  vec3 worldPos;
} IN;

out vec4 fragColour;

vec3 applyFog(vec3 rgb, vec3 rayDir, Light light, float f)
{
  float fogAmount = 1.0 - exp( -length(rayDir) * fogExp );
  float sunAmount = max(dot( rayDir, light.position - IN.worldPos ), 0.0);
  vec3 fogColour = mix(fogBaseColour, light.colour.rgb * light.colour.a, f);
	return mix(rgb, fogColour, fogAmount * fogAtten);
}

vec3 processPointLight(vec4 diffuse, Light light)
{
	vec3 incident = normalize(light.position - IN.worldPos);
  float lambert = max(0.0, dot(incident, IN.bump.xyz));

  float dist = length(light.position - IN.worldPos);
  float atten = 1.0 - clamp(dist / light.reach, 0.0, 1.0);

  vec3 viewDir = normalize(cameraPos - IN.worldPos);
  vec3 halfDir = normalize(incident + viewDir);

  float rFactor = max(0.0, dot(halfDir, IN.bump.xyz));
  float sFactor = pow(rFactor, specularPower);

	vec3 lColour = light.colour.rgb * light.colour.a;
  vec3 diffuseColour = diffuse.rgb * lColour;
  vec3 specColour = (lColour * sFactor) * specularIntensity;

  vec3 colour = vec3((diffuseColour + specColour) * atten * lambert);
  colour += diffuseColour * light.ambientIntensity;
	colour = applyFog(colour, IN.worldPos - cameraPos, light, atten);

	return colour;
}

vec3 processSpotLight(vec4 diffuse, SpotLight light)
{
  vec3 lightToFragment = normalize(IN.worldPos - light.light.position);
  float factor = dot(lightToFragment, normalize(light.direction));
	vec3 colour = vec3(0.0, 0.0, 0.0);

  if (factor > light.cutoff)
	{
    colour = processPointLight(diffuse, light.light);
    colour *= (1.0 - (1.0 - factor) * 1.0 / (1.0 - light.cutoff));
  }
	
	return colour;
}

const float levelTH[] = float[](0.0, 0.5, 0.55, 0.6, 0.66, 1.0);

void main(void)
{
	float height = IN.bump.w;
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	
	if (height >= levelTH[0] && height <= levelTH[1])
	{
		diffuse = texture(levelTex[0], IN.texCoord); 
	}
  else if (height <= levelTH[2]) 
  { 
    height -= levelTH[1]; 
    height /= levelTH[2] - levelTH[1];  
       
    diffuse += texture(levelTex[0], IN.texCoord) * (1.0 - height); 
    diffuse += texture(levelTex[1], IN.texCoord) * height; 
  } 
  else if (height <= levelTH[3])
	{
		diffuse = texture(levelTex[1], IN.texCoord); 
	}
  else if (height <= levelTH[4]) 
  { 
    height -= levelTH[3]; 
    height /= levelTH[4] - levelTH[3];
       
    diffuse += texture(levelTex[1], IN.texCoord) * (1.0 - height); 
    diffuse += texture(levelTex[2], IN.texCoord) * height;       
  } 
  else
	{
		diffuse = texture(levelTex[2], IN.texCoord);
	}

  fragColour = vec4(0.0, 0.0, 0.0, 1.0);
	
	for (int i = 0; i < numPointLights; i++)
		fragColour.rgb += processPointLight(diffuse, pointLights[i].light);
	
  for (int i = 0; i < numSpotLights; i++)
		fragColour.rgb += processSpotLight(diffuse, spotLights[i]);
}
