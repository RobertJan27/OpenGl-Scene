#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;
in vec4 fPos;

out vec4 fColor;


//lighting
uniform	vec3 lightDir;
uniform vec3 lightDir2;
uniform	vec3 lightColor;
uniform	vec3 lightColor2;
uniform	vec3 lightPos1;
uniform	vec3 lightPos2;
uniform	vec3 lightPos3;
uniform vec3 lightColor3;
uniform int  lighttype1;
uniform int  lighttype2;



//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;
vec3 lightc = vec3 (1.0f);


float constant = 1.0f; 
float linear = 0.0045f; 
float quadratic = 0.0075f;

float computeShadow()
{
vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
normalizedCoords = normalizedCoords * 0.5 + 0.5;
if (normalizedCoords.z > 1.0f)
return 0.0f;
float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
float currentDepth = normalizedCoords.z;
float bias = 0.005f;
float shadow= (currentDepth-bias)> closestDepth ? 1.0f : 0.0f;


return shadow;
}



float computeFog()
{
 float fogDensity = 0.015f;
 float fragmentDistance = length(fPosEye);
 float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));

 return clamp(fogFactor, 0.0f, 1.0f);
}


void computeLightComponents1()
{		
	
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
}

void computeLightPunctiform(vec3 lightPosEye)
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightPosEye-fPos.xyz);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPos.xyz);
		
	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);

	
	//compute distance to light
	float dist = length(lightPosEye - fPos.xyz);
	//compute attenuation
	float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist));
	
	//compute ambient light
	ambient +=att* ambientStrength * lightColor3;
	
	//compute diffuse light
	diffuse +=att* max(dot(normalEye, lightDirN), 0.0f) * lightColor3;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(normalEye, halfVector), 0.0f), shininess);
	specular +=att* specularStrength * specCoeff * lightColor3;

}


void computeLightComponents2()
{		
	
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir2);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	//compute ambient light
	ambient += ambientStrength * lightColor2;
	
	//compute diffuse light
	diffuse += max(dot(normalEye, lightDirN), 0.0f) * lightColor2;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular += specularStrength * specCoeff * lightColor2;
}

void main() 
{	

	float shadow=0.0f;

	if(lighttype1 ==1)
	computeLightComponents1();
	if(lighttype2 ==1)
	computeLightComponents2();	
	if(lighttype1 ==1 || lighttype2 ==1)
	shadow=computeShadow();
	computeLightPunctiform(lightPos1);
	//computeLightPunctiform(lightPos2);
	//computeLightPunctiform(lightPos3);
	
	
	
	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f);//orange
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;
	
	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
    	
	//fog
	vec4 color2=vec4(color, 1.0f);
	float fogFactor = computeFog();
	vec4 fogColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	fColor = fogColor*(1-fogFactor)+color2*fogFactor;
	
	//fragmentare
	/*vec4 colorFromTexture = texture(diffuseTexture, fTexCoords);
	if(colorFromTexture.a < 0.1)
	discard;
	fColor = colorFromTexture;
    	*/
	

//fColor = vec4(color, 1.0f);
}
