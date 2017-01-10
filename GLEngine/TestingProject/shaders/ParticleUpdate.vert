#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 velocity;
layout(location = 2) in float lifetime;
layout(location = 3) in float lifespan;

out vec3 vPosition;
out vec3 vVelocity;
out float vLifetime;
out float vLifespan;

uniform float time;
uniform float deltatime;

uniform float minLife;
uniform float maxLife;

uniform float minVelocity;
uniform float maxVelocity;

uniform vec3 emitterPosition;

const float INVERSE_MAX_UINT = 1.0f / 4294967295.0f;

float rand(uint seed, float range) {
	uint i = (seed ^ 12345391u) * 2654435769u;
	i ^= (i << 6u) ^ (i >> 26u);
	i *= 2654435769u;
	i += (i << 5u) ^ (i >> 12u);
	return float(range * i) * INVERSE_MAX_UINT;
} 

void main()
{
	uint seed = uint(time * 1000.0) + uint(gl_VertexID);

	vPosition = position + velocity * deltatime;
	vVelocity = velocity;
	vLifetime = lifetime + deltatime;
	vLifespan = lifespan;

	if(vLifetime > vLifespan)
	{
		vVelocity = vec3(rand(seed++, 2) - 1, rand(seed++, 2) - 1, rand(seed++, 2) - 1);
		vVelocity = normalize(vVelocity) * 2;// * rand(seed++, maxVelocity - minVelocity) + minVelocity);
		vPosition = emitterPosition;
		vLifetime = 0;
		vLifespan = rand(seed, maxLife - minLife) + minLife;
	}

}