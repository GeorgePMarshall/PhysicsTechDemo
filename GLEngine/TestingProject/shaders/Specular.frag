#version 410

struct specMaterial
{
	sampler2D diffuse;
	sampler2D normal;
	sampler2D specular;
	float specPow;
};

struct dirLight
{
	vec3 direction;
	vec3 diffuse;
	vec3 specular;
};

in vec4 vPosition;
in vec4 vNormal;
in vec4 vTangent;
in vec4 vBiTangent;
in vec2 vTexCoords;

out vec4 fragColour;

uniform float time;
uniform vec3 cameraPos;
uniform dirLight light;
uniform specMaterial material;

mat3 TBN;

vec4 CalcDirLight(dirLight light, vec3 normal, vec3 camPos, vec3 vPos)
{	
	TBN = mat3(normalize(vTangent.xyz), normalize(vBiTangent.xyz), normalize(vNormal.xyz));
	vec3 normalMap = texture(material.normal, vTexCoords).xyz * 2 - 1;

	float diffuseTerm = max(0, dot(normalize(TBN * normalMap), normalize(light.direction)));
	vec3 diffuse = vec3(texture(material.diffuse, vTexCoords)) * light.diffuse * diffuseTerm;

	vec3 reflectedVector = reflect(-light.direction, normal.xyz);
	vec3 surfaceToCameraVector = normalize(camPos - vPos);

	float specularTerm = pow(max(0, dot(reflectedVector, surfaceToCameraVector)), 1.0f);
	vec3 specular = light.specular * texture(material.specular, vTexCoords).rgb * specularTerm;

	return vec4(diffuse + specular, 1);

}


void main()
{
	fragColour = CalcDirLight(light, vNormal.xyz, cameraPos, vPosition.xyz);
}

