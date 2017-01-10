#pragma once
#include "Camera.h"
#include "Input.h"
#include "gTime.h"



class FlyCamera : public Camera
{
	Input* input;

	GLfloat speed = 5.0f, sprintSpeed = 10.0f;
	GLfloat rotateSpeed = 0.025f;
	vec2 mouseOffset, oldMousePos;
	bool useMouseInput = false;


	void CalculateRotation();


public:
	void initialize() override;
	void update() override;

	void toggleMouseInput(GLFWwindow* window);

};