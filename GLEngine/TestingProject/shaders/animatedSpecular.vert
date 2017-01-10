#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec4 biTangent;
layout(location = 4) in vec2 texCoords;
layout(location = 5) in vec4 weights;
layout(location = 6) in vec4 indices;

out vec4 vPosition;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;
out vec2 vTexCoords;

uniform mat4 projectionView;
uniform mat4 transform;

uniform mat4 bones[128];

void main() 
{
	vPosition = transform * position;
	vNormal = normalize((transform * normal).xyz);
	vTangent = normalize((transform * tangent).xyz);
	vBiTangent = normalize((transform * biTangent).xyz);
	vTexCoords = texCoords;

	ivec4 index = ivec4(indices);

	vec4 animOffset = bones[index.x] * position * weights.x;
	animOffset = bones[index.y] * position * weights.y;
	animOffset = bones[index.z] * position * weights.z;
	animOffset = bones[index.w] * position * weights.w;

	gl_Position = projectionView * transform * animOffset;
}
