#include "Input.h"

#include <iostream>

Input* Input::inputInstance = nullptr;

Input::Input(GLFWwindow* window)
{ 
	this->window = window; 
	glfwSetKeyCallback(window, staticKeyCallback);
	glfwSetCursorPosCallback(window, staticMousePosCallBack);

	//glfwSetCursorPos(window, 640, 360);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	for (int i = 0; i < 1024; i++)
	{
		curKeyState[i] = GLFW_RELEASE;
		prevKeyState[i] = GLFW_RELEASE;
	}
}

Input* Input::getInput()
{
	if (!inputInstance)
		return nullptr;
	return inputInstance;
}

void Input::Initialize(GLFWwindow* a_window)
{
	if (!inputInstance)
	{
		inputInstance = new Input(a_window);
	}
}

void Input::staticKeyCallback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode)
{
	getInput()->keyCallback(window, key, scancode, action, mode);
}
void Input::keyCallback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode)
{
	curKeyState[key] = action;

	if (action == GLFW_RELEASE)
		prevKeyState[key] = GLFW_PRESS;
}

void Input::staticMousePosCallBack(GLFWwindow* a_window, GLdouble xPos, GLdouble yPos)
{
	getInput()->mousePosCallBack(a_window, xPos, yPos);
}
void Input::mousePosCallBack(GLFWwindow* a_window, GLdouble xPos, GLdouble yPos)
{
	mousePosition.x = (float)xPos;
	mousePosition.y = (float)yPos;
}


bool Input::getKey(GLint key)
{
	if (curKeyState[key] == GLFW_RELEASE)
		return false;
	return true;
}
bool Input::getKeyDown(GLint key)
{
	//if key not pressed set old state to not pressed
	if (curKeyState[key] == GLFW_RELEASE)
	{
		prevKeyState[key] = GLFW_RELEASE;
		return false;
	}

	//if key is pressed and was not presed last frame
	if(curKeyState[key] == GLFW_PRESS && prevKeyState[key] == GLFW_RELEASE)
	{
		prevKeyState[key] = GLFW_PRESS;
		return true;
	}

	return false;
}
bool Input::getKeyUp(GLint key)
{
	if (curKeyState[key] == GLFW_RELEASE && prevKeyState[key] == GLFW_PRESS)
	{
		prevKeyState[key] = GLFW_RELEASE;
		return true;
	}
	return false;
}


vec2 Input::getMousePosition()
{
	return mousePosition;
}

void Input::setMousePosition(vec2 position)
{
	glfwSetCursorPos(window, position.x, position.y);
}
	
