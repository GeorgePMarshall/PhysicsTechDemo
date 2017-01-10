#pragma once
#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Input.h"
#include "gTime.h"
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include "ShaderProgram.h"
#include "Texture.h"
#include "Network.h"

class Application
{
private:
	GLFWwindow* window;

public:
	
	int Run();
	GLFWwindow* GetWindow();
	void RunLoad();

protected:
	virtual void ThreadedLoad() = 0;
	virtual void Load() = 0;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Shutdown() = 0;


};

