#pragma once
#include <Application.h>
#include <FlyCamera.h>
#include <ShaderProgram.h>
#include <Mesh.h>
#include <SpecularMaterial.h>
#include <ParticleEmitter.h>
#include <Input.h>
#include <Terrain.h>
#include <vector>
#include <gTime.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <Geometry.h>
#include "Player.h"
#include <PhysicsManager.h>
#include <vector>
#include <Spring.h>
#include "PhysXScene.h"
#include "Cube.h"


//testing aplication for my opengl engine




class TestingApplication : public Application
{
	FlyCamera camera;
	ShaderProgram shader;
	PhysicsManager physicsManager;

	Rigidbody ground;
	Cube cube;
	Cube cube2;
	Player player;
	Player player2;
	Spring* spring;

	GLfloat charge = 0.0f;

	std::vector<Player*> players;
	std::vector<Cube*> cubes;
	std::vector<Spring*> springs;

	PhysXScene physX;

	bool showUI = true;
	vec3 lightDirection;

	std::vector<glm::mat4> cats;
	GLuint i = 0;


	void ThreadedLoad() override;
	void Load() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Shutdown() override;

};