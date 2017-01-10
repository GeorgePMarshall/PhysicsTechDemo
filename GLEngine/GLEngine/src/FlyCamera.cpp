#include "FlyCamera.h"


void FlyCamera::initialize()
{
	input = Input::getInput();
}

void FlyCamera::update()
{
	vec3 moveDir(0);
	vec3 right = worldTransform[0].xyz;
	vec3 up = worldTransform[1].xyz;
	vec3 forward = worldTransform[2].xyz;

	if (input->getKey(GLFW_KEY_W))
	{
		moveDir -= forward;
	}

	if (input->getKey(GLFW_KEY_S))
	{
		moveDir += forward;
	}

	if (input->getKey(GLFW_KEY_A))
	{
		moveDir -= right;
	}

	if (input->getKey(GLFW_KEY_D))
	{
		moveDir += right;
	}

	if (input->getKey(GLFW_KEY_C))
	{
		moveDir -= vec3(0, 1, 0);
	}

	if (input->getKey(GLFW_KEY_SPACE))
	{
		moveDir += vec3(0, 1, 0);
	}


	if (glm::length(moveDir))
	{
		if (input->getKey(GLFW_KEY_LEFT_SHIFT))
			moveDir = gTime::deltaTime() * sprintSpeed * glm::normalize(moveDir);
		else
			moveDir = gTime::deltaTime() * speed * glm::normalize(moveDir);

		setPosition(getPosition() + moveDir);
	}

	if(useMouseInput)
		CalculateRotation();

	//LookAt(worldTransform[3].xyz, vec3(0));

}

void FlyCamera::CalculateRotation()
{
	mouseOffset = input->getMousePosition() - oldMousePos;
	oldMousePos = input->getMousePosition();

	if (!glm::length2(mouseOffset)) return;

	if (mouseOffset.y != 0)
	{
		setWorldTransform(getWorldTransform() * glm::rotate(rotateSpeed * gTime::deltaTime() * -mouseOffset.y, vec3(1, 0, 0)));
	}

	if (mouseOffset.x != 0)
	{
		setWorldTransform(getWorldTransform() * glm::rotate(rotateSpeed * gTime::deltaTime() * -mouseOffset.x, vec3(viewTransform[1])));
	}

}

void FlyCamera::toggleMouseInput(GLFWwindow* window)
{
	if(!useMouseInput)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	
	useMouseInput = !useMouseInput;
}

