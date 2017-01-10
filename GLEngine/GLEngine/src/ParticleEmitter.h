#pragma once
#include "ShaderProgram.h"
#include "Camera.h"
#include "gTime.h"

struct Particle 
{
	Particle() : lifeTime(1), lifeSpan(0) {}

	glm::vec3 position;
	glm::vec3 velocity;
	float lifeSpan;
	float lifeTime;
};



class ParticleEmitter
{
	ShaderProgram drawShader;
	ShaderProgram updateShader;
	
	void CreateBuffers();



protected:

	GLuint activeBuffer, vao[2], vbo[2];

	Particle* particles;	
	GLuint maxParticles;
	

	float minLifeSpan, maxLifeSpan;
	float startSize, endSize;
	float minVelocity, maxVelocity;

	glm::vec4 startColour, endColour;

	float lastDrawTime;


public:

	glm::vec3 position;
	~ParticleEmitter();
	void Initialize();
	void Draw(Camera& camera);







};