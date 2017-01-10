#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec4 biTangent;
layout(location = 4) in vec2 texCoords;

out vec4 vPosition;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;
out vec2 vTexCoords;

uniform mat4 projectionView;
uniform mat4 transform;

void main() 
{
	vPosition = transform * position;
	vNormal = normalize((transform * normal).xyz);
	vTangent = normalize((transform * tangent).xyz);
	vBiTangent = normalize((transform * biTangent).xyz);
	vTexCoords = texCoords;

	gl_Position = projectionView * transform * position;
}

