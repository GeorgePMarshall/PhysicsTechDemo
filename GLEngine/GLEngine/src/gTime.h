#pragma once
#include <GLFW\glfw3.h>


class gTime
{
private:
	static gTime* timeInstance;
	float dt;
	float curTime;
	float prevTime;


public:

	static void Initialize();
	static gTime* getInstance();

	void Update();

	static float deltaTime();
	static float physicsTimeStep();
	static float Time();


};
