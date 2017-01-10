#version 410

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 vPosition[];
in float vLifetime[];
in float vLifespan[];

out vec4 gColour;

uniform vec3 cameraPos;
uniform mat4 projectionView;

uniform float startSize;
uniform float endSize;

uniform vec4 startColour;
uniform vec4 endColour;

void main()
{
	gColour = mix(startColour, endColour, vLifetime[0] / vLifespan[0]);

	float halfsize = mix(startSize, endSize, vLifetime[0] / vLifespan[0]);

	vec3 verticies[4];

	verticies[0] = vec3(-halfsize, halfsize, 0);
	verticies[1] = vec3(halfsize, halfsize, 0);
	verticies[2] = vec3(-halfsize, -halfsize, 0);
	verticies[3] = vec3(halfsize, -halfsize, 0);

	mat3 billboard;
	billboard[2] = normalize(cameraPos - vPosition[0]);
	billboard[0] = cross(billboard[2], vec3(0, 1, 0));
	billboard[1] = cross(billboard[2], billboard[0]);

	gl_Position = projectionView * vec4(billboard * verticies[0] + vPosition[0] , 1);
	EmitVertex();
	
	gl_Position = projectionView * vec4(billboard * verticies[1] + vPosition[0] , 1);
	EmitVertex();
	
	gl_Position = projectionView * vec4(billboard * verticies[2] + vPosition[0] , 1);
	EmitVertex();
	
	gl_Position = projectionView * vec4(billboard * verticies[3] + vPosition[0] , 1);
	EmitVertex();
}