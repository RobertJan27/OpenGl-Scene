#version 410 core

uniform vec3 baseColor;

layout(location=0) in vec3 normalEye;
layout(location=1) in vec4 vertPosEye

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

//lighting
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 baseColor;


vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;

out vec4 fcolor;


float constant = 1.0f;
float linear = 0.0045f;
float quadratic = 0.0075f;
void computeLight()
{
	//compute eye space coordinates
	vertPosEye = view * model * vec4(vPosition, 1.0f);
	normalEye = normalize(normalMatrix * vNormal);
	
	//normalize light direction
	vec3 lightDirN = normalize(lightDir - fragPosEye.xyz);
	
	//compute view direction (in eye coordinates, the viewer is situated at the origin
	vec3 viewDir = normalize(viewPosEye - fragPosEye.xyz);
	vec3 halfVector = normalize(lightDirN + viewDirN);

	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflectDir = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(normalEye, halfVector), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
}

void main()
{	


computelight();
color=min((ambient + diffuse) * baseColor + specular, 1.0f);
fColor = vec4(color, 1.0f);
}
