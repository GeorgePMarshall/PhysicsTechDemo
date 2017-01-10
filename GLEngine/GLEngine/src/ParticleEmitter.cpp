#include "ParticleEmitter.h"


ParticleEmitter::~ParticleEmitter()
{
	delete[] particles;
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);
}

void ParticleEmitter::Initialize()
{
	activeBuffer = 0;

	startSize = 0.01f;
	endSize = 0.1f;
	startColour = vec4(1.f, 0.3f, 0.0f, 1.0f);
	endColour = vec4(0.5, 0.1f, 0.0f, 0.9f);
	minVelocity = 0;
	maxVelocity = 100;
	minLifeSpan = 1;
	maxLifeSpan = 3;

	maxParticles = 100000;

	particles = new Particle[maxParticles];

	CreateBuffers();

	drawShader.CreateShaderProgram("ParticleDraw.vert", "ParticleDraw.geom", "ParticleDraw.frag");
	drawShader.setFloat("startSize", startSize);
	drawShader.setFloat("endSize", endSize);
	drawShader.setVec4("startColour", startColour);
	drawShader.setVec4("endColour", endColour);
	
	const char* varyings[] = { "vPosition", "vVelocity", "vLifetime", "vLifespan" };
	updateShader.CreateShaderProgram("ParticleUpdate.vert", varyings, 4);
	updateShader.setFloat("minLife", minLifeSpan);
	updateShader.setFloat("maxLife", maxLifeSpan);
	updateShader.setFloat("minVelocity", minVelocity);
	updateShader.setFloat("maxVelocity", maxVelocity);
}

void ParticleEmitter::CreateBuffers()
{

	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	//buffer 1
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Particle), particles, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);

	glEnableVertexAttribArray(1); //velocity
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 12);

	glEnableVertexAttribArray(2); //lifetime
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 24);

	glEnableVertexAttribArray(3); //lifespan
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 28);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//buffer 2
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Particle), 0, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);

	glEnableVertexAttribArray(1); //velocity
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 12);

	glEnableVertexAttribArray(2); //lifetime
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 24);

	glEnableVertexAttribArray(3); //lifespan
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 28);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void ParticleEmitter::Draw(Camera& camera)
{
	GLuint otherBuffer = (activeBuffer + 1) % 2;

	updateShader.setFloat("time", (GLfloat)glfwGetTime());
	updateShader.setFloat("deltatime", gTime::deltaTime());
	//updateShader.setVec3("emitterPosition", vec3(5 * sin(glfwGetTime()), 3, 5 * cos(glfwGetTime())));
	updateShader.setVec3("emitterPosition", position);
	//updateShader.setVec3("emitterPosition", camera->getPosition() + vec3(glm::normalize(-camera->getWorldTransform()[2]) * 10.f));

	updateShader.useProgram();
	glEnable(GL_RASTERIZER_DISCARD);
	glBindVertexArray(vao[activeBuffer]);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, maxParticles);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	drawShader.setMat4("projectionView", camera.getProjectionViewTransform());
	drawShader.setVec3("cameraPos", camera.getPosition());
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	drawShader.useProgram();
	glBindVertexArray(vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, maxParticles);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	activeBuffer = otherBuffer;
}