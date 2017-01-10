#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

uniform mat4 projectionView;
uniform sampler2D heightMap;

void main() 
{
	vPosition = position;
	vNormal = normal;
	vTexCoord = texCoord;
	vec3 pos = position;
	//pos.y += texture(heightMap, texCoord).r;

	gl_Position = projectionView * vec4(pos, 1);
}

